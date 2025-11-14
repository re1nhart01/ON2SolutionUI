#pragma once

namespace foundation
{
  struct ScrollViewProps final : BaseProps<ScrollViewProps> {
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

    static ScrollViewProps up() { return ScrollViewProps{}; }

    ScrollViewProps& add_child(const std::shared_ptr<Component>& c) {
      children.push_back(c);
      return *this;
    }

    ScrollViewProps& set_children(const std::vector<std::shared_ptr<Component>>& ch) {
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

    ScrollViewProps& scroll_x(bool v = true) {
      horizontal = v;
      return *this;
    }

    ScrollViewProps& scroll_y(bool v = true) {
      horizontal = !v;
      return *this;
    }

    ScrollViewProps& disable(bool v = true) {
      disabled = v;
      return *this;
    }

    ScrollViewProps& enable() {
      disabled = false;
      return *this;
    }
  };
}