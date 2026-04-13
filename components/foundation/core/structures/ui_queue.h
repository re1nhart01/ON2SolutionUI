//
// Created by evgeniy on 4/12/26.
//

#pragma once

#include "delegate.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include <queue>

namespace foundation {
  class UIQueue {
  private:
    std::queue<Delegate<void()>> queue_delegates{};
    SemaphoreHandle_t mutex;
  public:
    explicit UIQueue() {
      this->mutex = xSemaphoreCreateMutex();
    };

    ~UIQueue() {
      if (mutex) {
        vSemaphoreDelete(mutex);
      }
    }

    struct Lock {
      SemaphoreHandle_t m;
      explicit Lock(SemaphoreHandle_t m) : m(m) {
        if (m) xSemaphoreTake(m, portMAX_DELAY);
      }
      ~Lock() {
        if (m) xSemaphoreGive(m);
      }
    };

    static UIQueue& get_instance() {
      static UIQueue instance;
      return instance;
    }

    void push(Delegate<void()> delegate) {
      Lock lock(mutex);
      this->queue_delegates.push(std::move(delegate));
    }

    void pop() {
      Lock lock(mutex);
      if (!queue_delegates.empty()) {
        queue_delegates.pop();
      }
    }

    void clear() {
      Lock lock(mutex);
      std::queue<Delegate<void()>> local;
      std::swap(local, queue_delegates);
    }

    void process() {
      std::queue<Delegate<void()>> local;

      {
        Lock lock(mutex);
        std::swap(local, queue_delegates);
      }

      while (!local.empty()) {
        auto fn = std::move(local.front());
        local.pop();

        if (fn) {
          fn();
        }
      }
    }

  };
}
