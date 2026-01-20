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
      };

      T value_store;
      std::vector<Binding> bindings;
      SemaphoreHandle_t _mutex;

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
        binding.component = static_cast<void*>(component);
        binding.updater = [updater](void* comp_ptr, const T& val) {
          if (comp_ptr && updater) {
            updater(static_cast<TComp*>(comp_ptr), val);
          }
        };

        this->bindings.push_back(std::move(binding));
      }

      void detach(const void *component) override
      {
        if (!_mutex) return;
        Lock lock(_mutex);

        auto it = std::remove_if(bindings.begin(), bindings.end(),
            [component](const Binding& b) { return b.component == component; });

        if (it != bindings.end()) {
            bindings.erase(it, bindings.end());
        }
      }

      void set(Delegate<T(const T&)> fn) {
        if (!_mutex || !fn) return;

        T current;
        {
          Lock lock(_mutex);
          current = value_store;
          T next = fn(current);
          set(next);
        }
      }

      void set(const T& newValue) {
        if (!_mutex) return;

        std::vector<Binding> snapshot;

        {
          Lock lock(_mutex);
          value_store = newValue;
          snapshot = bindings;
          for (auto& b : snapshot) {
            if (b.component && b.updater) {
              b.updater(b.component, newValue);
            }
          }
        }
      }

      T get() {
        if (!_mutex) return T();
        Lock lock(_mutex);
        return value_store;
      }
    };
}