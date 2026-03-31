#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "core/structures/delegate.h"
#include "core/state/reactive.h"
#include "esp_log.h"
#include <memory>
#include <vector>
#include <string>
#include <algorithm>

namespace foundation
{
    template <typename T>
    class ThreadReactive : public IReactive
    {
    private:
      struct Binding {
        std::string key;
        void* component;
        Delegate<void(void*, const T&), 40> updater;

        uint32_t listener_id = 0;
      };

      T value_store;
      std::vector<Binding> bindings;
      SemaphoreHandle_t _mutex;
      uint32_t _next_id = 1;

      struct Lock {
          SemaphoreHandle_t m;
          explicit Lock(SemaphoreHandle_t m) : m(m) {
            if (m) xSemaphoreTake(m, portMAX_DELAY);
          }
          ~Lock() {
            if (m) xSemaphoreGive(m);
          }
      };

    public:
      ThreadReactive(const T& default_val) : value_store(default_val)
      {
        _mutex = xSemaphoreCreateMutex();
        if(_mutex == nullptr)
        {
          ESP_LOGE("REACTIVE", "Failed to create mutex!");
        }
      }

      virtual ~ThreadReactive() override {
        if (_mutex) {
          {
            Lock lock(_mutex);
            bindings.clear();
          }
          vSemaphoreDelete(_mutex);
          _mutex = nullptr;
        }
      }

      template<typename TComp>
      void attach(std::string key, TComp* component, Delegate<void(TComp*, const T&), 40> updater)
      {
        if (!_mutex) return;
        Lock lock(_mutex);

        Binding binding;
        binding.key = std::move(key);
        binding.listener_id = 0;
        binding.component = static_cast<void*>(component);
        binding.updater = [updater](void* comp_ptr, const T& val) {
          if (comp_ptr && updater) {
            updater(static_cast<TComp*>(comp_ptr), val);
          }
        };

        this->bindings.push_back(std::move(binding));
      }

      uint32_t addEventListener(std::string key, Delegate<void(const T&), 40> callback)
      {
        if (!_mutex) return 0;
        Lock lock(_mutex);

        uint32_t id = _next_id++;

        Binding binding;
        binding.key = std::move(key);
        binding.component = nullptr;
        binding.listener_id = id;

        binding.updater = [callback](void* /*ignored*/, const T& val) {
          if (callback) {
            callback(val);
          }
        };

        this->bindings.push_back(std::move(binding));
        return id;
      }

      void removeEventListener(uint32_t id)
      {
        if (!_mutex || id == 0) return;
        Lock lock(_mutex);

        auto it = std::remove_if(bindings.begin(), bindings.end(),
            [id](const Binding& b) { return b.listener_id == id; });

        if (it != bindings.end()) {
          bindings.erase(it, bindings.end());
        }
      }

      void detach(const void *component) override
      {
        if (!_mutex) return;
        Lock lock(_mutex);

        auto it = std::remove_if(bindings.begin(), bindings.end(),
            [component](const Binding& b) {
                return b.component == component && b.listener_id == 0;
            });

        if (it != bindings.end()) {
          bindings.erase(it, bindings.end());
        }
      }

      void set(Delegate<T(const T&)> fn) {
        if (!_mutex || !fn) return;

        T next;
        {
          Lock lock(_mutex);
          next = fn(value_store);
          value_store = next;

          for (auto& b : bindings) {
            b.updater(b.component, value_store);
          }
        }
      }

      void set(const T& newValue) {
        if (!_mutex) return;

        std::vector<Binding> snapshot;

        {
          Lock lock(_mutex);
          value_store = newValue;
          snapshot = bindings;
        }

        for (auto& b : snapshot) {
          b.updater(b.component, newValue);
        }
      }

      T get() {
        if (!_mutex) return T();
        Lock lock(_mutex);
        return value_store;
      }
    };
}