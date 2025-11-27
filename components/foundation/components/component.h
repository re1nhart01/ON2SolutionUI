#pragma once
#include "core/structures/delegate.h"
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

      auto* ctx = new Delegate<void()>(
          [this, fn = std::forward<Fn>(fn)]() mutable {

              if constexpr (std::is_invocable_v<Fn, Props&>) {
                  fn(this->props);

              } else if constexpr (std::is_invocable_v<Fn, Props&, Component<Props>*>) {
                  fn(this->props, this);
              }

              this->forceUpdate();
          }
      );

      lv_async_call(
          [](void* user_data) {
              auto* ctx = static_cast<Delegate<void()>*>(user_data);
              (*ctx)();
              delete ctx;
          },
          ctx
      );
    }

  };

}