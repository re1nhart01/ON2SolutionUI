#pragma once

#include "component.hh"
#include "styling.hh"

#include <memory>
#include <vector>

extern "C" {
#include "core/waveshare_rgb_lcd_port.h"
#include "sdkconfig.h"
}

namespace foundation {

  struct view_props {
    std::shared_ptr<Ref> ref = nullptr;
    std::shared_ptr<Styling> style;
    std::vector<std::shared_ptr<Component>> children;

    short width = LV_PCT(100);
    short height = LV_SIZE_CONTENT;
    lv_flex_align_t justify_content = LV_FLEX_ALIGN_START;
    lv_flex_align_t align_items = LV_FLEX_ALIGN_CENTER;
    lv_flex_align_t track_cross_place = LV_FLEX_ALIGN_START;
    lv_flex_flow_t flex_direction = LV_FLEX_FLOW_COLUMN;
  };

  class View final : public Component {
  private:
    view_props props;
    std::vector<std::shared_ptr<Component>> children;

  public:
    explicit View(lv_obj_t *parent, const view_props& props);
    explicit View(const view_props& props);
    ~View() override = default;

    lv_obj_t* render() override;
    std::shared_ptr<Styling> styling() override;
    View* append(lv_obj_t *obj) override;

  private:
    lv_obj_t* create_initial(lv_obj_t* parental);
  };

} // namespace foundation
