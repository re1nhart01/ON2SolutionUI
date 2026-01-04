//
// Created by evgeniy on 11/13/25.
//
#pragma once

#include "core/styling/styling.h"
#include "fragment_props.h"

namespace foundation
{
  class Fragment final : public Component<FragmentProps> {
  using Component::props;
  public:
    explicit Fragment(const FragmentProps& props)
      : Component(nullptr, nullptr, std::move(props)) {
      this->apply_reactive<Fragment>(this, props.reactive_delegates);
    };
    ~Fragment() override
    {
      if (this->props.ref != nullptr)
      {
          this->props.ref->unlink();
      }
      if (!this->props.reactive_link.empty())
      {
        this->detach_reactives<Fragment>(this, this->props.reactive_link);
      }
    };

    lv_obj_t* render() override
    {
      Component::render();

      lv_obj_t* parent_obj = this->get_parent();
      if (!parent_obj) return nullptr;

      set_component(lv_obj_create(parent_obj));
      auto* obj = get_component();

      lv_obj_set_size(obj, 1, 1);

      lv_obj_set_style_bg_opa(obj, LV_OPA_TRANSP, 0);
      lv_obj_set_style_border_width(obj, 0, 0);
      lv_obj_set_style_outline_width(obj, 0, 0);
      lv_obj_set_style_shadow_width(obj, 0, 0);
      lv_obj_set_style_radius(obj, 0, 0);
      lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE);

      return obj;
    };

    const Styling* styling() const override
    {
      style.reset();

      apply_base_style(style);

      if (props.style_override) {
          props.style_override(style);
      }

      return &style;
    }

    Fragment* append(lv_obj_t* obj) override
    {
      lv_obj_set_parent(obj, get_component());
      return this;
    };
  };
}