#pragma once
#include "components/component.h"
#include "flat_list_props.h"

namespace foundation
{
  class FlatList final : public Component<FlatListProps>
  {
  public:
    explicit FlatList(FlatListProps&& props)
        : Component(nullptr, nullptr, std::move(props))
    {
      this->apply_reactive<FlatList>(this, this->props.reactive_delegates);
        if (this->props.ref != nullptr) this->props.ref->set(this);
    }

    ~FlatList()
    {
        if (this->props.ref != nullptr) {
            this->props.ref->unlink();
        }
        if (!this->props.reactive_link.empty())
        {
            this->detach_reactives<FlatList>(this, this->props.reactive_link);
        }
    }

    lv_obj_t* render() override
    {
        if (get_component() == nullptr || this->parent == nullptr) {
            this->set_component(lv_list_create(this->parent));
        }

        lv_obj_t* list = get_component();

        lv_obj_set_size(list, props.width, props.height);

      lv_obj_clear_flag(list, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
        lv_obj_set_scroll_snap_y(list, LV_SCROLL_SNAP_NONE);
        lv_obj_clear_flag(list, LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM);

        for (const auto& child : props.children) {
            if (child != nullptr) {
                child->set_active(true);
                child->set_parent(list);
                child->render();
                lv_obj_clear_flag(child->get_component(), LV_OBJ_FLAG_SCROLL_ON_FOCUS);
            }
        }

        if (auto style = styling()) {
            lv_obj_add_style(list, style->getStyle(), LV_PART_MAIN);
        }

        lv_obj_set_scroll_snap_y(list, LV_SCROLL_SNAP_NONE);
        lv_obj_set_scroll_snap_x(list, LV_SCROLL_SNAP_NONE);
        lv_obj_clear_flag(list, LV_OBJ_FLAG_SCROLL_ELASTIC);
        lv_obj_clear_flag(list, LV_OBJ_FLAG_SCROLL_MOMENTUM);
        lv_obj_clear_flag(list, LV_OBJ_FLAG_SCROLL_CHAIN);

        return list;
    }

    void do_rebuild() override
    {
        lv_obj_t* list = get_component();
        if (!list) return;

        lv_obj_set_size(list, props.width, props.height);

        if (auto style = styling(); style->get_is_dirty()) {
            lv_obj_invalidate(list);
        }

        for (const auto& child : props.children) {
            child->do_rebuild();
        }
    }

    const Styling* styling() const override
    {
      apply_base_style(style);

      style.setBackgroundOpa(LV_OPA_COVER);

      if (props.style_override) {
          props.style_override(style);
      }
      return &style;
    }

    FlatList* append(lv_obj_t* obj) override
    {
        lv_obj_set_parent(obj, get_component());
        return this;
    }
  };
}