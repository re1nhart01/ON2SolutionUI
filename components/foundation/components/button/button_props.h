#pragma once
#include "components/props.h"
#include "components/vnode.h"
#include "core/lv_event.h"
#include "core/structures/delegate.h"
#include "core/styling/styling.h"

#include <functional>
#include <memory>
#include "components/props.h";

namespace foundation
{
  class Button;
  struct ButtonProps final : BaseProps<ButtonProps, Button>
  {
    std::unique_ptr<VNode> child = nullptr;
    std::string text;


    Delegate<void(lv_event_t *)> on_click{};
    Delegate<void(lv_event_t *)> on_long_press{};
    Delegate<void(lv_event_t *)> on_pressed{};
    Delegate<void(lv_event_t *)> on_released{};
    Delegate<void(lv_event_t *)> on_focused{};
    Delegate<void(lv_event_t *)> on_defocused{};

    ButtonProps&& set_child(std::unique_ptr<VNode> c) {
      child = std::move(c);
      return std::move(*this);
    }

    ButtonProps&& label(const std::string& t) {
      text = t;
      return std::move(*this);
    }

    ButtonProps&& click(auto fn) {
      on_click = fn;
      return std::move(*this);
    }

    ButtonProps&& long_press(auto fn) {
      on_long_press = fn;
      return std::move(*this);
    }

    ButtonProps&& pressed(auto fn) {
      on_pressed = fn;
      return std::move(*this);
    }

    ButtonProps&& released(auto fn) {
      on_released = fn;
      return std::move(*this);
    }

    ButtonProps&& focused(auto fn) {
      on_focused = fn;
      return std::move(*this);
    }

    ButtonProps&& defocused(auto fn) {
      on_defocused = fn;
      return std::move(*this);
    }

  };
}