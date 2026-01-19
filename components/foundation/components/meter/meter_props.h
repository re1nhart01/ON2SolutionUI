#pragma once
#include "components/props.h"

namespace foundation
{
  class Meter;
  struct MeterProps final : BaseProps<MeterProps, Meter> {
    std::string label_symbol = "%";
    bool show_label_default = false;
    int min_v = 0;
    int max_v = 100;
    float current_v = 0;

    short width = 200;
    short height = 200;

    MeterProps&& label(const std::string& s) {
      label_symbol = s;
      return std::move(*this);
    }

    MeterProps&& show_label(bool v) {
      show_label_default = v;
      return std::move(*this);
    }

    MeterProps&& min(int v) {
      min_v = v;
      return std::move(*this);
    }

    MeterProps&& max(int v) {
      max_v = v;
      return std::move(*this);
    }

    MeterProps&& value(float v) {
      current_v = v;
      return std::move(*this);
    }

    MeterProps&& w(short v) {
      width = v;
      return std::move(*this);
    }

    MeterProps&& h(short v) {
      height = v;
      return std::move(*this);
    }
  };
}