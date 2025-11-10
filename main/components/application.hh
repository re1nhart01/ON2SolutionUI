#pragma once

#include <functional>
#include "component.hh"
#include <ctime>
#include <string>
#include "config.hh"

extern "C" {
    #include "esp_log.h"
    #include "core/lvgl_port.h"
}



namespace foundation {
    class Application {
    protected:
        lv_obj_t* screen = nullptr;
        Component* root = nullptr;

    public:
        Application(lv_obj_t* screen_v) {
            this->screen = screen_v;
        };
        virtual ~Application() = default;

        virtual Component* root_component() = 0;
        virtual void before_load_application() = 0;
        virtual void after_load_application() = 0;


        void set_root(Component *root_component);
        void renderApp();
        void tick(lv_obj_t* scr, std::function<void()> callback);
        void turnOnBacklight();
        void turnOffBacklight();
    };

    inline void Application::set_root(Component* root_component) {
        this->root = root_component;
    }


    inline void Application::turnOnBacklight() {
        // waveshare_rgb_lcd_bl_on();
    }

    inline void Application::turnOffBacklight() {
        // waveshare_rgb_lcd_bl_off();
    }

    inline void Application::renderApp() {
        if (this->screen == nullptr) return;
        if (this->root == nullptr) {
            this->before_load_application();

            Component* root_component_view = this->root_component();
            this->set_root(root_component_view);
        }

        if (this->root != nullptr) {
            this->root->set_parent(this->screen);

            if (lvgl_port_lock(-1)) {
                lv_obj_t* rendered = this->root->render();
                this->root->set_component(rendered);

                this->after_load_application();

#if IS_MULTITHREAD
                ESP_LOGI(APP_LOG_TAG, "Running on single-threaded LVGL loop");
#endif
#if DISPLAY_BASE_TYPE_WAVESHARE_DISPLAY
                ESP_LOGI(APP_LOG_TAG, "Running waveshare display");
#endif

                lvgl_port_unlock();
            }
        }
    }


    inline void Application::tick(lv_obj_t* scr, std::function<void()> callback) {
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

} // namespace foundation
