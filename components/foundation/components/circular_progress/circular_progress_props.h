#pragma once
#include "components/props.h";

namespace foundation
{
  class CircularProgress;
  struct CircularProgressProps final : BaseProps<CircularProgressProps, CircularProgress> {
    std::string label_symbol = "%";
    bool show_label_default = false;
    short min_dy = 0;
    short max_dy = 100;
    short default_dy = 0;

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

    CircularProgressProps&& value(const short v) {
      default_dy = v;
      return std::move(*this);
    }

    CircularProgressProps&& value(const float v) {
      default_dy = v;
      return std::move(*this);
    }

    CircularProgressProps&& value(const int v) {
      default_dy = v;
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
  };
}