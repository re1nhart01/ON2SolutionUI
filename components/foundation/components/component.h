#pragma once
#include "vnode.h"

namespace foundation {
  template<typename Props>
  class Component : public virtual VNode {
  public:
    Props props;

    explicit Component(const Props& props) : VNode(nullptr, nullptr), props(props) {}

    explicit Component(lv_obj_t* obj, lv_obj_t* parent, const Props& props) : VNode(obj, parent), props(props) {}

    template<typename Fn>
    void set_state(Fn&& fn) {
      if (!this->component) return;
      if constexpr (std::is_invocable_v<Fn, Props&>) {
          fn(this->props);

      } else if constexpr (std::is_invocable_v<Fn, Props&, Component*>) {
          fn(this->props, this);

      } else {
          static_assert(false, "Invalid set_state callback signature");
      }

      this->forceUpdate();
    }
  };

}