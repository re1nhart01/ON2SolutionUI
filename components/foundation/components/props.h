#pragma once

#include <core/state/thread_reactive.h>
#include <memory>
#include <utility>
#include "core/ref/ref.h"
#include "core/state/reactive.h"
#include "core/structures/delegate.h"
#include "core/styling/styling.h"

namespace foundation {
  template <typename Derived, typename RefT>
  struct BaseProps {
    std::shared_ptr<Ref<RefT>> ref = nullptr;
    Delegate<void(Styling&)> style_override{};
    Delegate<void(lv_obj_t*)> ref_control{};
    mutable std::vector<Delegate<void(void*), 40>> reactive_delegates{};
    std::vector<IReactive*> reactive_link{};

    bool is_visible = true;

    static Derived up() { return Derived{}; }

    template <typename TVal>
    Derived&& watch(Reactive<TVal>* reactive, const std::string& key,
                    Delegate<void(RefT*, const TVal&)> updater) {
      if (!reactive)
        return std::move(static_cast<Derived&>(*this));

      this->reactive_delegates.push_back([reactive, updater,
                                          key](void* internal_component) {
        reactive->attach(key, static_cast<RefT*>(internal_component), updater);
      });

      this->reactive_link.push_back(reactive);

      return std::move(static_cast<Derived&>(*this));
    }

    template <typename TVal>
    Derived&& watch(ThreadReactive<TVal>* reactive, const std::string& key,
                    Delegate<void(RefT*, const TVal&)> updater) {
      if (!reactive)
        return std::move(static_cast<Derived&>(*this));

      this->reactive_delegates.push_back(
          [reactive, updater, key](void* internal_component) {
            auto* comp = static_cast<RefT*>(internal_component);

            Delegate<void(RefT*, std::shared_ptr<const TVal>), 64> adapter =
                [updater](RefT* c, std::shared_ptr<const TVal> val) {
                  if (c && updater && val) {
                    updater(c, *val);
                  }
                };

            reactive->attach(key, comp, adapter);
          });

      this->reactive_link.push_back(reactive);

      return std::move(static_cast<Derived&>(*this));
    }

    Derived&& set_style(Delegate<void(Styling&)> fn) {
      style_override = std::move(fn);
      return std::move(static_cast<Derived&>(*this));
    }

    Derived&& set_ref(const std::shared_ptr<Ref<RefT>>& r) {
      ref = r;
      return std::move(static_cast<Derived&>(*this));
    }

    Derived&& control_ref(Delegate<void(lv_obj_t*)> controller) {
      this->ref_control = std::move(controller);
      return std::move(static_cast<Derived&>(*this));
    }

    Derived&& set_visible(const bool value) {
      this->is_visible = value;
      return std::move(static_cast<Derived&>(*this));
    }

    template <typename Fn>
    Derived&& merge(Fn fn) {
      Derived& updated = fn(static_cast<Derived&>(*this));
      return std::move(updated);
    }
  };
}  // namespace foundation