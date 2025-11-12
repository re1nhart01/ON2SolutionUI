


#include "driver/uart.h"

#include <string>
#include <bits/range_access.h>

#include "freertos/queue.h"

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
    uart_config_t uart_config = {
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


class UART {
public:
    UART(uart_port_t uart_num, int tx_pin, int rx_pin, int baud_rate = 9600);

    ~UART() = default;

    bool init();

    int send(const char* data, size_t length) {
        return 0;
    };
    int receive(char* buffer, size_t maxLength) {
        return 0;
    };

private:
    uart_port_t _uart_num;
    int _tx_pin;
    int _rx_pin;
    int _baud_rate;
    QueueHandle_t _uart_queue;
};

static void uart_interrupt_handler(void *pvParameters);

bool UART::init() {
    uart_mutex = xSemaphoreCreateMutex();
    uart_driver_install(UART_NUM_1, BUF_SIZE * 2, BUF_SIZE * 2, 20, &uart_queue, 0);
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, this->_tx_pin, this->_rx_pin, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_enable_rx_intr(UART_NUM_1);
    xTaskCreate(uart_interrupt_handler, "uart_event_task", 4096, nullptr, 12, nullptr);
    return true;
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