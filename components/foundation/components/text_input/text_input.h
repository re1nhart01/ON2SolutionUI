
#pragma once
#include "core/keyboard_manager/keyboard_manager.h"
#include "text_input_props.h"

namespace foundation
{
  class TextInput final : public Component<TextInputProps> {
  public:
    using Component::props;
    bool is_init = true;
    explicit TextInput(const TextInputProps& props) : Component(nullptr, nullptr, std::move(props)) {
      this->apply_reactive<TextInput>(this, props.reactive_delegates);
      if (this->props.ref != nullptr) {
          this->props.ref->set(this);
      }
    };

    ~TextInput() override
    {
      if (this->props.ref != nullptr) {
          this->props.ref->unlink();
      }
      if (!this->props.reactive_link.empty())
      {
        this->detach_reactives<TextInput>(this, this->props.reactive_link);
      }
    };

    lv_obj_t* render() override
    {
      lv_obj_t* parent = this->get_parent();
      if (!parent) return nullptr;

      set_component(lv_textarea_create(parent));
      lv_obj_t* obj = this->get_component();

      do_rebuild();

      return obj;
    };

    void do_rebuild() override
    {
      lv_obj_t* obj = this->get_component();
      if (!obj) return;
      
      this->set_active(this->props.is_visible);
      lv_obj_update_layout(obj);

      lv_textarea_set_one_line(obj, props.is_one_line);
      lv_textarea_set_placeholder_text(obj, props.placeholder);
      lv_textarea_set_password_mode(obj, props.is_secure);
      lv_textarea_set_password_show_time(obj, props.secure_timeout);
      lv_textarea_set_max_length(obj, props.length);

      if (props.text != nullptr) {
          lv_textarea_set_text(obj, props.text);
      }

      lv_obj_remove_event_cb(obj, nullptr);

      if (this->props.kbManager != nullptr)
        {
          lv_obj_add_event_cb(obj, [](lv_event_t* e) {
            auto* instance = static_cast<TextInput*>(lv_event_get_user_data(e));
            if (!instance) return;

            instance->props.kbManager->create(lv_scr_act());

            if (instance->props.on_submit) {
              instance->props.kbManager->attach_on_submit_event(instance->props.on_submit);
            }

            if (instance->get_component() != nullptr)
              {
                instance->props.kbManager->attach(instance->get_component());
              }
          }, LV_EVENT_FOCUSED, this);

          lv_obj_add_event_cb(obj, [](lv_event_t* e) {
            auto* instance = static_cast<TextInput*>(lv_event_get_user_data(e));
            if (!instance) return;
            instance->props.kbManager->hide();
          }, LV_EVENT_DEFOCUSED, this);
        }

      if (props.on_click)          lv_obj_add_event_cb(obj, event_adapter, LV_EVENT_CLICKED, this);
      if (props.on_focused)        lv_obj_add_event_cb(obj, event_adapter, LV_EVENT_FOCUSED, this);
      if (props.on_defocused)      lv_obj_add_event_cb(obj, event_adapter, LV_EVENT_DEFOCUSED, this);
      if (props.on_value_changed)  lv_obj_add_event_cb(obj, event_adapter, LV_EVENT_VALUE_CHANGED, this);
    }

    const Styling* styling() const override
    {
      style.reset();

      apply_base_style(style);

      if (props.style_override) {
          props.style_override(style);
      }

      return &style;
    }

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

    static void event_adapter(lv_event_t *event) {
      auto *instance = static_cast<TextInput *>(lv_event_get_user_data(event));
      if (!instance) return;

      auto &events = instance->props;

      switch (event->code) {
        case LV_EVENT_CLICKED:
          if (events.on_click) events.on_click(event);
          break;
        case LV_EVENT_FOCUSED:
          if (events.on_focused) events.on_focused(event);
          break;
        case LV_EVENT_DEFOCUSED:
          if (events.on_defocused) events.on_defocused(event);
          break;
        case LV_EVENT_VALUE_CHANGED:
          if (events.on_value_changed)
            {
              lv_obj_t* ta = lv_event_get_target(event);
              const char* txt = lv_textarea_get_text(ta);
              events.on_value_changed(std::string(txt));
            };
          break;
        default:
          ESP_LOGW("button", "Unknown event code %d", event->code);
      }
    }
  };
}