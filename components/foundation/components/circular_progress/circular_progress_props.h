#pragma once

namespace foundation
{
  struct CircularProgressProps final : BaseProps<CircularProgressProps> {
    std::string label_symbol = "%";
    bool show_label_default = false;
    short min_dy = 0;
    short max_dy = 100;
    short default_dy = 0;

    short w = 10;
    short h = 10;

    static CircularProgressProps up() { return CircularProgressProps{}; }

    CircularProgressProps& set_ref(std::shared_ptr<Ref> r) {
      ref = std::move(r);
      return *this;
    }

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

    CircularProgressProps& width(short v) {
      w = v;
      return *this;
    }

    CircularProgressProps& height(short v) {
      h = v;
      return *this;
    }
  };
}