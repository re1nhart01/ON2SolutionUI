
#pragma once
#include <functional>
#include <string>

extern "C" {
  #include "lv_demos.h"
}
typedef void (*callback_keyboard)(lv_event_t *);
//using callback_keyboard = void (*)(lv_event_t *);

namespace foundation {
    class KeyboardManager {
    private:
        lv_obj_t* keyboard = nullptr;
        std::function<void(std::string)> on_submit_callback;
        callback_keyboard lambda = nullptr;
    public:
        void create(lv_obj_t* parent) {
            if (!keyboard) {
                keyboard = lv_keyboard_create(parent);
                lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
                lv_obj_align(keyboard, LV_ALIGN_BOTTOM_MID, 0, 0);
            }
        }

        void attach(lv_obj_t* textarea) {
            if (!keyboard) return;
            lv_keyboard_set_textarea(keyboard, textarea);
            lv_obj_clear_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
        }

        void hide() {
            if (keyboard) {
                lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
                lv_keyboard_set_textarea(keyboard, nullptr);
            }
        }

      void attach_on_submit_event(const std::function<void(std::string value)> on_submit = nullptr) {
          if (this->keyboard == nullptr) return;

          this->on_submit_callback = on_submit;

          this->lambda = [](lv_event_t *e) {
            lv_obj_t* kb = lv_event_get_target(e);
            auto* self = static_cast<KeyboardManager*>(lv_event_get_user_data(e)); // get `this`

            if (lv_event_get_code(e) == LV_EVENT_READY) {
                lv_obj_t* ta = lv_keyboard_get_textarea(kb);
                const char* text = lv_textarea_get_text(ta);

                if (self->on_submit_callback) {
                    self->on_submit_callback(std::string(text));
                }

                // lv_obj_del(kb);
            }
          };

          lv_obj_add_event_cb(this->keyboard, lambda, LV_EVENT_ALL, this); // pass `this` as user_data
        }

        void detach_on_submit_event(const std::function<void(std::string value)> on_submit = nullptr) {
          if (this->keyboard == nullptr) return;

          lv_obj_remove_event_cb(keyboard, this->lambda);

          this->lambda = nullptr;
        }

        bool is_visible() const {
            return keyboard && !lv_obj_has_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
        }

        lv_obj_t* get_keyboard() const {
            return keyboard;
        }
    };

}
