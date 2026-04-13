#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

#include <unordered_map>
#include <cstdint>

namespace foundation {
  class VNode;

  class ComponentRegistry {
  private:
    std::unordered_map<uintptr_t, VNode*> registry;
    SemaphoreHandle_t mutex;

    struct Lock {
      SemaphoreHandle_t m;
      explicit Lock(SemaphoreHandle_t m) : m(m) {
        if (m) xSemaphoreTakeRecursive(m, portMAX_DELAY);
      }
      ~Lock() {
        if (m) xSemaphoreGiveRecursive(m);
      }
    };

  public:
    ComponentRegistry() {
      mutex = xSemaphoreCreateRecursiveMutex();
    }

    ~ComponentRegistry() {
      if (mutex) {
        vSemaphoreDelete(mutex);
        mutex = nullptr;
      }
    }

    static ComponentRegistry& instance() {
      static ComponentRegistry inst;
      return inst;
    }

    void register_component(uintptr_t id, VNode* ptr) {
      if (!mutex) return;
      Lock lock(mutex);
      registry[id] = ptr;
    }

    void unregister_component(uintptr_t id) {
      if (!mutex) return;
      Lock lock(mutex);
      registry.erase(id);
    }

    bool is_alive(uintptr_t id) const {
      if (!mutex) return false;
      Lock lock(mutex);
      return registry.contains(id);
    }

    VNode* get(uintptr_t id) {
      if (!mutex) return nullptr;
      Lock lock(mutex);

      auto it = registry.find(id);
      if (it != registry.end()) {
        return it->second;
      }
      return nullptr;
    }
  };

}