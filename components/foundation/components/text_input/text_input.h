
#pragma once
#include "core/keyboard_manager/keyboard_manager.h"
#include "text_input_props.h"

namespace foundation
{
  class TextInput final : public Component<TextInputProps> {
  private:
    mutable lv_obj_t* keyboard = nullptr;
    KeyboardManager* kbManager = nullptr;

  public:
    using Component::props;

    explicit TextInput(const TextInputProps& props, KeyboardManager* kbManager) : Component(nullptr, nullptr, std::move(props)) {
      this->props = props;
      this->kbManager = kbManager;

      if (this->props.on_submit != nullptr && this->kbManager != nullptr) {
          this->kbManager->attach_on_submit_event(this->props.on_submit);
      }

      if (this->props.ref != nullptr) {
          this->props.ref->set(this);
      }
    };

    ~TextInput() override = default;

    lv_obj_t* render() override
    {
      lv_obj_t* parent = this->get_parent();
      if (!parent) return nullptr;

      set_component(lv_textarea_create(parent));
      lv_obj_t* obj = this->get_component();

      if (kbManager != nullptr)
        {
          lv_obj_add_event_cb(obj, [](lv_event_t* e) {
            auto* instance = static_cast<TextInput*>(lv_event_get_user_data(e));
            if (!instance || !instance->kbManager) return;

            instance->kbManager->create(lv_scr_act());

            if (instance->props.on_submit != nullptr) {
              instance->kbManager->attach_on_submit_event(instance->props.on_submit);
            }

            instance->kbManager->attach(lv_event_get_target(e));
          }, LV_EVENT_FOCUSED, this);

          lv_obj_add_event_cb(obj, [](lv_event_t* e) {
            auto* instance = static_cast<TextInput*>(lv_event_get_user_data(e));
            if (!instance || !instance->kbManager) return;
            instance->kbManager->hide();
          }, LV_EVENT_DEFOCUSED, this);
        }

      do_rebuild();

      return obj;
    };

    void do_rebuild() override
    {
      lv_obj_t* obj = this->get_component();
      if (!obj) return;

      lv_obj_update_layout(obj);

      lv_textarea_set_one_line(obj, props.is_one_line);
      lv_textarea_set_placeholder_text(obj, props.placeholder);
      lv_textarea_set_password_mode(obj, props.is_secure);
      lv_textarea_set_password_show_time(obj, props.secure_timeout);
      lv_textarea_set_max_length(obj, props.length);

      if (props.text != nullptr) {
          lv_textarea_set_text(obj, props.text);
      }

      if (props.style) {
          props.style->applyTo(obj);
      }

      lv_obj_remove_event_cb(obj, nullptr);

      if (props.on_click)          lv_obj_add_event_cb(obj, props.on_click, LV_EVENT_CLICKED, nullptr);
      if (props.on_focused)        lv_obj_add_event_cb(obj, props.on_focused, LV_EVENT_FOCUSED, nullptr);
      if (props.on_defocused)      lv_obj_add_event_cb(obj, props.on_defocused, LV_EVENT_DEFOCUSED, nullptr);
      if (props.on_value_changed)  lv_obj_add_event_cb(obj, props.on_value_changed, LV_EVENT_VALUE_CHANGED, nullptr);
    }

    std::shared_ptr<Styling> styling() override
    {
      if (this->props.style) {
          return this->props.style;
      }
      return {};
    };

    TextInput* append(lv_obj_t* obj) override
    {
      lv_obj_set_parent(obj, get_component());
      return this;
    };

    const char* get_text() const
    {
      if (this->get_component() == nullptr) return "";
      return lv_textarea_get_text(get_component());
    };

    void set_text(const char* txt) const
    {
      if (this->get_component() == nullptr) return;
      lv_textarea_set_text(get_component(), txt);
    };
  };
}