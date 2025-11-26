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
static SemaphoreHandle_t uart_mutex;
static QueueHandle_t uart_queue;

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
    uint8_t data[BUF_SIZE];
    size_t len;
    bool flag;
  };
  struct UartCallbackResponse
  {
    UartData response;
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

  int tx_pin;
  int rx_pin;
  int baud_rate = 9600;
  uart_port_t current_uart_num;
  uint32_t stack_dept;
  size_t size = 0;
  std::array<UartTypes::UartHandlerEvent, 32> list{};
  TaskHandle_t task_handle = nullptr;

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
    const int rx_pin, const int baud_rate,
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
    xTaskCreate(uart_interrupt_handler_trampoline, "uart_event_task", stack_dept, this, 12, &this->task_handle);
    return true;
  }

  static void uart_interrupt_handler_trampoline(void *pvParameters) {
    if (const auto self = static_cast<UartHandler *>(pvParameters); self != nullptr)
    {
      self->uart_interrupt_handler();
    }
  }

  void add_event_listener(const UartTypes::UartHandlerEvent& event)
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

  void remove_all_event_listeners()
  {
    if (uart_queue == nullptr) {
        ESP_LOGW(TAG, "[remove_all_event_listeners] UART queue is NULL — nothing to remove");
        return;
    }

    this->enable_rx(false);
    if (task_handle != nullptr) {
        ESP_LOGI(TAG, "Deleting UART task...");
        vTaskDelete(task_handle);
        task_handle = nullptr;
    }

    this->list = {};
    ESP_LOGI(TAG, "Deleting UART driver...");
    if (const esp_err_t err = uart_driver_delete(current_uart_num); err != ESP_OK) {
        ESP_LOGE(TAG, "uart_driver_delete failed: %s", esp_err_to_name(err));
    }

    uart_queue = nullptr;

    if (uart_mutex != nullptr) {
        vSemaphoreDelete(uart_mutex);
        uart_mutex = nullptr;
    }

    ESP_LOGI(TAG, "UART event listeners fully removed");
  }

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
    if (!uart_queue) {
        ESP_LOGE(TAG, "UART queue is NULL!");
        return;
    }

    while (xQueueReceive(uart_queue, &event, pdMS_TO_TICKS(600))) {
        if (xSemaphoreTake(uart_mutex, portMAX_DELAY)) {
            switch (event.type) {
              case UART_DATA:
                {
                  UartTypes::UartData uart_data;
                  size_t len = 0;

                  len = uart_read_bytes(this->current_uart_num,
                                        uart_data.data,
                                        event.size,
                                        pdMS_TO_TICKS(600));

                  if(len > 0 && len < sizeof(uart_data.data))
                    {
                      uart_data.data[len] = '\0';
                    } else {
                        uart_data.data[sizeof(uart_data.data) - 1] = '\0';
                    }

                  uart_data.len = len;
                  uart_data.flag = true;

                  auto str = std::string(reinterpret_cast<const char*>(uart_data.data), uart_data.len);

                  // ESP_LOG_BUFFER_HEXDUMP(TAG, uart_data.data, uart_data.len, ESP_LOG_INFO);
                  ESP_LOGI("TAB", str.c_str());
                  ESP_LOGI("UART_CALLBACK", "data: %.*s", uart_data.len, uart_data.data);

                  ESP_LOGI(TAG, "EVENT_ARRIVED");
                  ESP_LOGI(TAG, "Received %d bytes: %s", len, uart_data.data); // Use data.data.data()

                  UartTypes::UartCallbackResponse resp{ .response = uart_data };
                  execute_callback_event(UART_DATA, resp);
                }
                break;
              case UART_FIFO_OVF:
                uart_flush_input(this->current_uart_num);
                xQueueReset(uart_queue);
                {
                  UartTypes::UartCallbackResponse resp{ .response = {} };
                  this->execute_callback_event(UART_FIFO_OVF, resp);
                }
                break;
              case UART_BUFFER_FULL:
                uart_flush_input(this->current_uart_num);
                xQueueReset(uart_queue);
                {
                  UartTypes::UartCallbackResponse resp{ .response = {} };
                  this->execute_callback_event(UART_BUFFER_FULL, resp);
                }
                break;
              case UART_PARITY_ERR:
                {
                  UartTypes::UartCallbackResponse resp{ .response = {} };
                  this->execute_callback_event(UART_PARITY_ERR, resp);
                }
                break;
              case UART_FRAME_ERR:
                {
                  UartTypes::UartCallbackResponse resp{ .response = {} };
                  this->execute_callback_event(UART_FRAME_ERR, resp);
                }
                break;
              default:
                ESP_LOGW(TAG, "Unhandled UART event: %d", event.type);
                {
                  UartTypes::UartCallbackResponse resp{ .response = {} };
                  this->execute_callback_event(UART_WAKEUP, resp);
                }
                break;
            }
            xSemaphoreGive(uart_mutex);
        }
    }
  }
};