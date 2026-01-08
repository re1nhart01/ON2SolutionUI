

#pragma once

#include "components/props.h";

namespace foundation
{
  class Text;
  struct TextProps final : BaseProps<TextProps, Text> {
    std::string text;

    lv_event_cb_t on_click = nullptr;
    lv_event_cb_t on_long_press = nullptr;
    lv_event_cb_t on_pressed = nullptr;
    lv_event_cb_t on_released = nullptr;
    lv_event_cb_t on_focused = nullptr;
    lv_event_cb_t on_defocused = nullptr;

    TextProps&& value(const std::string& v) {
      text = v;
      return std::move(*this);
    }

    TextProps&& click(lv_event_cb_t fn) {
      on_click = fn;
      return std::move(*this);
    }

    TextProps&& long_press(lv_event_cb_t fn) {
      on_long_press = fn;
      return std::move(*this);
    }

    TextProps&& pressed(lv_event_cb_t fn) {
      on_pressed = fn;
      return std::move(*this);
    }

    TextProps&& released(lv_event_cb_t fn) {
      on_released = fn;
      return std::move(*this);
    }

    TextProps&& focused(lv_event_cb_t fn) {
      on_focused = fn;
      return std::move(*this);
    }

    TextProps&& defocused(lv_event_cb_t fn) {
      on_defocused = fn;
      return std::move(*this);
    }
  };
}