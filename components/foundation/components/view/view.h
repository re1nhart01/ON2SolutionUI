#pragma once
#include "components/component.h"
#include "view_props.h"

namespace foundation
{
  class View final : public Component<ViewProps> {
  private:

  public:
    explicit View(ViewProps&& props) : Component(nullptr, nullptr, std::move(props)) {
      this->apply_reactive<View>(this, this->props.reactive_delegates);
      if(this->props.ref != nullptr) {
          this->props.ref->set(this);
      }
    };

    ~View() override
    {
      if (this->props.ref != nullptr) {
          this->props.ref->unlink();
      }
      if (!this->props.reactive_link.empty())
      {
        this->detach_reactives<View>(this, this->props.reactive_link);
      }
    };

    lv_obj_t* render() override
    {
      if(get_component() == nullptr || this->parent == nullptr) {
          this->set_component(this->create_initial(this->parent));
      }
      lv_obj_t *obj = get_component();
      lv_obj_set_layout(obj, LV_LAYOUT_FLEX);
      lv_obj_set_size(obj, props.width, props.height);
      lv_obj_set_flex_flow(obj, props.flex_direction);
      lv_obj_set_flex_align(obj, props.justify_content, props.align_items,
                            props.track_cross_place);

      lv_obj_set_scroll_dir(obj, LV_DIR_NONE);
      lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
      const auto style = this->styling();

      if (this->props.overflow_visible) {
        lv_obj_add_flag(obj, LV_OBJ_FLAG_OVERFLOW_VISIBLE);
      } else {
        lv_obj_clear_flag(obj, LV_OBJ_FLAG_OVERFLOW_VISIBLE);
      }

      for (const auto& child : this->props.children) {
          if (child != nullptr) {
              child->set_active(true);
              child->set_parent(obj);
              child->render();
          }
      }

      if (style != nullptr) {
          lv_obj_add_style(this->get_component(), style->getStyle(), LV_PART_MAIN);
      }

      return obj;
    };

    void do_rebuild() override
    {
      lv_obj_t* obj = this->get_component();
      if (!obj) return;

      this->set_active(this->props.is_visible);
      lv_obj_set_layout(obj, LV_LAYOUT_FLEX);
      lv_obj_set_size(obj, props.width, props.height);
      lv_obj_set_flex_flow(obj, props.flex_direction);
      lv_obj_set_flex_align(obj, props.justify_content, props.align_items,
                            props.track_cross_place);

      lv_obj_set_scroll_dir(obj, LV_DIR_NONE);
      lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);

      if (auto style = styling(); style->get_is_dirty()) {
        lv_obj_invalidate(obj);
      }

      for (const auto &child : this->props.children) {
          child->do_rebuild();
      }
    };

    void refresh_childrens() const
    {
      lv_obj_t* obj = this->get_component();
      if (!obj) return;

      lv_obj_clean(obj);

      for (const auto& child : this->props.children) {
          if (child) {
              child->set_component(nullptr);
              child->set_active(true);
              child->set_parent(obj);
              child->render();
          }
      }

      lv_obj_update_layout(obj);
    }

    const Styling* styling() const override
    {
      apply_base_style(style);

      if (props.style_override) {
          props.style_override(style);
      }

      return &style;
    }

    View* append(lv_obj_t *obj) override
    {
      lv_obj_set_parent(obj, get_component());
      return this;
    };

  private:
    lv_obj_t* create_initial(lv_obj_t* parental) const
    {
      return lv_obj_create(parental);
    };
  };
}