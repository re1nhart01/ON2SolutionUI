#pragma once
#include "components/component.h"
#include "scroll_view_props.h"

namespace foundation {
  class ScrollView final : public Component<ScrollViewProps> {
   public:
    explicit ScrollView(ScrollViewProps&& props)
        : Component(nullptr, nullptr, std::move(props)) {
      this->apply_reactive<ScrollView>(this, this->props.reactive_delegates);
      if (this->props.ref != nullptr) {
        this->props.ref->set(this);
      }
    }

    ~ScrollView() override {
      if (!this->props.reactive_link.empty()) {
        this->detach_reactives<ScrollView>(this, this->props.reactive_link);
      }
      if (this->props.ref != nullptr) {
        this->props.ref->unlink();
      }
    }

    lv_obj_t* render() override {
      if (get_component() == nullptr || this->parent == nullptr) {
        this->set_component(this->create_initial(this->parent));
      }

      lv_obj_t* comp = get_component();

      // Layout
      lv_obj_set_layout(comp, LV_LAYOUT_FLEX);
      lv_obj_set_size(comp, props.width, props.height);
      lv_obj_set_flex_flow(comp, props.flex_direction);
      lv_obj_set_flex_align(comp, props.justify_content, props.align_items,
                            props.track_cross_place);

      lv_obj_set_scroll_snap_x(comp, props.snap_x);
      lv_obj_set_scroll_snap_y(comp, props.snap_y);

      // Physics flags
      if (props.momentum)
        lv_obj_add_flag(comp, LV_OBJ_FLAG_SCROLL_MOMENTUM);
      else
        lv_obj_clear_flag(comp, LV_OBJ_FLAG_SCROLL_MOMENTUM);

      if (props.elastic)
        lv_obj_add_flag(comp, LV_OBJ_FLAG_SCROLL_ELASTIC);
      else
        lv_obj_clear_flag(comp, LV_OBJ_FLAG_SCROLL_ELASTIC);

      // Gesture bubble (если надо)
      if (props.gesture_bubble)
        lv_obj_add_flag(comp, LV_OBJ_FLAG_GESTURE_BUBBLE);
      else
        lv_obj_clear_flag(comp, LV_OBJ_FLAG_GESTURE_BUBBLE);

      if (props.disabled) {
        lv_obj_set_scroll_dir(comp, LV_DIR_NONE);
        lv_obj_set_scrollbar_mode(comp, LV_SCROLLBAR_MODE_OFF);
      } else {
        lv_obj_set_scroll_dir(comp, props.scroll_dir);
        lv_obj_set_scrollbar_mode(comp, props.scrollbar_mode);
      }

      // Render children
      for (const auto& child : props.children) {
        if (child != nullptr) {
          child->set_active(true);
          child->set_parent(comp);
          child->render();
        }
      }

      // Styling
      if (auto style = styling()) {
        lv_obj_add_style(comp, style->getStyle(), LV_PART_MAIN);
      }

      return comp;
    }

    void do_rebuild() override {
      lv_obj_t* comp = get_component();
      if (!comp)
        return;

      this->set_active(props.is_visible);

      lv_obj_set_layout(comp, LV_LAYOUT_FLEX);
      lv_obj_set_size(comp, props.width, props.height);
      lv_obj_set_flex_flow(comp, props.flex_direction);
      lv_obj_set_flex_align(comp, props.justify_content, props.align_items,
                            props.track_cross_place);

      lv_obj_set_scroll_snap_x(comp, props.snap_x);
      lv_obj_set_scroll_snap_y(comp, props.snap_y);

      // Physics flags
      if (props.momentum)
        lv_obj_add_flag(comp, LV_OBJ_FLAG_SCROLL_MOMENTUM);
      else
        lv_obj_clear_flag(comp, LV_OBJ_FLAG_SCROLL_MOMENTUM);

      if (props.elastic)
        lv_obj_add_flag(comp, LV_OBJ_FLAG_SCROLL_ELASTIC);
      else
        lv_obj_clear_flag(comp, LV_OBJ_FLAG_SCROLL_ELASTIC);

      // Gesture bubble (если надо)
      if (props.gesture_bubble)
        lv_obj_add_flag(comp, LV_OBJ_FLAG_GESTURE_BUBBLE);
      else
        lv_obj_clear_flag(comp, LV_OBJ_FLAG_GESTURE_BUBBLE);

      if (props.disabled) {
        lv_obj_set_scroll_dir(comp, LV_DIR_NONE);
        lv_obj_set_scrollbar_mode(comp, LV_SCROLLBAR_MODE_OFF);
      } else {
        lv_obj_set_scroll_dir(comp, props.scroll_dir);
        lv_obj_set_scrollbar_mode(comp, props.scrollbar_mode);
      }

      if (auto style = styling(); style->get_is_dirty()) {
        lv_obj_invalidate(comp);
      }

      for (const auto& child : props.children) {
        child->do_rebuild();
      }
    }

    void refresh_childrens() const {
      lv_obj_t* obj = get_component();
      if (!obj)
        return;

      lv_obj_clean(obj);

      for (const auto& child : props.children) {
        if (child) {
          child->set_component(nullptr);
          child->set_active(true);
          child->set_parent(obj);
          child->render();
        }
      }

      lv_obj_update_layout(obj);
    }

    const Styling* styling() const override {
      apply_base_style(style);

      if (props.style_override) {
        props.style_override(style);
      }

      return &style;
    }

    ScrollView* append(lv_obj_t* obj) override {
      lv_obj_set_parent(obj, get_component());
      return this;
    }

   private:
    lv_obj_t* create_initial(lv_obj_t* parent) const {
      return lv_obj_create(parent);
    }
  };
}  // namespace foundation
