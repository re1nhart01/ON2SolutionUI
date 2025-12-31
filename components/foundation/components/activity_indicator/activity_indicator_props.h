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
    int spin_time = 1000;
    int arc_length = 60;
    int size = 40;

    ActivityIndicatorProps& time(int t) {
      this->spin_time = t;
      return *this;
    }

    ActivityIndicatorProps& arc(int a) {
      this->arc_length = a;
      return *this;
    }

    ActivityIndicatorProps& sz(int s) {
      this->size = s;
      return *this;
    }

    ActivityIndicatorProps& set_color(lv_color_t color)
    {
      this->color = color;
      return *this;
    }
  };
}