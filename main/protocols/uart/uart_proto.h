#pragma once

#include "driver/uart.h"

#include "core/structures/delegate.h"
#include "core/structures/static_hashmap.h"

#include <bits/range_access.h>
#include <string>

#include "freertos/queue.h"

#include <cstring>
#include <sys/stat.h>
#include <tgmath.h>

extern "C" {
  #include "esp_log.h"
  #include "freertos/FreeRTOS.h"
  #include "freertos/task.h"
  #include "driver/gpio.h"
  #include "sdkconfig.h"
}

static auto TAG = "example";

#define BUF_SIZE (1024)
#define UART_EMPTY_THRESH_DEFAULT (10)

extern "C" {
  inline uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 122,
        .source_clk = UART_SCLK_DEFAULT,
        .flags = {
            .allow_pd = 0,
            .backup_before_sleep = 0
        }
    };
}

namespace UartTypes
{
  struct UartData
  {
    std::array<char, BUF_SIZE> data;
    size_t len;
    bool flag;
  };
  struct UartCallbackResponse
  {
    UartData data;
  };

  struct UartHandlerEvent
  {
    char* key_v;
    uart_event_type_t event;
    foundation::Delegate<void(UartCallbackResponse)> delegate;
  };
}



class UartHandler
{
private:
  SemaphoreHandle_t uart_mutex = nullptr;
  QueueHandle_t uart_queue = nullptr;
  int tx_pin;
  int rx_pin;
  int baud_rate;
  uart_port_t current_uart_num = UART_NUM_1;
  uint32_t stack_dept = 4096;
  size_t size = 0;
  std::array<UartTypes::UartHandlerEvent, 32> list{};

  bool push_array(const UartTypes::UartHandlerEvent& value)
  {
    if (size < list.size()) {
        list[size++] = value;
        return true;
    }
    return false;
  }

public:
  explicit UartHandler(const uart_port_t uart_num, const int tx_pin,
    const int rx_pin, const int baud_rate = 9600,
    const uint32_t stack_dept)
  {
    this->current_uart_num = uart_num;
    this->tx_pin = tx_pin;
    this->rx_pin = rx_pin;
    this->baud_rate = baud_rate;
    this->stack_dept = stack_dept;
  };

  bool init()
  {
    uart_mutex = xSemaphoreCreateMutex();
    uart_driver_install(current_uart_num, BUF_SIZE * 2, BUF_SIZE * 2, 20, &uart_queue, 0);
    uart_param_config(current_uart_num, &uart_config);
    uart_set_pin(current_uart_num, this->tx_pin, this->rx_pin, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_enable_rx_intr(current_uart_num);
    xTaskCreate(uart_interrupt_handler_trampoline, "uart_event_task", stack_dept, this, 12, nullptr);
    return true;
  }

  static void uart_interrupt_handler_trampoline(void *pvParameters) {
    if (auto self = static_cast<UartHandler *>(pvParameters); self != nullptr)
    {
      self->uart_interrupt_handler();
    }
  }

  void add_event_listener(const UartTypes::UartHandlerEvent &event)
  {
    xSemaphoreTake(uart_mutex, portMAX_DELAY);
    push_array(event);
    xSemaphoreGive(uart_mutex);
  }

  int send(const char* text) const
  {
    xSemaphoreTake(uart_mutex, portMAX_DELAY);

    const uint8_t length = strlen(text);
    uart_write_bytes(this->current_uart_num, text, length);

    xSemaphoreGive(uart_mutex);
    return length;
  };

  int send(const std::string& text) const
  {
    xSemaphoreTake(uart_mutex, portMAX_DELAY);

    const uint8_t length = strlen(text.c_str());
    uart_write_bytes(this->current_uart_num, text.c_str(), length);

    xSemaphoreGive(uart_mutex);
    return length;
  };

  void remove_event_listener(const char * key_v)
  {
    xSemaphoreTake(uart_mutex, portMAX_DELAY);

    for (size_t i = 0; i < size; i++)
    {
        if (strcmp(list[i].key_v, key_v) == 0)
        {
          for (size_t j = i; j < size - 1; j++)
          {
              list[j] = list[j + 1];
          }
          size--;
          xSemaphoreGive(uart_mutex);
          return;
        }
    }

    xSemaphoreGive(uart_mutex);
  }

  void enable_rx(const bool value) const
  {
    xSemaphoreTake(uart_mutex, portMAX_DELAY);
    if (value)
      {
        uart_enable_rx_intr(this->current_uart_num);
        uart_enable_tx_intr(this->current_uart_num, 1, UART_EMPTY_THRESH_DEFAULT);
      }
      else
      {
        uart_disable_rx_intr(this->current_uart_num);
        uart_disable_tx_intr(this->current_uart_num);
      }
    xSemaphoreGive(uart_mutex);
  }

  void execute_callback_event(const uart_event_type_t event, const UartTypes::UartCallbackResponse& data)
  {
    for (size_t i = 0; i < size; i++) {
        if (list[i].event == event) {
            list[i].delegate(data);
        }
    }
  }

  void uart_interrupt_handler()
  {
    uart_event_t event;
    uint8_t buff[BUF_SIZE];

    if (!uart_queue) {
        ESP_LOGE(TAG, "UART queue is NULL!");
        return;
    }

    while (xQueueReceive(uart_queue, &event, portMAX_DELAY)) {
        if (xSemaphoreTake(uart_mutex, portMAX_DELAY)) {
            switch (event.type) {
              case UART_DATA:
                {
                  size_t len = 0;
                  len = uart_read_bytes(this->current_uart_num, buff, event.size, pdMS_TO_TICKS(100));

                  if(len > 0 && len < sizeof(buff))
                    {
                      buff[len] = '\0';
                    }
                  else
                    {
                      buff[sizeof(buff) - 1] = '\0';
                    }

                  UartTypes::UartData data;
                  data.len = len;
                  data.flag = true;
                  memcpy(data.data.data(), buff, len);
                  data.data[len] = '\0';

                  UartTypes::UartCallbackResponse resp{ .data = data };
                  execute_callback_event(UART_DATA, resp);
                }
                break;
              case UART_FIFO_OVF:
                uart_flush_input(this->current_uart_num);
                xQueueReset(uart_queue);
                {
                  UartTypes::UartCallbackResponse resp{ .data = {} };
                  this->execute_callback_event(UART_FIFO_OVF, resp);
                }
                break;
              case UART_BUFFER_FULL:
                uart_flush_input(this->current_uart_num);
                xQueueReset(uart_queue);
                {
                  UartTypes::UartCallbackResponse resp{ .data = {} };
                  this->execute_callback_event(UART_BUFFER_FULL, resp);
                }
                break;
              case UART_PARITY_ERR:
                {
                  UartTypes::UartCallbackResponse resp{ .data = {} };
                  this->execute_callback_event(UART_PARITY_ERR, resp);
                }
                break;
              case UART_FRAME_ERR:
                {
                  UartTypes::UartCallbackResponse resp{ .data = {} };
                  this->execute_callback_event(UART_FRAME_ERR, resp);
                }
                break;
              default:
                ESP_LOGW(TAG, "Unhandled UART event: %d", event.type);
                {
                  UartTypes::UartCallbackResponse resp{ .data = {} };
                  this->execute_callback_event(UART_WAKEUP, resp);
                }
                break;
            }
            xSemaphoreGive(uart_mutex);
        }
    }
  }
};