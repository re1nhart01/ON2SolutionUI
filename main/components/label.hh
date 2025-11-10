#pragma once

#include "component.hh"
#include <memory>
#include <string>

namespace foundation {

  struct label_props {
    std::shared_ptr<Ref> ref = nullptr;
    std::shared_ptr<Styling> style;
    std::string text;

    lv_event_cb_t on_click = nullptr;
    lv_event_cb_t on_long_press = nullptr;
    lv_event_cb_t on_pressed = nullptr;
    lv_event_cb_t on_released = nullptr;
    lv_event_cb_t on_focused = nullptr;
    lv_event_cb_t on_defocused = nullptr;
  };

  class Label final : public Component {
  private:
    label_props props;

  public:
    explicit Label(label_props props);
    ~Label() override = default;

    lv_obj_t* render() override;
    std::shared_ptr<Styling> styling() override;
    Label* append(lv_obj_t* obj) override;

    void update(const std::string &value) const;
  };

} // namespace foundation
