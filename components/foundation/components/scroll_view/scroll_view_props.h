#pragma once
#include "components/props.h";

namespace foundation
{
  class ScrollView;
  struct ScrollViewProps final : BaseProps<ScrollViewProps, ScrollView> {
    std::vector<std::shared_ptr<VNode>> children;
    short width = LV_PCT(100);
    short height = LV_SIZE_CONTENT;
    lv_flex_align_t justify_content = LV_FLEX_ALIGN_START;
    lv_flex_align_t align_items = LV_FLEX_ALIGN_CENTER;
    lv_flex_align_t track_cross_place = LV_FLEX_ALIGN_START;
    lv_flex_flow_t flex_direction = LV_FLEX_FLOW_COLUMN;

    lv_dir_t scroll_dir = LV_DIR_VER;
    lv_scrollbar_mode_t scrollbar_mode = LV_SCROLLBAR_MODE_AUTO;
    bool disabled = false;

    ScrollViewProps& add_child(const std::shared_ptr<VNode>& c) {
      children.push_back(c);
      return *this;
    }

    ScrollViewProps& set_children(const std::vector<std::shared_ptr<VNode>>& ch) {
      children = ch;
      return *this;
    }

    ScrollViewProps& w(short v) {
      width = v;
      return *this;
    }

    ScrollViewProps& h(short v) {
      height = v;
      return *this;
    }

    ScrollViewProps& direction(lv_flex_flow_t value) {
      flex_direction = value;
      return *this;
    }

    ScrollViewProps& justify(lv_flex_align_t value) {
      justify_content = value;
      return *this;
    }

    ScrollViewProps& items(lv_flex_align_t value) {
      align_items = value;
      return *this;
    }

    ScrollViewProps& track_cross(lv_flex_align_t value) {
      track_cross_place = value;
      return *this;
    }

    ScrollViewProps& scroll(lv_dir_t dir) {
      scroll_dir = dir;
      return *this;
    }

    ScrollViewProps& scrollbar(lv_scrollbar_mode_t mode) {
      scrollbar_mode = mode;
      return *this;
    }

    ScrollViewProps& disable(bool v = false) {
      disabled = v;
      return *this;
    }

  };
}