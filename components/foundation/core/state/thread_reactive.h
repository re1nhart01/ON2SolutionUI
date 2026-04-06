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
#include <cstdint>

namespace foundation
{
    template <typename T>
    class ThreadReactive : public IReactive
    {
    private:
      struct Binding {
        std::string key;
        uintptr_t component_id;
        bool is_listener = false;
        Delegate<void(void*, std::shared_ptr<const T>), 64> updater;
      };

      std::shared_ptr<const T> value_store;
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
      ThreadReactive(const T& default_val)
        : value_store(std::make_shared<const T>(default_val))
      {
        _mutex = xSemaphoreCreateMutex();
      }

      virtual ~ThreadReactive() override {
          if (_mutex) {
              { Lock lock(_mutex); bindings.clear(); }
              vSemaphoreDelete(_mutex);
          }
      }

      template<typename TComp>
      void attach(std::string key, TComp* component, Delegate<void(TComp*, std::shared_ptr<const T>), 64> updater)
      {
        if (!_mutex || !component) return;
        Lock lock(_mutex);

        Binding binding;
        binding.key = std::move(key);
        binding.component_id = reinterpret_cast<uintptr_t>(component);

        binding.updater = [updater, component](void* /*ignored*/, std::shared_ptr<const T> val) {
          if (updater) {
            updater(component, std::move(val));
          }
        };

        this->bindings.push_back(std::move(binding));
      }


      void add_event_listener(std::string key, Delegate<void(std::shared_ptr<const T>), 64> updater) {
        if (!_mutex) return;
        Lock lock(_mutex);

        Binding binding;
        binding.key = std::move(key);
        binding.component_id = reinterpret_cast<uintptr_t>(this);

        binding.updater = [updater](void* _, std::shared_ptr<const T> val) {
          if (updater) {
            updater(std::move(val));
          }
        };
      }

      void remove_event_listener(const std::string& key) {
        if (!_mutex) return;
        Lock lock(_mutex);

        auto it = std::remove_if(bindings.begin(), bindings.end(), [key](const Binding& b) {
            return b.key == key;
        });
      }

      void detach(const void* component) override
      {
        if (!_mutex || !component) return;
        Lock lock(_mutex);

        auto target_id = reinterpret_cast<uintptr_t>(component);
        auto it = std::remove_if(bindings.begin(), bindings.end(),
            [target_id](const Binding& b) {
                return b.component_id == target_id;
            });

        if (it != bindings.end()) {
            bindings.erase(it, bindings.end());
        }
      }

      void set(Delegate<T(const T&)> fn) {
        if (!_mutex || !fn) return;
        Lock lock(_mutex);

        // Створюємо нові дані на основі старих
        T next = fn(*value_store);
        _broadcast(std::make_shared<const T>(std::move(next)));
      }

      void set_silent(Delegate<T(const T&)> fn) {
        if (!_mutex || !fn) return;
        Lock lock(_mutex);

        T next = fn(*value_store);
        value_store = std::make_shared<const T>(std::move(next));;
      }

      void set(const T& newValue) {
        if (!_mutex) return;
        Lock lock(_mutex);
        _broadcast(std::make_shared<const T>(newValue));
      }

      std::shared_ptr<const T> get() {
        if (!_mutex) return nullptr;
        Lock lock(_mutex);
        return value_store;
      }

    private:
      void _broadcast(std::shared_ptr<const T> new_val) {
        value_store = new_val;

        auto snapshot = bindings;

        for (auto& b : snapshot) {
          if (b.updater) {
            b.updater(nullptr, value_store);
          }
        }
      }
    };
}