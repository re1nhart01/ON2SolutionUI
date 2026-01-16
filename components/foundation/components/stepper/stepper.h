#pragma once

#include "stepper_props.h"
#include "components/view/view.h"
#include "core/shortcuts.h"

namespace foundation
{
  class Stepper final : public Component<StepperProps> {
  public:
    using Component::props;
    lv_obj_t* btn_minus;
    lv_obj_t* btn_plus;
    lv_obj_t* spinbox;
    lv_obj_t* spinbox_label;

    explicit Stepper(StepperProps&& props) : Component(nullptr, nullptr, std::move(props)) {
      this->apply_reactive<Stepper>(this, this->props.reactive_delegates);

      if (this->props.ref != nullptr) {
          this->props.ref->set(this);
      }
    };

    ~Stepper() override
    {
      if (!this->props.reactive_link.empty())
      {
        this->detach_reactives<Stepper>(this, this->props.reactive_link);
      }
      if (this->props.ref != nullptr) {
          this->props.ref->unlink();
      }
    };

  lv_obj_t* render() override
  {
    lv_obj_t* parent_obj = this->get_parent();
    if (!parent_obj) return nullptr;

    lv_obj_t* obj = lv_obj_create(parent_obj);
    lv_obj_set_size(obj, props.width, props.height + 30);
    lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_bg_opa(obj, 0, 0);
    lv_obj_set_style_border_width(obj, 0, 0);
    lv_obj_set_style_pad_all(obj, 0, 0);
    lv_obj_set_style_pad_column(obj, 0, 0);

    if (props.label != "")
    {
      this->spinbox_label = lv_label_create(obj);
      lv_label_set_text(this->spinbox_label, props.label.c_str());
    }

    lv_obj_t* stepper_obj = lv_obj_create(obj);
    this->set_component(obj);

    lv_obj_set_size(stepper_obj, props.width, props.height);
    lv_obj_set_flex_flow(stepper_obj, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(stepper_obj, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_set_style_bg_opa(stepper_obj, 0, 0);
    lv_obj_set_style_border_width(stepper_obj, 0, 0);
    lv_obj_set_style_pad_all(stepper_obj, 0, 0);
    lv_obj_set_style_pad_column(stepper_obj, 5, 0);
    lv_obj_set_style_pad_all(obj, 0, 0);
    lv_obj_set_style_pad_column(obj, 5, 0);
    lv_obj_set_style_shadow_width(obj, 0, 0);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_add_flag(stepper_obj, LV_OBJ_FLAG_CLICKABLE);
    this->set_active(this->props.is_visible);

    this->btn_minus = lv_btn_create(stepper_obj);
    lv_obj_set_size(btn_minus, props.button_width, props.button_height);
    lv_obj_t* label_minus = lv_label_create(btn_minus);
    lv_label_set_text(label_minus, LV_SYMBOL_MINUS);
    lv_obj_center(label_minus);

    this->spinbox = lv_textarea_create(stepper_obj);
    lv_textarea_set_one_line(spinbox, true);
    update_text(this->spinbox, this->props.initial_value, this->props.precision);
    lv_obj_set_width(spinbox, props.width - (props.button_width * 2) - 10);
    lv_obj_set_style_text_align(spinbox, LV_TEXT_ALIGN_CENTER, 0);

    lv_obj_set_style_opa(this->spinbox, LV_OPA_0, LV_PART_CURSOR | LV_STATE_FOCUSED);
    lv_obj_set_style_opa(this->spinbox, LV_OPA_0, LV_PART_CURSOR | LV_STATE_DEFAULT);

    lv_obj_clear_flag(this->spinbox, LV_OBJ_FLAG_CLICKABLE);

    this->btn_plus = lv_btn_create(stepper_obj);
    lv_obj_set_size(btn_plus, props.button_width, props.button_height);
    lv_obj_t* label_plus = lv_label_create(btn_plus);
    lv_label_set_text(label_plus, LV_SYMBOL_PLUS);
    lv_obj_center(label_plus);

    lv_obj_add_event_cb(btn_minus, [](lv_event_t* e) {
     const auto self = static_cast<Stepper*>(lv_event_get_user_data(e));

     float current = std::stof(lv_textarea_get_text(self->spinbox));
     if (current - self->props.step >= self->props.min_value) {
         current -= self->props.step;
         self->update_text(self->spinbox, current, self->props.precision);

         if (self->props.on_change_cb) self->props.on_change_cb(current);
     }
    }, LV_EVENT_CLICKED, this);

    lv_obj_add_event_cb(btn_plus, [](lv_event_t* e) {
     const auto self = static_cast<Stepper*>(lv_event_get_user_data(e));

     float current = std::stof(lv_textarea_get_text(self->spinbox));
     if (current + self->props.step <= self->props.max_value) {
         current += self->props.step;
         self->update_text(self->spinbox, current, self->props.precision);

         if (self->props.on_change_cb) self->props.on_change_cb(current);
     }
    }, LV_EVENT_CLICKED, this);

    return obj;
  }

  void update_text(lv_obj_t* ta, float val, short precision) {
    char buf[32];
    if (precision <= 0) {
        snprintf(buf, sizeof(buf), "%d", (int)val);
    } else {
        char fmt[10];
        snprintf(fmt, sizeof(fmt), "%%.%df", precision);
        snprintf(buf, sizeof(buf), fmt, val);
    }
    lv_textarea_set_text(ta, buf);
  }

    const Styling* styling() const override
    {
      apply_base_style(style);

      if (props.style_override) {
          props.style_override(style);
      }

      return &style;
    }

    Stepper* append(lv_obj_t* obj) override
    {
      lv_obj_set_parent(obj, get_component());
      return this;
    };
  };
}