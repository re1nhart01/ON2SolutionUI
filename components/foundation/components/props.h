#pragma once

#include "core/ref/ref.h"
#include "core/styling/styling.h"

#include <memory>

namespace foundation
{
  template <typename Derived>
  struct BaseProps
  {
    std::shared_ptr<Ref> ref = nullptr;
    std::shared_ptr<Styling> style;

    virtual ~BaseProps() = default;

    Derived& set_style(const std::shared_ptr<Styling> & s) { style = s; return static_cast<Derived&>(*this); }
    Derived& set_ref(const std::shared_ptr<Ref>& s) { ref = s; return static_cast<Derived&>(*this); }

    template <typename Fn>
    Derived& merge(Fn fn) {
      fn(*this);
      return static_cast<Derived&>(*this);
    }
  };
}