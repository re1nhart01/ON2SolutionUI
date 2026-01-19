#pragma once

#include "core/styling/styling.h"
#include "dropdown_props.h"

namespace foundation {
  class Dropdown final : public Component<DropdownProps> {
    using Component::props;

   public:
    explicit Dropdown(DropdownProps&& props)
        : Component(nullptr, nullptr, std::move(props)) {
      this->apply_reactive<Dropdown>(this, this->props.reactive_delegates);
    };
    ~Dropdown() override {
      if (this->props.ref != nullptr) {
        this->props.ref->unlink();
      }
      if (!this->props.reactive_link.empty()) {
        this->detach_reactives<Dropdown>(this, this->props.reactive_link);
      }
    };

    lv_obj_t* render() override {
      Component::render();

      lv_obj_t* parent_obj = this->get_parent();
      if (!parent_obj)
        return nullptr;

      set_component(lv_dropdown_create(parent_obj));
      auto* obj = get_component();


      for (auto option : this->props.options) {
        if (option != nullptr)
        lv_dropdown_add_option(obj, option, LV_DROPDOWN_POS_LAST);
      }

      lv_obj_add_event_cb(obj, event_adapter, LV_EVENT_ALL, this);

      return obj;
    };

    const Styling* styling() const override {
      apply_base_style(style);

      if (props.style_override) {
        props.style_override(style);
      }

      return &style;
    }

    Dropdown* append(lv_obj_t* obj) override {
      lv_obj_set_parent(obj, get_component());
      return this;
    };

    static void event_adapter(lv_event_t* event) {
      auto* instance = static_cast<Dropdown*>(lv_event_get_user_data(event));
      if (!instance)
        return;

      auto& events = instance->props;

      switch (event->code) {
        case LV_EVENT_VALUE_CHANGED:
          if (events.on_change) {
            char buf[32]{0};
            lv_dropdown_get_selected_str(instance->get_component(), buf, sizeof(buf));
            instance->props.on_change(buf);
          };
          break;
        default:
          ESP_LOGW("button", "Unknown event code %d", event->code);
      }
    }
  };
}  // namespace foundation