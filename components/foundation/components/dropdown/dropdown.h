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

      lv_dropdown_clear_options(obj);

      for (auto option : this->props.options) {
        if (option != nullptr)
        lv_dropdown_add_option(obj, option, LV_DROPDOWN_POS_LAST);
      }

      if (const auto style = this->styling()) {
        lv_obj_add_style(obj, style->getStyle(), LV_PART_MAIN);
      }

      lv_dropdown_set_selected(obj, props.selected);

      lv_obj_add_event_cb(obj, event_adapter, LV_EVENT_ALL, this);

      return obj;
    };

    void do_rebuild() override {
      lv_obj_t* obj = this->get_component();
      if (!obj) return;
      
      if (props.style_override) {
        props.style_override(style);
        lv_obj_refresh_style(obj, LV_PART_MAIN, LV_STYLE_PROP_ANY);
      }

      lv_dropdown_set_selected(obj, props.selected);
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
            char buf[64]{0};
            lv_dropdown_get_selected_str(instance->get_component(), buf, sizeof(buf));
            instance->props.on_change(std::string(buf));
          };
          break;
        default:
          break;
      }
    }
  };
}  // namespace foundation