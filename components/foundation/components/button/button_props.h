#pragma once
#include "components/props.h"
#include "components/vnode.h"
#include "core/lv_event.h"
#include "core/structures/delegate.h"
#include "core/styling/styling.h"

#include <functional>
#include <memory>

namespace foundation
{
  class Button;
  struct ButtonProps final : BaseProps<ButtonProps, Button>
  {
    std::shared_ptr<VNode> child = nullptr;
    std::string text;


    Delegate<void(lv_event_t *)> on_click{};
    Delegate<void(lv_event_t *)> on_long_press{};
    Delegate<void(lv_event_t *)> on_pressed{};
    Delegate<void(lv_event_t *)> on_released{};
    Delegate<void(lv_event_t *)> on_focused{};
    Delegate<void(lv_event_t *)> on_defocused{};

    static ButtonProps up() { return ButtonProps{}; }

    ButtonProps& set_style(std::shared_ptr<Styling> s) {
      style = std::move(s);
      return *this;
    }

    ButtonProps& set_child(const std::shared_ptr<VNode>& c) {
      child = c;
      return *this;
    }

    ButtonProps& label(const std::string& t) {
      text = t;
      return *this;
    }

    ButtonProps& click(auto fn) {
      on_click = fn;
      return *this;
    }

    ButtonProps& long_press(auto fn) {
      on_long_press = fn;
      return *this;
    }

    ButtonProps& pressed(auto fn) {
      on_pressed = fn;
      return *this;
    }

    ButtonProps& released(auto fn) {
      on_released = fn;
      return *this;
    }

    ButtonProps& focused(auto fn) {
      on_focused = fn;
      return *this;
    }

    ButtonProps& defocused(auto fn) {
      on_defocused = fn;
      return *this;
    }

  };
}