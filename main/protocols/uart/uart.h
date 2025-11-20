

#pragma once


#include "driver/uart.h"

#include "core/structures/delegate.h"
#include "core/structures/static_hashmap.h"

#include <bits/range_access.h>
#include <string>

#include "freertos/queue.h"

#include <cstring>
#include <tgmath.h>

extern "C" {
  #include "esp_log.h"
  #include "freertos/FreeRTOS.h"
  #include "freertos/task.h"
  #include "driver/gpio.h"
  #include "sdkconfig.h"
}


static const char *TAG = "example";
static SemaphoreHandle_t uart_mutex;
static QueueHandle_t uart_queue = nullptr;
#define BUF_SIZE (1024)

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

class UartHandler
{
private:
  int tx_pin;
  int rx_pin;
  int baud_rate;
  uart_port_t current_uart_num = UART_NUM_1;
  uint32_t stack_dept = 4096;

public:
  struct UartHandlerEventKey
  {
    char* key_v;
    uart_event_t event;
  };
  struct UartCallbackResponse
  {
    uint8_t origin_data;
    void* other_data;
  };
  StaticHashMap<UartHandlerEventKey, foundation::Delegate<UartCallbackResponse()>, 32> map;

  explicit UartHandler(const uart_port_t uart_num, const int tx_pin,
                       const int rx_pin, const int baud_rate = 9600, const uint32_t stack_dept)
  {
    this->current_uart_num = uart_num;
    this->tx_pin = tx_pin;
    this->rx_pin = rx_pin;
    this->baud_rate = baud_rate;
    this->stack_dept = stack_dept;
  };

  bool init() const
  {
    uart_mutex = xSemaphoreCreateMutex();
    uart_driver_install(current_uart_num, BUF_SIZE * 2, BUF_SIZE * 2, 20, &uart_queue, 0);
    uart_param_config(current_uart_num, &uart_config);
    uart_set_pin(current_uart_num, this->tx_pin, this->rx_pin, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_enable_rx_intr(current_uart_num);
    xTaskCreate(uart_interrupt_handler, "uart_event_task", stack_dept, nullptr, 12, nullptr);
    return true;
  }

  void add_event_listener(const UartHandlerEventKey key,
    const foundation::Delegate<UartCallbackResponse()> &callback)
  {

    this->map.put(key, callback);
  }

  int send(const char* text, size_t len) const
  {
    const uint8_t length = strlen(text);
    uart_write_bytes(this->current_uart_num, text, length);
    return length;
  };
  int send(const std::string& text) const
  {
    const uint8_t length = strlen(text.c_str());
    uart_write_bytes(this->current_uart_num, text.c_str(), length);
    return length;
  };

  void remove_event_listener()
  {
  }

  void enable_rx(bool value)
  {

  }

  static void uart_interrupt_handler(void *pvParameters) {
    uart_event_t event;
    uint8_t data[BUF_SIZE];

    if (!uart_queue) {
        ESP_LOGE(TAG, "UART queue is NULL!");
        return;
    }


    while (xQueueReceive(uart_queue, &event, portMAX_DELAY)) {
        if (xSemaphoreTake(uart_mutex, portMAX_DELAY)) {
            int len = 0;
            size_t length = 0;
            std::string str = "";
            switch (event.type) {
              case UART_DATA:
                len = uart_read_bytes(UART_NUM_1, data, event.size, pdMS_TO_TICKS(100));

                if (len > 0 && len < sizeof(data)) {
                    data[len] = '\0';
                } else {
                    data[sizeof(data) - 1] = '\0';
                }
                // length = std::size(data);
                // str = std::string(reinterpret_cast<const char*>(data), length);
                // lv_label_set_text(label_counter, std::format("{}", str).c_str());
                break;
              case UART_FIFO_OVF:
                ESP_LOGW(TAG, "UART FIFO Overflow!");
                uart_flush_input(UART_NUM_1);
                xQueueReset(uart_queue);
                break;
              case UART_BUFFER_FULL:
                ESP_LOGW(TAG, "UART Buffer Full!");
                uart_flush_input(UART_NUM_1);
                xQueueReset(uart_queue);
                break;
              case UART_PARITY_ERR:
                ESP_LOGE(TAG, "UART Parity Error");
                break;
              case UART_FRAME_ERR:
                ESP_LOGE(TAG, "UART Frame Error");
                break;
              default:
                ESP_LOGW(TAG, "Unhandled UART event: %d", event.type);
                break;
            }
            xSemaphoreGive(uart_mutex);
        }
    }
  }
};