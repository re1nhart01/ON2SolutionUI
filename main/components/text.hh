#pragma once

#include "component.hh"
#include <memory>
#include <string>

namespace foundation {

  struct text_props {
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

  class Text final : public Component {
  public:
    text_props props;
    explicit Text(text_props props);
    ~Text() override = default;

    lv_obj_t* render() override;
    void do_rebuild() override;
    std::shared_ptr<Styling> styling() override;
    Text* append(lv_obj_t* obj) override;

    void update(const std::string &value) const;
  };

} // namespace foundation
