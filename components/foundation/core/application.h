#pragma once

#include "../components/component.h"
#include "../config.hh"
#include <ctime>
#include <functional>
#include <string>

extern "C" {
#include "internals/lvgl_port.h"
#include "esp_log.h"
}
  namespace foundation {
    class Application {
    protected:
      lv_obj_t* screen = nullptr;
      std::shared_ptr<VNode> root_node = nullptr;

    public:
      Application(lv_obj_t* screen_v) : screen(screen_v) {};
      virtual ~Application() = default;

      virtual std::shared_ptr<VNode> root_component() { return nullptr; }

      virtual void on_init() {}

      virtual void before_load_application() = 0;
      virtual void after_load_application() = 0;

      void renderApp() {
        if (this->screen == nullptr) return;

        this->before_load_application();

        if (lvgl_port_lock(-1)) {

            this->on_init();
  #if IS_MULTITHREAD
            ESP_LOGI(APP_LOG_TAG, "Running on single-threaded LVGL loop");
  #endif
  #if DISPLAY_BASE_TYPE_WAVESHARE_DISPLAY
            ESP_LOGI(APP_LOG_TAG, "Running waveshare display");
  #endif
            if (!this->root_node) {
                this->root_node = this->root_component();

                if (this->root_node) {
                    this->root_node->set_parent(this->screen);
                    lv_obj_t* rendered = this->root_node->render();
                    this->root_node->set_component(rendered);
                }
            }

            lvgl_port_unlock();
        }

        this->after_load_application();
      }

      void tick(lv_obj_t* scr, std::function<void()> callback) const
      {
        if (lvgl_port_lock(-1)) {
  #if IS_MULTITHREAD
            ESP_LOGI(APP_LOG_TAG, "Running on single-threaded LVGL loop");
  #endif

            std::time_t result = std::time(nullptr);
            std::string timeStr = std::asctime(std::localtime(&result));
            callback();
            lvgl_port_unlock();
        }

      }
    };
  }