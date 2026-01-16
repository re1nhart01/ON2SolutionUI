#pragma once

#include "core/ref/ref.h"
#include "core/state/reactive.h"
#include "core/structures/delegate.h"
#include "core/styling/styling.h"
#include <core/state/thread_reactive.h>
#include <memory>

namespace foundation
{
  template <typename Derived, typename RefT>
  struct BaseProps
  {
    std::shared_ptr<Ref<RefT>> ref = nullptr;
    Delegate<void(Styling&)> style_override{};
    mutable std::vector<Delegate<void(void*), 40>> reactive_delegates{};
    std::vector<IReactive*> reactive_link{};

    bool is_visible = true;

    static Derived up(){ return Derived{}; }


    template<typename TVal>
    Derived&& watch(Reactive<TVal>* reactive, std::string key, Delegate<void(RefT*, const TVal&)> updater)
    {
      if (!reactive) return std::move(static_cast<Derived&>(*this));

      this->reactive_delegates.push_back([reactive, updater, key](void* internal_component) {
        reactive->attach(key, static_cast<RefT*>(internal_component), updater);
      });

      this->reactive_link.push_back(reactive);

      return std::move(static_cast<Derived&>(*this));
    }

    template<typename TVal>
    Derived&& watch(ThreadReactive<TVal> * reactive, std::string key, Delegate<void(RefT*, const TVal&)> updater)
    {
      if (!reactive) return std::move(static_cast<Derived&>(*this));

      this->reactive_delegates.push_back([reactive, updater, key](void* internal_component) {
        reactive->attach(key, static_cast<RefT*>(internal_component), updater);
      });

      this->reactive_link.push_back(reactive);

      return std::move(static_cast<Derived&>(*this));
    }

    Derived&& set_style(Delegate<void(Styling&)> fn)
    {
      style_override = std::move(fn);
      return std::move(static_cast<Derived&>(*this));
    }

    Derived&& set_ref(const std::shared_ptr<Ref<RefT>>& r) {
      ref = r;
      return std::move(static_cast<Derived&>(*this));
    }

    Derived&& set_visible(const bool value)
    {
      this->is_visible = value;
      return std::move(static_cast<Derived&>(*this));
    }

    template <typename Fn>
    Derived&& merge(Fn fn) {
      Derived& updated = fn(static_cast<Derived&>(*this));
      return std::move(updated);
    }
  };
}