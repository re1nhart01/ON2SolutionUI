#pragma once

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

    static CircularProgressProps up() { return CircularProgressProps{}; }

    CircularProgressProps& set_style(std::shared_ptr<Styling> s) {
      style = std::move(s);
      return *this;
    }

    CircularProgressProps& label(const std::string& s) {
      label_symbol = s;
      return *this;
    }

    CircularProgressProps& show_label(bool v) {
      show_label_default = v;
      return *this;
    }

    CircularProgressProps& min(short v) {
      min_dy = v;
      return *this;
    }

    CircularProgressProps& max(short v) {
      max_dy = v;
      return *this;
    }

    CircularProgressProps& value(short v) {
      default_dy = v;
      return *this;
    }

    CircularProgressProps& w(short v) {
      width = v;
      return *this;
    }

    CircularProgressProps& h(short v) {
      height = v;
      return *this;
    }
  };
}