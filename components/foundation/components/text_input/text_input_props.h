#pragma once

namespace foundation
{
  class TextInput;
  struct TextInputProps final : BaseProps<TextInputProps, TextInput>
  {
    const char* placeholder = nullptr;

    Delegate<void(lv_event_t *)> on_click{};
    Delegate<void(lv_event_t *)>  on_focused{};
    Delegate<void(lv_event_t *)>  on_defocused{};
    Delegate<void(std::string value)> on_value_changed{};
    Delegate<void(std::string value)> on_submit{};
    bool is_secure = false;
    bool is_one_line = false;
    char* text = {};
    short secure_timeout = 1500;
    short length = 500;
    KeyboardManager* kbManager = nullptr;


    static TextInputProps up() { return TextInputProps{}; }

    TextInputProps& set_is_one_line(const bool v)
    {
      is_one_line = v;
      return *this;
    }

    TextInputProps& set_secure_timeout(const short v)
    {
      secure_timeout = v;
      return *this;
    }

    TextInputProps& set_keyboard(KeyboardManager* v)
    {
      kbManager = v;
      return *this;
    }

    TextInputProps& set_is_secure(const bool v)
    {
      is_secure = v;
      return *this;
    }


    TextInputProps& set_length(const short v)
    {
      length = v;
      return *this;
    }

    TextInputProps& set_style(std::shared_ptr<Styling> s) {
      style = std::move(s);
      return *this;
    }

    TextInputProps& hint(const char* text) {
      placeholder = text;
      return *this;
    }

    TextInputProps& on_click_h(auto fn) {
      on_click = fn;
      return *this;
    }

    TextInputProps& on_focused_h(auto fn) {
      on_focused = fn;
      return *this;
    }

    TextInputProps& on_defocused_h(auto fn) {
      on_defocused = fn;
      return *this;
    }

    TextInputProps& on_changed_h(auto fn) {
      on_value_changed = std::move(fn);
      return *this;
    }

    TextInputProps& submit(auto fn) {
      on_submit = std::move(fn);
      return *this;
    }
  };
}