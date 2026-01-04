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

      std::unique_ptr<TypedValue<T>> value_store;
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
      ThreadReactive() : value_store(nullptr) {
          _mutex = xSemaphoreCreateMutex();
          if (_mutex == NULL) {
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

      void set(const T& newValue)
      {
        if (!_mutex) return;
        Lock lock(_mutex);

        if (value_store && value_store->data == newValue) return;

        value_store = std::make_unique<TypedValue<T>>(newValue);

        for (auto &b : bindings) {
          if (b.component != nullptr && b.updater)
          {
            b.updater(b.component, value_store->data);
          }
        }
      }

      T get() const {
        if (!_mutex) return T();
        Lock lock(_mutex);
        return value_store ? value_store->data : T();
      }
    };
}