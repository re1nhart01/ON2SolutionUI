#pragma once
#include "components/component.h"
#include "view_props.h"

namespace foundation
{
  class View final : public Component<ViewProps> {
  private:
      using Component::props;
      std::vector<std::shared_ptr<VNode>> children;

  public:
    explicit View(lv_obj_t * parent, const ViewProps &props) : Component(nullptr, parent, props), props(props) {
      set_style(props.style);
      if(!props.children.empty()) {
          this->children.insert(this->children.end(), props.children.begin(), props.children.end());
      }

      if(this->props.ref != nullptr) {
          this->props.ref->set(this);
      }
    };

    explicit View(const ViewProps &props) : Component(nullptr, nullptr, props), props(props) {
      set_style(props.style);
      if(!props.children.empty()) {
          this->children.insert(this->children.end(), props.children.begin(), props.children.end());
      }

      if(this->props.ref != nullptr) {
          this->props.ref->set(this);
      }
    };

    ~View() override = default;

    lv_obj_t* render() override
    {
      Component::render();
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

      for (const auto& child : this->children) {
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