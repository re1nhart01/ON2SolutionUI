#pragma once

#include "pagination_props.h"
#include "components/component.h"

namespace foundation
{
  static void pagination_event_adapter(lv_event_t *event);

  class Pagination final : public Component<PaginationProps> {
  private:
    std::vector<std::string> map_strings;
    std::vector<const char*> map_ptrs;

    void update_map_buffer() {
        map_strings.clear();
        map_ptrs.clear();

        map_strings.push_back(LV_SYMBOL_LEFT);
        for (int i = 1; i <= props.total_pages; ++i) {
            map_strings.push_back(std::to_string(i));
        }
        map_strings.push_back(LV_SYMBOL_RIGHT);

        for (const auto& s : map_strings) {
            map_ptrs.push_back(s.c_str());
        }
        map_ptrs.push_back("");
    }

  public:
    using Component::props;

    explicit Pagination(PaginationProps&& props) : Component(nullptr, nullptr, std::move(props)) {
        this->apply_reactive<Pagination>(this, this->props.reactive_delegates);
    }

    lv_obj_t *render() override {
      Component::render();
      lv_obj_t* parent_obj = this->get_parent();
      if (!parent_obj) return nullptr;

      set_component(lv_btnmatrix_create(parent_obj));
      lv_obj_t* obj = this->get_component();

      update_map_buffer();
      lv_btnmatrix_set_map(obj, map_ptrs.data());

      lv_btnmatrix_set_btn_ctrl_all(obj, LV_BTNMATRIX_CTRL_CHECKABLE);
      lv_btnmatrix_clear_btn_ctrl(obj, 0, LV_BTNMATRIX_CTRL_CHECKABLE); // Left symbol
      lv_btnmatrix_clear_btn_ctrl(obj, props.total_pages + 1, LV_BTNMATRIX_CTRL_CHECKABLE); // Right symbol
      
      lv_btnmatrix_set_one_checked(obj, true);
      
      const auto style = this->styling();
      lv_obj_add_style(obj, style->getStyle(), LV_PART_MAIN);

      lv_obj_add_event_cb(obj, pagination_event_adapter, LV_EVENT_VALUE_CHANGED, this);

      do_rebuild();
      return obj;
    }

    void do_rebuild() override {
      lv_obj_t* obj = this->get_component();
      if (!obj) return;

      if (props.current_page < 1) props.current_page = 1;
      if (props.current_page > props.total_pages) props.current_page = props.total_pages;

      lv_btnmatrix_set_btn_ctrl(obj, props.current_page, LV_BTNMATRIX_CTRL_CHECKED);

      if (const auto style = this->styling(); style != nullptr) {
        if (style->width > 0) lv_obj_set_width(obj, style->width);
        if (style->height > 0) lv_obj_set_height(obj, style->height);
      }
    }

    const Styling* styling() const override {
      apply_base_style(style);
      if (props.style_override) props.style_override(style);
      return &style;
    }

    Pagination *append(lv_obj_t *obj) override
    {
      lv_obj_set_parent(obj, get_component());
      return this;
    };
  };


  static void pagination_event_adapter(lv_event_t *e) {
    auto *instance = static_cast<Pagination *>(lv_event_get_user_data(e));
    if (!instance) return;

    lv_obj_t * obj = lv_event_get_target(e);
    uint32_t id = lv_btnmatrix_get_selected_btn(obj);
    int current = instance->props.current_page;
    int total = instance->props.total_pages;

    if (id == 0) {
        if (current > 1) current--;
    } else if (id == total + 1) {
        if (current < total) current++;
    } else {
        current = (int)id; 
    }

    if (current != instance->props.current_page) {
        instance->props.current_page = current;
        if (instance->props.on_page_changed) {
            instance->props.on_page_changed(current);
        }
        instance->do_rebuild();
    }
  }
}