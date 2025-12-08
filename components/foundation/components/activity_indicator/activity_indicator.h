//
// Created by evgeniy on 11/23/25.
//

#pragma once
#include "activity_indicator_props.h"

namespace foundation
{
  class ActivityIndicator : public Component<ActivityIndicatorProps>
  {
    using Component<ActivityIndicatorProps>::Component;
    public:

    explicit ActivityIndicator(const ActivityIndicatorProps &props) : Component(nullptr, nullptr, std::move(props)) {
      if (this->props.ref != nullptr) {
          this->props.ref->set(this);
      }
    }

    ~ActivityIndicator() override {
      if (this->props.ref != nullptr) {
          this->props.ref->unlink();
      }
    };

    lv_obj_t *render() override
    {
      Component::render();
      lv_obj_t* parent_obj = this->get_parent();
      if (!parent_obj) return nullptr;

      set_component(lv_spinner_create(this->parent, this->props.spin_time, this->props.arc_length));
      lv_obj_t* obj = this->get_component();

      if (props.style) {
          lv_obj_add_style(obj, props.style->getStyle(), 0);
      }

      do_rebuild();

      return this->get_component();
    };

    void do_rebuild() override
    {
      lv_obj_t* obj = this->get_component();

      lv_obj_set_size(obj, props.size, props.size);
      lv_obj_set_style_arc_color(obj, this->props.color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
      lv_obj_set_style_arc_opa(obj, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);

      this->set_active(this->props.is_visible);

      if (props.style) {
          lv_obj_add_style(obj, props.style->getStyle(), 0);
      }

    }

    std::shared_ptr<Styling> styling() override
    {
      if (this->props.style) {
          return this->props.style;
      }
      return {};
    };

    ActivityIndicator *append(lv_obj_t *obj) override
    {
      lv_obj_set_parent(obj, get_component());
      return this;
    };
  };
}