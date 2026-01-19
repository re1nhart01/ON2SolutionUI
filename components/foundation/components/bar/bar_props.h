
#pragma once
#include "components/props.h";

namespace foundation
{
  class Bar;
  struct BarProps final : BaseProps<BarProps, Bar>
  {
    short value;
    std::string text;
    short width = 200;
    short height = 70;
    bool reversed = false;

    BarProps&& set_value(short val) {
      value = val;
      return std::move(*this);
    }

    BarProps&& label(const std::string& t) {
      text = t;
      return std::move(*this);
    }

    BarProps&& w(short value)
    {
      width = value;
      return std::move(*this);
    }

    BarProps&& h(short value)
    {
      height = value;
      return std::move(*this);
    }

    BarProps&& set_reversed(bool value)
    {
      reversed = value;
      return std::move(*this);
    }
  };
}