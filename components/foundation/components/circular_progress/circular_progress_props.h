#pragma once
#include "components/props.h"

namespace foundation
{
  class CircularProgress;
  struct CircularProgressProps final : BaseProps<CircularProgressProps, CircularProgress> {
    std::string label_symbol = "%";
    bool show_label_default = false;
    Delegate<void(Styling&)> text_style_override{};
    Delegate<void(Styling&)> arc_style_override{};
    short min_dy = 0;
    short max_dy = 100;
    double current_value = 0;
    short precision = 0;

    short width = 10;
    short height = 10;

    CircularProgressProps&& label(const std::string& s) {
      label_symbol = s;
      return std::move(*this);
    }

    CircularProgressProps&& show_label(bool v) {
      show_label_default = v;
      return std::move(*this);
    }

    CircularProgressProps&& min(short v) {
      min_dy = v;
      return std::move(*this);
    }



    CircularProgressProps&& max(short v) {
      max_dy = v;
      return std::move(*this);
    }

    CircularProgressProps&& value(const double v) {
      current_value = v;
      return std::move(*this);
    }

    CircularProgressProps&& decimals(int p) {
      precision = p;
      return std::move(*this);
    }

    CircularProgressProps&& w(short v) {
      width = v;
      return std::move(*this);
    }

    CircularProgressProps&& h(short v) {
      height = v;
      return std::move(*this);
    }

    CircularProgressProps&& set_text_style(Delegate<void(Styling&)> fn) {
      text_style_override = std::move(fn);
      return std::move(*this);
    }
    CircularProgressProps&& set_arc_style(Delegate<void(Styling&)> fn) {
      arc_style_override = std::move(fn);
      return std::move(*this);
    }
  };
}