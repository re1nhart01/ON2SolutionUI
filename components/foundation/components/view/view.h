#pragma once
#include "components/component.h"
#include "view_props.h"

namespace foundation
{
  class View final : public Component<ViewProps> {
  private:

  public:
    explicit View(lv_obj_t * parent, const ViewProps &props) : Component(nullptr, parent, props) {
      set_style(props.style);

      if(this->props.ref != nullptr) {
          this->props.ref->set(this);
      }
    };

    explicit View(const ViewProps &props) : Component(nullptr, nullptr, std::move(props)) {
      set_style(props.style);

      if(this->props.ref != nullptr) {
          this->props.ref->set(this);
      }
    };

    ~View() override
    {
      if (this->props.ref != nullptr) {
          this->props.ref->unlink();
      }
    };

    lv_obj_t* render() override
    {
      if(get_component() == nullptr || this->parent == nullptr) {
          this->set_component(this->create_initial(this->parent));
      }
      lv_obj_t *comp = get_component();
      lv_obj_set_layout(comp, LV_LAYOUT_FLEX);
      lv_obj_set_size(comp, props.width, props.height);
      lv_obj_set_flex_flow(comp, props.flex_direction);
      lv_obj_set_flex_align(comp, props.justify_content, props.align_items,
                            props.track_cross_place);

      lv_obj_set_scroll_dir(comp, LV_DIR_NONE);
      lv_obj_set_scrollbar_mode(comp, LV_SCROLLBAR_MODE_OFF);
      std::shared_ptr<Styling> style = this->styling();

      for (const auto& child : this->props.children) {
          if (child != nullptr) {
              child->set_active(true);
              child->set_parent(comp);
              child->render();
              lv_obj_set_parent(child->get_component(), comp);
          }
      }

      if (style != nullptr) {
          lv_obj_add_style(this->get_component(), style->getStyle(), LV_PART_MAIN);
      }

      return comp;
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

      if (std::shared_ptr<Styling> style = this->styling(); style != nullptr) {
          style->applyTo(this->get_component());
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

    std::shared_ptr<Styling> styling() override
    {
      if (this->props.style) {
          return this->props.style;
      }
      return {};
    };

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