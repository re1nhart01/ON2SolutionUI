#pragma once

#include "components/view/view.h"
#include "core/shortcuts.h"
#include "stepper_props.h"

namespace foundation {
  class Stepper final : public Component<StepperProps> {
   public:
    using Component::props;
    lv_obj_t* btn_minus;
    lv_obj_t* btn_plus;
    lv_obj_t* spinbox;
    lv_obj_t* text;

    mutable Styling spinbox_style{};
    mutable Styling btn_style{};
    mutable Styling text_style{};
    mutable Styling text_wrapper_style{};

    explicit Stepper(StepperProps&& props)
        : Component(nullptr, nullptr, std::move(props)) {
      this->apply_reactive<Stepper>(this, this->props.reactive_delegates);

      if (this->props.ref != nullptr) {
        this->props.ref->set(this);
      }
    };

    ~Stepper() override {
      if (!this->props.reactive_link.empty()) {
        this->detach_reactives<Stepper>(this, this->props.reactive_link);
      }
      if (this->props.ref != nullptr) {
        this->props.ref->unlink();
      }
    };

    lv_obj_t* render() override {
      lv_obj_t* parent_obj = this->get_parent();
      if (!parent_obj)
        return nullptr;

      lv_obj_t* obj = lv_obj_create(parent_obj);
      this->set_component(obj);

      const auto style = this->styling();

      lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
      lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_ROW);
      lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER,
                            LV_FLEX_ALIGN_CENTER);

      lv_obj_set_style_bg_opa(obj, 0, 0);
      lv_obj_set_style_border_width(obj, 0, 0);
      lv_obj_set_style_pad_all(obj, 0, 0);
      lv_obj_set_style_pad_column(obj, 4, 0);
      lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);

      this->set_active(this->props.is_visible);

      if (style != nullptr) {
        lv_obj_add_style(obj, style->getStyle(), LV_PART_MAIN);
      }

      this->btn_minus = lv_btn_create(obj);
      lv_obj_set_size(btn_minus, props.button_width, props.button_height);
      lv_obj_t* label_minus = lv_label_create(btn_minus);
      lv_label_set_text(label_minus, LV_SYMBOL_MINUS);
      lv_obj_set_style_pad_all(btn_minus, 0, 0);
      lv_obj_center(label_minus);

      if (props.buttons_style_override) {
        props.buttons_style_override(this->btn_style);
      }

      if (props.spinbox_style_override) {
        props.spinbox_style_override(this->spinbox_style);
      }

      if (props.text_style_override) {
        props.text_style_override(this->text_style);
      }

      this->spinbox = lv_obj_create(obj);
      lv_obj_set_height(spinbox, props.height);
      lv_obj_set_width(spinbox, props.width);


      lv_obj_set_style_opa(this->spinbox, LV_OPA_0,
                           LV_PART_CURSOR | LV_STATE_FOCUSED);

      lv_obj_set_flex_flow(spinbox, LV_FLEX_FLOW_ROW);
      lv_obj_set_flex_align(spinbox, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER,
                            LV_FLEX_ALIGN_CENTER);

      text = lv_label_create(spinbox);

      lv_obj_add_style(this->spinbox, this->spinbox_style.getStyle(), LV_PART_MAIN);
      lv_obj_add_style(text, this->text_style.getStyle(), LV_PART_MAIN);

      update_text(this->props.initial_value, this->props.precision);

      this->btn_plus = lv_btn_create(obj);
      lv_obj_set_size(btn_plus, props.button_width, props.button_height);
      lv_obj_t* label_plus = lv_label_create(btn_plus);
      lv_label_set_text(label_plus, LV_SYMBOL_PLUS);
      lv_obj_set_style_pad_all(btn_plus, 0, 0);
      lv_obj_center(label_plus);

      lv_obj_add_style(this->btn_plus, this->btn_style.getStyle(), LV_PART_MAIN);
      lv_obj_add_style(this->btn_minus, this->btn_style.getStyle(), LV_PART_MAIN);

      lv_obj_add_event_cb(
          btn_minus,
          [](lv_event_t* e) {
            const auto self = static_cast<Stepper*>(lv_event_get_user_data(e));

            float current = std::stof(lv_label_get_text(self->text));
            if (current - self->props.step >= self->props.min_value) {
              current -= self->props.step;
              self->update_text(current, self->props.precision);

              if (self->props.on_change_cb)
                self->props.on_change_cb(current);
            }
          },
          LV_EVENT_CLICKED, this);

      lv_obj_add_event_cb(
          btn_plus,
          [](lv_event_t* e) {
            const auto self = static_cast<Stepper*>(lv_event_get_user_data(e));

            float current = std::stof(lv_label_get_text(self->text));
            if (current + self->props.step <= self->props.max_value) {
              current += self->props.step;
              self->update_text(current, self->props.precision);

              if (self->props.on_change_cb)
                self->props.on_change_cb(current);
            }
          },
          LV_EVENT_CLICKED, this);

      return obj;
    }

    void do_rebuild() override {
      lv_obj_t* obj = this->get_component();
      if (!obj) return;

      if (props.style_override) {
        props.style_override(style);
        lv_obj_refresh_style(obj, LV_PART_MAIN, LV_STYLE_PROP_ANY);
      }
    };

    void update_text(float val, short precision) {
      char buf[32];
      if (precision <= 0) {
        snprintf(buf, sizeof(buf), "%d", (int)val);
      } else {
        char fmt[10];
        snprintf(fmt, sizeof(fmt), "%%.%df", precision);
        snprintf(buf, sizeof(buf), fmt, val);
      }

      lv_label_set_text(this->text, buf);
    }

    const Styling* styling() const override {
      apply_base_style(style);

      if (props.style_override) {
        props.style_override(style);
      }

      return &style;
    }

    Stepper* append(lv_obj_t* obj) override {
      lv_obj_set_parent(obj, get_component());
      return this;
    };
  };
}  // namespace foundation