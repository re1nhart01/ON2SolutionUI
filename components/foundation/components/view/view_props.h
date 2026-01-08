#pragma once

#include "components/props.h"
#include "misc/lv_area.h"
#include <lvgl__lvgl/src/lvgl.h>

#include "components/vnode.h"
#include "components/props.h";
#include <memory>

namespace foundation
{
  class View;
}
namespace foundation
{
  struct ViewProps final : public BaseProps<ViewProps, View>
  {
    std::vector<std::unique_ptr<VNode>> children;

    short width = LV_PCT(100);
    short height = LV_SIZE_CONTENT;
    lv_flex_align_t justify_content = LV_FLEX_ALIGN_START;
    lv_flex_align_t align_items = LV_FLEX_ALIGN_CENTER;
    lv_flex_align_t track_cross_place = LV_FLEX_ALIGN_START;
    lv_flex_flow_t flex_direction = LV_FLEX_FLOW_COLUMN;

    ViewProps w(const short value)
    {
      this->width = value;
      return std::move(*this);
    }

    ViewProps h(const short value)
    {
      this->height = value;
      return std::move(*this);
    }

    ViewProps set_children(std::vector<std::unique_ptr<VNode>>&& value)
    {
      this->children = std::move(value);
      return std::move(*this);
    }

    ViewProps direction(lv_flex_flow_t value) {
      flex_direction = value;
      return std::move(*this);
    }

    ViewProps justify(lv_flex_align_t value) {
      justify_content = value;
      return std::move(*this);
    }

    ViewProps items(lv_flex_align_t value) {
      align_items = value;
      return std::move(*this);
    }

    ViewProps track_cross(lv_flex_align_t value) {
      track_cross_place = value;
      return std::move(*this);
    }
  };
}