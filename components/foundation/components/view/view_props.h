#pragma once

#include "components/props.h"
#include "misc/lv_area.h"
#include <lvgl.h>

#include "components/vnode.h"
#include <memory>
#include <vector>

namespace foundation
{
  class View;
}

namespace foundation
{

  enum class FlexPreset {
    ColumnCenter,   // Столбик, всё по центру
    RowBetween, // Строка, элементы враспор (justify-between)
    RowStart,
    RowCenter,      // Строка, всё по центру
    Center,         // И по вертикали, и по горизонтали в центр
    ColumnStart,   // Список сверху вниз, прижат к левому краю
    SpaceCenter // column + space between + align items center;
  };

  struct ViewProps final : public BaseProps<ViewProps, View>
  {

    ViewProps() = default;
    ViewProps(const ViewProps&) = delete;
    ViewProps& operator=(const ViewProps&) = delete;
    ViewProps(ViewProps&&) = default;
    ViewProps& operator=(ViewProps&&) = default;

    std::vector<std::unique_ptr<VNode>> children;

    short width = LV_PCT(100);
    short height = LV_SIZE_CONTENT;
    lv_flex_align_t justify_content = LV_FLEX_ALIGN_START;
    lv_flex_align_t align_items = LV_FLEX_ALIGN_CENTER;
    lv_flex_align_t track_cross_place = LV_FLEX_ALIGN_START;
    lv_flex_flow_t flex_direction = LV_FLEX_FLOW_COLUMN;
    bool overflow_visible = false;


    // --- size ---
    ViewProps&& w(short value)
    {
      width = value;
      return std::move(*this);
    }

    ViewProps&& set_overflow(bool value)
    {
      overflow_visible = value;
      return std::move(*this);
    }

    ViewProps&& h(short value)
    {
      height = value;
      return std::move(*this);
    }

    // --- children ---
    ViewProps&& set_children(std::vector<std::unique_ptr<VNode>> value)
    {
      children = std::move(value);
      return std::move(*this);
    }

    // --- layout ---
    ViewProps&& direction(lv_flex_flow_t value)
    {
      flex_direction = value;
      return std::move(*this);
    }

    ViewProps&& justify(lv_flex_align_t value)
    {
      justify_content = value;
      return std::move(*this);
    }

    ViewProps&& items(lv_flex_align_t value)
    {
      align_items = value;
      return std::move(*this);
    }

    ViewProps&& track_cross(lv_flex_align_t value)
    {
      track_cross_place = value;
      return std::move(*this);
    }

    ViewProps&& flow(FlexPreset preset) {
      switch (preset) {
        case FlexPreset::ColumnCenter:
          flex_direction = LV_FLEX_FLOW_COLUMN;
          justify_content = LV_FLEX_ALIGN_CENTER;
          align_items = LV_FLEX_ALIGN_CENTER;
          track_cross_place = LV_FLEX_ALIGN_CENTER;
          break;

        case FlexPreset::RowBetween:
          flex_direction = LV_FLEX_FLOW_ROW;
          justify_content = LV_FLEX_ALIGN_SPACE_BETWEEN;
          align_items = LV_FLEX_ALIGN_CENTER;
          track_cross_place = LV_FLEX_ALIGN_CENTER;
          break;

        case FlexPreset::RowCenter:
          flex_direction = LV_FLEX_FLOW_ROW;
          justify_content = LV_FLEX_ALIGN_CENTER;
          align_items = LV_FLEX_ALIGN_CENTER;
          track_cross_place = LV_FLEX_ALIGN_CENTER;
          break;

        case FlexPreset::Center:
          justify_content = LV_FLEX_ALIGN_CENTER;
          align_items = LV_FLEX_ALIGN_CENTER;
          track_cross_place = LV_FLEX_ALIGN_CENTER;
          break;

        case FlexPreset::ColumnStart:
          flex_direction = LV_FLEX_FLOW_COLUMN;
          justify_content = LV_FLEX_ALIGN_START;
          align_items = LV_FLEX_ALIGN_START;
          track_cross_place = LV_FLEX_ALIGN_START;
          break;
        case FlexPreset::RowStart:
          flex_direction = LV_FLEX_FLOW_ROW;
          justify_content = LV_FLEX_ALIGN_START;
          align_items = LV_FLEX_ALIGN_START;
          track_cross_place = LV_FLEX_ALIGN_START;
        case FlexPreset::SpaceCenter:
          justify_content = LV_FLEX_ALIGN_SPACE_BETWEEN;
          align_items = LV_FLEX_ALIGN_CENTER;
          track_cross_place = LV_FLEX_ALIGN_CENTER;
      }
      return std::move(*this);
    }

  };
}
