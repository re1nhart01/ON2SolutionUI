#pragma once

#include "core/structures/delegate.h"
#include "core/structures/ui_queue.h"
#include "vnode.h"

namespace foundation {

  template <typename Props>
  class Component : public virtual VNode {
  public:
    Props props;

    explicit Component(Props&& props)
        : VNode(nullptr, nullptr), props(std::move(props)) {}

    explicit Component(lv_obj_t* obj, lv_obj_t* parent, Props&& props)
        : VNode(obj, parent), props(std::move(props)) {}

  private:
    template <typename Fn>
    void apply_state(Fn&& fn) {
      if (!this->component || !lv_obj_is_valid(this->component))
        return;

      if constexpr (std::is_invocable_v<Fn, Props&>) {
        fn(this->props);
      } else if constexpr (
          std::is_invocable_v<Fn, Props&, Component<Props>*>)
      {
        fn(this->props, this);
      }

      this->forceUpdate();
    }

  public:
    template <typename Fn>
    void set_state_async(Fn&& fn) {
      if (!this->component || !lv_obj_is_valid(this->component))
        return;

      UIQueue::get_instance().push(
          [this, fn = std::forward<Fn>(fn)]() mutable {
            if (!this->component ||
                !lv_obj_is_valid(this->component)) {
              ESP_LOGW(
                  "UI",
                  "Attempted to update a dead component!"
              );
              return;
            }

            this->apply_state(std::move(fn));
          });
    }

    template <typename Fn>
    void set_state(Fn&& fn) {
      apply_state(std::forward<Fn>(fn));
    }
  };

}  // namespace foundation