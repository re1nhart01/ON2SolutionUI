#pragma once

#include "core/ref/ref.h"
#include "core/styling/styling.h"

#include <memory>

namespace foundation
{
  template <typename Derived, typename RefT>
  struct BaseProps
  {
    std::shared_ptr<Ref<RefT>> ref = nullptr;
    std::shared_ptr<Styling> style = nullptr;

    bool is_visible = true;

    virtual ~BaseProps() = default;

    Derived& set_style(const std::shared_ptr<Styling> & s) {
      style = s;
      return static_cast<Derived&>(*this);
    }

    Derived& set_ref(const std::shared_ptr<Ref<RefT>>& r) {
      ref = r;
      return static_cast<Derived&>(*this);
    }

    Derived* set_visible(const bool value)
    {
      this->is_visible = value;
      return static_cast<Derived&>(*this);
    }

    template <typename Fn>
    Derived& merge(Fn fn) {
      fn(static_cast<Derived&>(*this));
      return static_cast<Derived&>(*this);
    }
  };
}