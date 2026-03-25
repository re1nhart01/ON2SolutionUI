//
// Created by evgeniy on 11/23/25.
//
#pragma once
#include "components/props.h";

namespace foundation
{
  class ActivityIndicator;
  struct ActivityIndicatorProps final : BaseProps<ActivityIndicatorProps, ActivityIndicator>
  {
    lv_color_t color = lv_color_hex(0xFF0000);
    short spin_time = 1000;
    short arc_length = 60;
    short size = 40;

    ActivityIndicatorProps&& time(short t) {
      this->spin_time = t;
      return std::move(*this);
    }

    ActivityIndicatorProps&& arc(short a) {
      this->arc_length = a;
      return std::move(*this);
    }

    ActivityIndicatorProps&& sz(short s) {
      this->size = s;
      return std::move(*this);
    }

    ActivityIndicatorProps&& set_color(lv_color_t color)
    {
      this->color = color;
      return std::move(*this);
    }
  };
}