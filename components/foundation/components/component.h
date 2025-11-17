#pragma once
#include "vnode.h"

namespace foundation {

  template<typename Props>
  class Component : public VNode {
  public:
    Props props;

    Component() : VNode() {}

    explicit Component(Props props)
      : VNode(nullptr, nullptr), props(std::move(props))
    {
      // if (this->props.ref != nullptr)
      //   this->props.ref->set(this);
    }

    Component(lv_obj_t* obj, lv_obj_t* parent, Props props)
      : VNode(obj, parent), props(std::move(props))
    {
      // if (this->props.ref != nullptr)
        // this->props.ref->set(this);
    }

    // setState() with props mutation
    template<typename Fn>
    void set_state(Fn&& fn) {
      fn(this->props);
      this->forceUpdate();
    }
  };

}
