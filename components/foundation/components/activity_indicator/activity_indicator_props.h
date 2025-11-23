//
// Created by evgeniy on 11/23/25.
//
#pragma once

namespace foundation
{
  class ActivityIndicator;
  struct ActivityIndicatorProps final : BaseProps<ActivityIndicatorProps, ActivityIndicator>
  {
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

    static ActivityIndicatorProps up() { return ActivityIndicatorProps(); }
  };
}