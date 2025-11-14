#pragma once
#include "components/props.h"
#include "core/lv_event.h"
#include "core/styling/styling.h"

#include <functional>
#include <memory>

namespace foundation
{
  struct ButtonProps final : BaseProps<ButtonProps>
  {
    std::shared_ptr<Component> child = nullptr;
    std::string text;

    std::function<void(lv_event_t *)> on_click = nullptr;
    std::function<void(lv_event_t *)> on_long_press = nullptr;
    std::function<void(lv_event_t *)> on_pressed = nullptr;
    std::function<void(lv_event_t *)> on_released = nullptr;
    std::function<void(lv_event_t *)> on_focused = nullptr;
    std::function<void(lv_event_t *)> on_defocused = nullptr;

    static ButtonProps up() { return ButtonProps{}; }

    ButtonProps& set_ref(std::shared_ptr<Ref> r) {
      ref = std::move(r);
      return *this;
    }

    ButtonProps& set_style(std::shared_ptr<Styling> s) {
      style = std::move(s);
      return *this;
    }

    ButtonProps& set_child(const std::shared_ptr<Component>& c) {
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