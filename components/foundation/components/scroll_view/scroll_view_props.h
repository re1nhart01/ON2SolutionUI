#pragma once
#include "components/props.h";

namespace foundation
{
  class ScrollView;
  struct ScrollViewProps final : BaseProps<ScrollViewProps, ScrollView> {
    std::vector<std::unique_ptr<VNode>> children;
    short width = LV_PCT(100);
    short height = LV_SIZE_CONTENT;
    lv_flex_align_t justify_content = LV_FLEX_ALIGN_START;
    lv_flex_align_t align_items = LV_FLEX_ALIGN_CENTER;
    lv_flex_align_t track_cross_place = LV_FLEX_ALIGN_START;
    lv_flex_flow_t flex_direction = LV_FLEX_FLOW_COLUMN;

    lv_dir_t scroll_dir = LV_DIR_VER;
    lv_scrollbar_mode_t scrollbar_mode = LV_SCROLLBAR_MODE_AUTO;
    bool disabled = false;

    lv_scroll_snap_t snap_x = LV_SCROLL_SNAP_NONE;
    lv_scroll_snap_t snap_y = LV_SCROLL_SNAP_NONE;

    // physics
    bool momentum = true;     // LV_OBJ_FLAG_SCROLL_MOMENTUM
    bool elastic = true;      // LV_OBJ_FLAG_SCROLL_ELASTIC

    // optional: scroll chaining behavior
    bool gesture_bubble = false; // LV_OBJ_FLAG_GESTURE_BUBBLE

    // optional: keep scroll position on rebuild
    bool keep_scroll_pos = true;

    ScrollViewProps&& add_child(std::unique_ptr<VNode> c) {
      children.push_back(std::move(c));
      return std::move(*this);
    }

    ScrollViewProps&& set_children(std::vector<std::unique_ptr<VNode>>&& ch) {
      children = std::move(ch);
      return std::move(*this);
    }

    ScrollViewProps&& w(short v) {
      width = v;
      return std::move(*this);
    }

    ScrollViewProps&& h(short v) {
      height = v;
      return std::move(*this);
    }

    ScrollViewProps&& direction(lv_flex_flow_t value) {
      flex_direction = value;
      return std::move(*this);
    }

    ScrollViewProps&& justify(lv_flex_align_t value) {
      justify_content = value;
      return std::move(*this);
    }

    ScrollViewProps&& items(lv_flex_align_t value) {
      align_items = value;
      return std::move(*this);
    }

    ScrollViewProps&& track_cross(lv_flex_align_t value) {
      track_cross_place = value;
      return std::move(*this);
    }

    ScrollViewProps&& scroll(lv_dir_t dir) {
      scroll_dir = dir;
      return std::move(*this);
    }

    ScrollViewProps&& scrollbar(lv_scrollbar_mode_t mode) {
      scrollbar_mode = mode;
      return std::move(*this);
    }

    ScrollViewProps&& disable(bool v = false) {
      disabled = v;
      return std::move(*this);
    }

    ScrollViewProps&& snap(lv_scroll_snap_t x, lv_scroll_snap_t y = LV_SCROLL_SNAP_NONE) {
      snap_x = x;
      snap_y = y;
      return std::move(*this);
    }

    ScrollViewProps&& set_momentum(bool v) {
      momentum = v;
      return std::move(*this);
    }

    ScrollViewProps&& set_elastic(bool v) {
      elastic = v;
      return std::move(*this);
    }

    ScrollViewProps&& bubble(bool v = true) {
      gesture_bubble = v;
      return std::move(*this);
    }

    ScrollViewProps&& preserve_scroll(bool v = true) {
      keep_scroll_pos = v;
      return std::move(*this);
    }

  };
}