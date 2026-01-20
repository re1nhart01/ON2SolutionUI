#pragma once
#include "core/state/reactive.h"
#include "core/structures/delegate.h"
#include "vnode.h"

#include <internals/lvgl_port.h>

namespace foundation {
  template<typename Props>
  class Component : public virtual VNode {
  public:
    Props props;

    explicit Component(Props&& props) : VNode(nullptr, nullptr), props(std::move(props)) {}

    explicit Component(lv_obj_t* obj, lv_obj_t* parent, Props&& props) : VNode(obj, parent), props(std::move(props)) {}

    template<typename Fn>
    void set_state(Fn&& fn) {
        if (!this->component || !lv_obj_is_valid(this->component)) return;

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

        if (lvgl_port_lock(-1)) {
            lv_async_call(
                [](void* user_data) {
                    auto* ctx = static_cast<Delegate<void()>*>(user_data);
                    if (ctx) {
                        (*ctx)();
                        delete ctx;
                    }
                },
                ctx
            );
            lvgl_port_unlock();
        } else {
            delete ctx;
        }
    }
  };

}