#pragma once

#include "component.hh"
#include "esp_log.h"
#include "shared.hh"

#include <functional>
#include <memory>
#include <string>

namespace foundation {

  struct button_props {
    std::shared_ptr<Ref> ref = nullptr;
    std::shared_ptr<Styling> style = nullptr;
    std::shared_ptr<Component> child = nullptr;
    std::string text;
    std::function<void(lv_event_t *)> on_click = nullptr;
    std::function<void(lv_event_t *)> on_long_press = nullptr;
    std::function<void(lv_event_t *)> on_pressed = nullptr;
    std::function<void(lv_event_t *)> on_released = nullptr;
    std::function<void(lv_event_t *)> on_focused = nullptr;
    std::function<void(lv_event_t *)> on_defocused = nullptr;
  };

  class Button : public Component {
  private:
    lv_obj_t *label = nullptr;

  public:
    button_props props;

    explicit Button(const button_props &props);
    ~Button() override = default;

    lv_obj_t *render() override;
    std::shared_ptr<Styling> styling() override;
    Button *append(lv_obj_t *obj) override;
  };

} // namespace foundation
