#pragma once

#include "components/props.h";

namespace foundation
{
  class Stepper;

  struct StepperProps final : BaseProps<StepperProps, Stepper>
  {
    Delegate<void(float), 64> on_change_cb{};


    float step = 1.0f;
    float min_value = 0.0f;
    float max_value = 100.0f;
    float initial_value = 0.0f;
    short precision = 0;

    short width = 150;
    short height = 40;
    short button_width = 40;
    short button_height = 40;

    std::string label = "";

    bool set_cursor_click_pos = false;
    uint8_t digit_count = 3;
    uint8_t separator_pos = 0;

    StepperProps& range(const float min, const float max) {
      this->min_value = min;
      this->max_value = max;
      return *this;
    }

    StepperProps& value(const float val) {
      this->initial_value = val;
      return *this;
    }

    StepperProps& set_step(float val) {
      this->step = val;
      return *this;
    }

    StepperProps& set_label(std::string val) {
      this->label = val;
      return *this;
    }

    StepperProps& on_change(Delegate<void(float), 64> cb) {
      this->on_change_cb = std::move(cb);
      return *this;
    }

    StepperProps& size(short w, short h) {
      this->width = w;
      this->height = h;
      return *this;
    }

    StepperProps& btn_width(short w) {
      this->button_width = w;
      return *this;
    }

    StepperProps& btn_height(short w) {
      this->button_height = w;
      return *this;
    }

    StepperProps& set_precision(short w) {
      this->precision = w;
      return *this;
    }

    StepperProps& format(uint8_t digits, uint8_t separator) {
      this->digit_count = digits;
      this->separator_pos = separator;
      return *this;
    }

    StepperProps& set_children(const std::vector<std::shared_ptr<VNode>>& value) {
      return *this;
    }

    StepperProps& set_height(lv_coord_t h) {
      this->height = h;
      return *this;
    }
  };
}