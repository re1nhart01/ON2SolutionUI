#pragma once


namespace foundation {
  class Debounce
  {
  private:
    TimerHandle_t timer_handle = nullptr;
    TickType_t delay_ticks = pdMS_TO_TICKS(500);
    Delegate<void(), 32> callback;

  public:
    explicit Debounce(uint32_t delay) : delay_ticks(pdMS_TO_TICKS(delay))
    {
      this->timer_handle = xTimerCreate(
        "interval",
        delay_ticks,
        pdFALSE,
        this,
        &Debounce::timer_cb
      );
    };
    
    Debounce(const Debounce&) = delete;
    Debounce& operator=(const Debounce&) = delete;

    ~Debounce() {
      if (timer_handle) {
          xTimerDelete(timer_handle, portMAX_DELAY);
      }
    }

    void exec(const Delegate<void()>& task)
    {
      callback = task;

      xTimerStop(timer_handle, 0);
      xTimerChangePeriod(timer_handle, delay_ticks, 0);
      xTimerStart(timer_handle, 0);
    }

    void cancel() const
    {
      if (timer_handle != nullptr)
      {
        xTimerStop(timer_handle, 0);
      }
    }

    static void timer_cb(TimerHandle_t xTimer) {
      auto* self = static_cast<Debounce*>(pvTimerGetTimerID(xTimer));
      if (self && self->callback) {
          self->callback();
      }
    }
  };
}
