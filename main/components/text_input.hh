#pragma once

#include "component.hh"
#include "keyboard_manager.hh"
#include <functional>
#include <string>

namespace foundation {
  class KeyboardManager;

  struct textinput_props {
    std::shared_ptr<Ref> ref = nullptr;
    std::shared_ptr<Styling> style;

    const char* placeholder = nullptr;
    lv_event_cb_t on_click = nullptr;
    lv_event_cb_t on_focused = nullptr;
    lv_event_cb_t on_defocused = nullptr;
    lv_event_cb_t on_value_changed = nullptr;
    std::function<void(std::string value)> on_submit = nullptr;
  };

  class TextInput final : public Component {
  private:
    mutable lv_obj_t* keyboard = nullptr;
    KeyboardManager* kbManager = nullptr;

  public:
    textinput_props props;

    explicit TextInput(const textinput_props& props,
                       KeyboardManager* kbManager = nullptr);
    ~TextInput() override = default;

    lv_obj_t* render() override;
    std::shared_ptr<Styling> styling() override;
    TextInput* append(lv_obj_t* obj) override;

    const char* get_text() const;
    void set_text(const char* txt) const;
  };

} // namespace foundation
