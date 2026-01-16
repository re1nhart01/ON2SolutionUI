#pragma once

#include "core/styling/styling.h"
#include "bar_props.h"

namespace foundation
{
  class Bar final : public Component<BarProps> {
    using Component::props;
  public:
    explicit Bar(BarProps&& props)
      : Component(nullptr, nullptr, std::move(props)) {
      this->apply_reactive<Bar>(this, this->props.reactive_delegates);
      if (this->props.ref != nullptr) {
        this->props.ref->set(this);
      }
    };
    ~Bar() override
    {
      if (this->props.ref != nullptr)
      {
        this->props.ref->unlink();
      }
      if (!this->props.reactive_link.empty())
      {
        this->detach_reactives<Bar>(this, this->props.reactive_link);
      }
    };

    lv_obj_t* render() override
    {
      Component::render();

      lv_obj_t* parent_obj = this->get_parent();
      if (!parent_obj) return nullptr;

      set_component(lv_bar_create(parent_obj));

      auto obj = this->get_component();

      lv_obj_set_size(obj, this->props.width, this->props.height);
      lv_bar_set_value(obj, this->props.value, LV_ANIM_OFF);

      if (this->props.reversed) {
        lv_obj_set_style_base_dir(obj, LV_BASE_DIR_RTL, 0);
      } else {
        lv_obj_set_style_base_dir(obj, LV_BASE_DIR_LTR, 0);
      }

      return obj;
    };

    void do_rebuild() override
    {
      lv_obj_t* obj = this->get_component();
      if (!obj) return;

      lv_obj_set_size(obj, this->props.width, this->props.height);
      lv_bar_set_value(obj, this->props.value, LV_ANIM_ON);

      if (auto style = styling(); style->get_is_dirty()) {
        lv_obj_invalidate(obj);
      }

      if (this->props.reversed) {
        lv_obj_set_style_base_dir(obj, LV_BASE_DIR_RTL, 0);
      } else {
          lv_obj_set_style_base_dir(obj, LV_BASE_DIR_LTR, 0);
      }

    };

    const Styling* styling() const override
    {
      apply_base_style(style);

      if (props.style_override) {
        props.style_override(style);
      }

      return &style;
    }

    Bar* append(lv_obj_t* obj) override
    {
      lv_obj_set_parent(obj, get_component());
      return this;
    };
  };
}