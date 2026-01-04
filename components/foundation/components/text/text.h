#pragma once
#include "text_props.h"

namespace foundation
{
  class Text final : public Component<TextProps> {
  public:
    explicit Text(const TextProps& props) : Component(props) {
      this->apply_reactive<Text>(this, props.reactive_delegates);

      for (auto& binder : props.reactive_delegates) {
          if (binder) binder(this);
      }
      if (this->props.ref != nullptr) {
          this->props.ref->set(this);
      }
    };

    ~Text() override
    {
      if (this->props.ref != nullptr) {
        this->props.ref->unlink();
      }
      if (!this->props.reactive_link.empty())
      {
        this->detach_reactives<Text>(this, this->props.reactive_link);
      }
    };

    lv_obj_t* render() override
    {
      Component::render();

      lv_obj_t* parent_obj = this->get_parent();
      if (!parent_obj) return nullptr;

      set_component(lv_label_create(parent_obj));
      auto* obj = get_component();

      lv_label_set_text(obj, props.text.c_str());

      const auto& e = props;
      if (e.on_click)       lv_obj_add_event_cb(obj, e.on_click, LV_EVENT_CLICKED, nullptr);
      if (e.on_long_press)  lv_obj_add_event_cb(obj, e.on_long_press, LV_EVENT_LONG_PRESSED, nullptr);
      if (e.on_pressed)     lv_obj_add_event_cb(obj, e.on_pressed, LV_EVENT_PRESSED, nullptr);
      if (e.on_released)    lv_obj_add_event_cb(obj, e.on_released, LV_EVENT_RELEASED, nullptr);
      if (e.on_focused)     lv_obj_add_event_cb(obj, e.on_focused, LV_EVENT_FOCUSED, nullptr);
      if (e.on_defocused)   lv_obj_add_event_cb(obj, e.on_defocused, LV_EVENT_DEFOCUSED, nullptr);

      return obj;
    };

    void do_rebuild() override
    {
      lv_obj_t* obj = this->get_component();
      if (!obj) return;

      this->set_active(this->props.is_visible);

      lv_obj_update_layout(obj);
      lv_label_set_text(obj, this->props.text.c_str());
    };

    const Styling* styling() const override
    {
      style.reset();

      apply_base_style(style);

      if (props.style_override) {
          props.style_override(style);
      }

      return &style;
    }

    Text* append(lv_obj_t* obj) override
    {
      lv_obj_set_parent(obj, get_component());
      return this;
    };

    void update(const std::string &value) const
    {
      lv_obj_t* obj = this->get_component();
      if (obj != nullptr) {
          lv_label_set_text(this->get_component(), value.c_str());
      }
    };
  };
}
