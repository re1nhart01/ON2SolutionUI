#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

#include <core/structures/ui_queue.h>
#include <algorithm>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include "components/registry.h"
#include "core/state/reactive.h"
#include "core/structures/delegate.h"
#include "esp_log.h"

namespace foundation {
  template <typename T>
  class ThreadReactive : public IReactive {
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
        if (m)
          xSemaphoreTakeRecursive(m, portMAX_DELAY);
      }
      ~Lock() {
        if (m)
          xSemaphoreGiveRecursive(m);
      }
    };

   public:
    explicit ThreadReactive(const T& default_val)
        : value_store(std::make_shared<const T>(default_val)) {
      _mutex = xSemaphoreCreateRecursiveMutex();
    }

    virtual ~ThreadReactive() override {
      if (_mutex) {
        {
          Lock lock(_mutex);
          bindings.clear();
        }
        vSemaphoreDelete(_mutex);
      }
    }

    template <typename TComp>
    void attach(std::string key, TComp* component,
                Delegate<void(TComp*, std::shared_ptr<const T>), 64> updater) {
      if (!_mutex || !component)
        return;
      Lock lock(_mutex);

      Binding binding;
      binding.key = std::move(key);
      binding.component_id = component->get_component_id();

      binding.updater = [updater, id = binding.component_id](
                            void*, std::shared_ptr<const T> val) {
        auto* node = foundation::ComponentRegistry::instance().get(id);

        if (!node)
          return;

        if (!node->alive())
          return;

        auto* typed = dynamic_cast<TComp*>(node);
        if (!typed)
          return;

        if (updater) {
          updater(typed, std::move(val));
        }
      };

      this->bindings.push_back(std::move(binding));
    }

    void add_event_listener(
        std::string key, Delegate<void(std::shared_ptr<const T>), 64> updater) {
      if (!_mutex)
        return;
      Lock lock(_mutex);

      Binding binding;
      binding.key = std::move(key);
      binding.component_id = reinterpret_cast<uintptr_t>(this);

      binding.updater = [updater](void* _, std::shared_ptr<const T> val) {
        if (updater) {
          updater(std::move(val));
        }
      };
      this->bindings.push_back(std::move(binding));
    }

    void remove_event_listener(const std::string& key) {
      if (!_mutex)
        return;
      Lock lock(_mutex);

      auto it =
          std::remove_if(bindings.begin(), bindings.end(),
                         [key](const Binding& b) { return b.key == key; });

      bindings.erase(it);
    }

    void detach(const void* component) override {
      if (!_mutex || !component)
        return;
      Lock lock(_mutex);

      auto* node = static_cast<const VNode*>(component);
      const auto target_id = node->get_component_id();

      auto it = std::remove_if(bindings.begin(), bindings.end(),
                               [target_id](const Binding& b) {
                                 return b.component_id == target_id;
                               });

      if (it != bindings.end()) {
        bindings.erase(it, bindings.end());
      }
    }

    void set(const T& newValue) {
      if (!_mutex)
        return;
      std::vector<Binding> snapshot;
      std::shared_ptr<const T> new_val;

      {
        Lock lock(_mutex);
        new_val = std::make_shared<const T>(newValue);
        value_store = new_val;
        snapshot = bindings;
      }

      _notify(snapshot, new_val);
    }

    void set(Delegate<T(const T&)> fn) {
      if (!_mutex || !fn)
        return;
      std::vector<Binding> snapshot;
      std::shared_ptr<const T> new_val;

      {
        Lock lock(_mutex);
        new_val = std::make_shared<const T>(fn(*value_store));
        value_store = new_val;
        snapshot = bindings;
      }

      _notify(snapshot, new_val);
    }

    void set_silent(Delegate<T(const T&)> fn) {
      if (!_mutex || !fn)
        return;
      Lock lock(_mutex);

      T next = fn(*value_store);
      value_store = std::make_shared<const T>(std::move(next));
      ;
    }

    std::shared_ptr<const T> get() {
      if (!_mutex)
        return nullptr;
      Lock lock(_mutex);
      return value_store;
    }

   private:
    void _notify(const std::vector<Binding>& snapshot,
                 std::shared_ptr<const T> val) {
      for (auto& b : snapshot) {
        if (b.updater) {
          UIQueue::get_instance().push([updater = b.updater, val]() {
            if (updater) {
              updater(nullptr, val);
            }
          });
        }
      }
    }
  };
}  // namespace foundation