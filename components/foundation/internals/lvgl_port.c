#include "lvgl_port.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_rgb.h"
#include "esp_lcd_touch.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "esp_heap_caps.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "lvgl.h"

static const char *TAG = "lv_port";
static SemaphoreHandle_t lvgl_mux;
static TaskHandle_t lvgl_task_handle = NULL;

#define BUF_SIZE (800 * 40)

static lv_disp_draw_buf_t disp_buf;
static lv_disp_drv_t disp_drv;

static void flush_callback(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map)
{
    esp_lcd_panel_handle_t panel_handle = (esp_lcd_panel_handle_t) drv->user_data;

    esp_lcd_panel_draw_bitmap(panel_handle, area->x1, area->y1, area->x2 + 1, area->y2 + 1, color_map);

    if (lv_disp_flush_is_last(drv)) {
        if (lvgl_task_handle) {
            ulTaskNotifyValueClear(NULL, ULONG_MAX);
            // Чекаємо сигналу від rgb_lcd_on_vsync_event (макс 16мс)
            ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(16));
        }
    }

    lv_disp_flush_ready(drv);
}

static lv_disp_t *display_init(esp_lcd_panel_handle_t panel_handle)
{
    assert(panel_handle);

    ESP_LOGI(TAG, "Allocating %d bytes for LVGL buffers in SRAM", BUF_SIZE * sizeof(lv_color_t) * 2);

    void *buf1 = heap_caps_malloc(BUF_SIZE * sizeof(lv_color_t), MALLOC_CAP_INTERNAL | MALLOC_CAP_DMA);
    void *buf2 = heap_caps_malloc(BUF_SIZE * sizeof(lv_color_t), MALLOC_CAP_INTERNAL | MALLOC_CAP_DMA);

    if (buf1 == NULL || buf2 == NULL) {
        ESP_LOGW(TAG, "SRAM full! Allocating LVGL buffers in PSRAM");
        if (!buf1) buf1 = heap_caps_malloc(BUF_SIZE * sizeof(lv_color_t), MALLOC_CAP_SPIRAM);
        if (!buf2) buf2 = heap_caps_malloc(BUF_SIZE * sizeof(lv_color_t), MALLOC_CAP_SPIRAM);
    }

    if (buf1 == NULL || buf2 == NULL) {
        ESP_LOGE(TAG, "Critical: Failed to allocate memory for display buffers!");
        abort();
    }

    lv_disp_draw_buf_init(&disp_buf, buf1, buf2, BUF_SIZE);

    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = 800;
    disp_drv.ver_res = 480;
    disp_drv.flush_cb = flush_callback;
    disp_drv.draw_buf = &disp_buf;
    disp_drv.user_data = panel_handle;
    
    disp_drv.full_refresh = 0;
    disp_drv.direct_mode = 0;

    return lv_disp_drv_register(&disp_drv);
}

static void touchpad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    esp_lcd_touch_handle_t tp = (esp_lcd_touch_handle_t)indev_drv->user_data;
    uint16_t tp_x, tp_y;
    uint8_t tp_cnt = 0;

    esp_lcd_touch_read_data(tp);
    if (esp_lcd_touch_get_coordinates(tp, &tp_x, &tp_y, NULL, &tp_cnt, 1) && tp_cnt > 0) {
        data->point.x = tp_x;
        data->point.y = tp_y;
        data->state = LV_INDEV_STATE_PRESSED;
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

static lv_indev_t *indev_init(esp_lcd_touch_handle_t tp)
{
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touchpad_read;
    indev_drv.user_data = tp;
    return lv_indev_drv_register(&indev_drv);
}

static void tick_increment(void *arg) {
    lv_tick_inc(LVGL_PORT_TICK_PERIOD_MS);
}

static esp_err_t tick_init(void) {
    const esp_timer_create_args_t tick_timer_args = {
        .callback = &tick_increment,
        .name = "lvgl_tick"
    };
    esp_timer_handle_t tick_timer = NULL;
    esp_timer_create(&tick_timer_args, &tick_timer);
    return esp_timer_start_periodic(tick_timer, LVGL_PORT_TICK_PERIOD_MS * 1000);
}

static void lvgl_port_task(void *arg)
{
    while (1) {
        if (lvgl_port_lock(-1)) {
            uint32_t delay = lv_timer_handler();
            lvgl_port_unlock();
            
            if (delay > LVGL_PORT_TASK_MAX_DELAY_MS) delay = LVGL_PORT_TASK_MAX_DELAY_MS;
            else if (delay < LVGL_PORT_TASK_MIN_DELAY_MS) delay = LVGL_PORT_TASK_MIN_DELAY_MS;
            
            vTaskDelay(pdMS_TO_TICKS(delay));
        }
    }
}

esp_err_t lvgl_port_init(esp_lcd_panel_handle_t lcd_handle, esp_lcd_touch_handle_t tp_handle)
{
    lv_init();
    tick_init();

    lv_disp_t *disp = display_init(lcd_handle);
    if (tp_handle) indev_init(tp_handle);

    lvgl_mux = xSemaphoreCreateRecursiveMutex();
    
    xTaskCreatePinnedToCore(lvgl_port_task, "lvgl", LVGL_PORT_TASK_STACK_SIZE, NULL,
                            LVGL_PORT_TASK_PRIORITY, &lvgl_task_handle, LVGL_PORT_TASK_CORE);
    return ESP_OK;
}

bool lvgl_port_lock(int timeout_ms) {
    return xSemaphoreTakeRecursive(lvgl_mux, timeout_ms < 0 ? portMAX_DELAY : pdMS_TO_TICKS(timeout_ms)) == pdTRUE;
}

void lvgl_port_unlock(void) {
    xSemaphoreGiveRecursive(lvgl_mux);
}

bool lvgl_port_notify_rgb_vsync(void)
{
    BaseType_t need_yield = pdFALSE;
    if (lvgl_task_handle) {
        xTaskNotifyFromISR(lvgl_task_handle, ULONG_MAX, eNoAction, &need_yield);
    }
    return (need_yield == pdTRUE);
}