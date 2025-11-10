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

  struct scroll_view_props {
    std::shared_ptr<Ref> ref = nullptr;
    std::shared_ptr<Styling> style;

    std::vector<std::shared_ptr<Component>> children;
    short width = LV_PCT(100);
    short height = LV_SIZE_CONTENT;
    lv_flex_align_t justify_content = LV_FLEX_ALIGN_START;
    lv_flex_align_t align_items = LV_FLEX_ALIGN_CENTER;
    lv_flex_align_t track_cross_place = LV_FLEX_ALIGN_START;
    lv_flex_flow_t flex_direction = LV_FLEX_FLOW_COLUMN;

    bool horizontal = false;
    bool disabled = false;
  };

  class ScrollView final : public Component {
  private:
    scroll_view_props props;
    std::vector<std::shared_ptr<Component>> children;

  public:
    explicit ScrollView(lv_obj_t* parent, const scroll_view_props& props);
    ~ScrollView() override = default;

    lv_obj_t* render() override;
    std::shared_ptr<Styling> styling() override;
    ScrollView* append(lv_obj_t* obj) override;

  private:
    lv_obj_t* create_initial(lv_obj_t* parental);
  };

} // namespace foundation
