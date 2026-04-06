#pragma once

#include "components/component.h"
#include "pagination_props.h"

namespace foundation {
  static void pagination_event_adapter(lv_event_t* event);
  static void pagination_draw_task_cb(lv_event_t* e);

  class Pagination final : public Component<PaginationProps> {
   private:
    std::vector<std::string> map_strings;
    std::vector<const char*> map_ptrs;
    mutable Styling btn_style{};

    void update_map_buffer() {
      map_strings.clear();
      map_ptrs.clear();

      map_strings.emplace_back(LV_SYMBOL_LEFT);
      for (int i = 1; i <= props.total_pages; ++i) {
        map_strings.push_back(std::to_string(i));
      }
      map_strings.emplace_back(LV_SYMBOL_RIGHT);

      for (const auto& s : map_strings) {
        map_ptrs.push_back(s.c_str());
      }
      map_ptrs.push_back("");
    }

   public:
    using Component::props;

    explicit Pagination(PaginationProps&& props)
        : Component(nullptr, nullptr, std::move(props)) {
      this->apply_reactive<Pagination>(this, this->props.reactive_delegates);

      if (this->props.ref != nullptr) {
        this->props.ref->set(this);
      }
    }

    ~Pagination() override {
      if (this->props.ref != nullptr) {
        this->props.ref->unlink();
      }

      if (!this->props.reactive_link.empty())
      {
        this->detach_reactives<Pagination>(this, this->props.reactive_link);
      }
    }

    lv_obj_t* render() override {
      Component::render();
      lv_obj_t* parent_obj = this->get_parent();
      if (!parent_obj)
        return nullptr;

      set_component(lv_btnmatrix_create(parent_obj));
      lv_obj_t* obj = this->get_component();

      update_map_buffer();
      lv_btnmatrix_set_map(obj, map_ptrs.data());

      lv_btnmatrix_set_btn_ctrl_all(obj, LV_BTNMATRIX_CTRL_CHECKABLE);
      lv_btnmatrix_clear_btn_ctrl(obj, 0, LV_BTNMATRIX_CTRL_CHECKABLE);
      lv_btnmatrix_clear_btn_ctrl(obj, props.total_pages + 1,
                                  LV_BTNMATRIX_CTRL_CHECKABLE);
      lv_btnmatrix_set_one_checked(obj, true);

      lv_obj_add_event_cb(obj, pagination_draw_task_cb,
                          LV_EVENT_DRAW_PART_BEGIN, this);
      lv_obj_add_event_cb(obj, pagination_event_adapter, LV_EVENT_VALUE_CHANGED,
                          this);


      if (props.style_override) {
        props.style_override(style);
      }
      lv_obj_add_style(obj, style.getStyle(), LV_PART_MAIN);

      if (props.btn_style_override) {
        props.btn_style_override(btn_style);
      }
      lv_obj_add_style(obj, btn_style.getStyle(), LV_PART_ITEMS);

      do_rebuild();
      return obj;
    }

    void do_rebuild() override {
      lv_obj_t* obj = this->get_component();
      if (!obj) return;

      update_map_buffer();
      lv_btnmatrix_set_map(obj, map_ptrs.data());

      if (props.current_page < 1) props.current_page = 1;
      if (props.current_page > props.total_pages) props.current_page = props.total_pages;

      lv_btnmatrix_set_btn_ctrl(obj, props.current_page, LV_BTNMATRIX_CTRL_CHECKED);

      int btn_count = props.total_pages + 2;
      int gap = 8;
      short total_w = (props.width * btn_count) + (gap * (btn_count - 1));

      lv_obj_set_size(obj, total_w, props.height);
      lv_obj_set_style_pad_gap(obj, gap, LV_PART_MAIN);
      lv_obj_set_style_pad_all(obj, 0, LV_PART_MAIN);
      lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN);

      if (props.style_override) {
        props.style_override(style);
        lv_obj_refresh_style(obj, LV_PART_MAIN, LV_STYLE_PROP_ANY);
      }

      if (props.btn_style_override) {
        props.btn_style_override(btn_style);
        lv_obj_refresh_style(obj, LV_PART_ITEMS, LV_STYLE_PROP_ANY);
      }

    }

    const Styling* styling() const override {
      apply_base_style(style);
      if (props.style_override)
        props.style_override(style);
      return &style;
    }

    Pagination* append(lv_obj_t* obj) override {
      lv_obj_set_parent(obj, get_component());
      return this;
    };
  };

  static void pagination_draw_task_cb(lv_event_t* e) {
    lv_obj_draw_part_dsc_t* dsc = lv_event_get_draw_part_dsc(e);

    if (dsc->class_p == &lv_btnmatrix_class &&
        dsc->type == LV_BTNMATRIX_DRAW_PART_BTN) {
      lv_obj_t* obj = lv_event_get_target(e);

      if (lv_btnmatrix_has_btn_ctrl(obj, dsc->id, LV_BTNMATRIX_CTRL_CHECKED)) {
        dsc->rect_dsc->bg_color =
            lv_color_hex(0xDDEFEF);
        dsc->rect_dsc->bg_opa = LV_OPA_COVER;
        dsc->label_dsc->color =
            lv_color_hex(0x00897B);
      } else {
        dsc->rect_dsc->bg_opa = 0;
      }
      dsc->rect_dsc->radius = 8;
    }
  }

  static void pagination_event_adapter(lv_event_t* e) {
    auto* instance = static_cast<Pagination*>(lv_event_get_user_data(e));
    if (!instance)
      return;

    lv_obj_t* obj = lv_event_get_target(e);
    uint32_t id = lv_btnmatrix_get_selected_btn(obj);
    if (id == LV_BTNMATRIX_BTN_NONE)
      return;

    int current = instance->props.current_page;
    int total = instance->props.total_pages;

    if (id == 0) {
      if (current > 1)
        current--;
    } else if (id == (uint32_t)(total + 1)) {
      if (current < total)
        current++;
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
}  // namespace foundation