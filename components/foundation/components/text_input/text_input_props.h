#pragma once

namespace foundation
{
  class TextInput;
  struct TextInputProps final : BaseProps<TextInputProps, TextInput> {
    const char* placeholder = nullptr;
    lv_event_cb_t on_click = nullptr;
    lv_event_cb_t on_focused = nullptr;
    lv_event_cb_t on_defocused = nullptr;
    lv_event_cb_t on_value_changed = nullptr;
    std::function<void(std::string value)> on_submit = nullptr;
    bool is_secure = false;
    bool is_one_line = false;
    char* text = '';
    short secure_timeout = 1500;
    short length = 500;


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

    TextInputProps& click(lv_event_cb_t fn) {
      on_click = fn;
      return *this;
    }

    TextInputProps& focused(lv_event_cb_t fn) {
      on_focused = fn;
      return *this;
    }

    TextInputProps& defocused(lv_event_cb_t fn) {
      on_defocused = fn;
      return *this;
    }

    TextInputProps& changed(lv_event_cb_t fn) {
      on_value_changed = fn;
      return *this;
    }

    // ---- Submit handler ----
    TextInputProps& submit(std::function<void(std::string)> fn) {
      on_submit = std::move(fn);
      return *this;
    }
  };
}