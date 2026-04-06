
#pragma once
#include "matrix_props.h"
#include "components/component.h"

namespace foundation
{
  static void matrix_event_adapter(lv_event_t *event);
  static void matrix_draw_task_cb(lv_event_t* event);

  class Matrix final : public Component<MatrixProps> {
  private:
    mutable Styling btn_style{};

  public:
    using Component::props;

   explicit Matrix(MatrixProps&& props) : Component(nullptr, nullptr, std::move(props)) {
     this->apply_reactive<Matrix>(this, this->props.reactive_delegates);
      if (this->props.ref != nullptr) {
          this->props.ref->set(this);
      }
    }

    ~Matrix() override
    {
       if (!this->props.reactive_link.empty())
       {
         this->detach_reactives<Matrix>(this, this->props.reactive_link);
       }
      if (this->props.ref != nullptr) {
          this->props.ref->unlink();
      }
    };

    lv_obj_t *render() override
    {
      Component::render();
      lv_obj_t* parent_obj = this->get_parent();
      if (!parent_obj) return nullptr;

      set_component(lv_btnmatrix_create(parent_obj));
      lv_obj_t* obj = this->get_component();

      if (const Styling* s = styling()) {
        lv_obj_add_style(obj, s->getStyle(), LV_PART_MAIN);
      }

      if (this->props.btn_map) {
        lv_btnmatrix_set_map(obj, this->props.btn_map);

        lv_obj_add_event_cb(obj, matrix_event_adapter, LV_EVENT_VALUE_CHANGED, this);
      }

      if (this->props.submit_sym) {
        lv_btnmatrix_set_btn_width(obj, props.submit_pos, 2);
      }

      if (style.width) {
        lv_obj_set_width(obj, style.width);
      }

      if (style.height) {
        lv_obj_set_width(obj, style.height);
      }

      lv_obj_add_event_cb(obj, matrix_draw_task_cb,
                          LV_EVENT_DRAW_PART_BEGIN, this);

      if (props.btn_style_override) {
        props.btn_style_override(btn_style);
      }
      lv_obj_add_style(obj, btn_style.getStyle(), LV_PART_ITEMS);

      lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);

      do_rebuild();

      return this->get_component();
    };

    void do_rebuild() override
    {
      lv_obj_t* obj = this->get_component();
      if (!obj) return;

      this->set_active(this->props.is_visible);

      if (auto style = styling(); style->get_is_dirty()) {
        lv_obj_invalidate(obj);
      }

      if (props.style_override) {
        props.style_override(style);
        lv_obj_refresh_style(obj, LV_PART_MAIN, LV_STYLE_PROP_ANY);
      }
    }

    const Styling* styling() const override
    {
      apply_base_style(style);

      if (props.style_override) {
          props.style_override(style);
      }

      return &style;
    }

    Matrix *append(lv_obj_t *obj) override
    {
      lv_obj_set_parent(obj, get_component());
      return this;
    };
  };

  static void matrix_draw_task_cb(lv_event_t* event) {
    lv_obj_draw_part_dsc_t* dsc = lv_event_get_draw_part_dsc(event);
    auto *instance = static_cast<Matrix *>(lv_event_get_user_data(event));
    if (!instance) return;
    auto &props = instance->props;


    if (dsc->class_p == &lv_btnmatrix_class && dsc->type == LV_BTNMATRIX_DRAW_PART_BTN) {
      lv_obj_t* obj = lv_event_get_target(event);

      dsc->rect_dsc->radius = 8;
      dsc->rect_dsc->shadow_width = 0;
      dsc->rect_dsc->border_width = 0;

      if (lv_btnmatrix_has_btn_ctrl(obj, dsc->id, LV_BTNMATRIX_CTRL_CHECKED)) {
        dsc->rect_dsc->bg_color = props.active_bg_color;
        dsc->rect_dsc->bg_opa = LV_OPA_COVER;
        dsc->label_dsc->color = lv_color_hex(0x00897B);
      } else {
        dsc->rect_dsc->bg_color = props.btn_bg_color;
        dsc->rect_dsc->bg_opa = LV_OPA_COVER;
      }
    }
  }

  static void matrix_event_adapter(lv_event_t *event) {
    auto *instance = static_cast<Matrix *>(lv_event_get_user_data(event));
    if (!instance) return;

    if (event->code != LV_EVENT_VALUE_CHANGED) return;

    lv_obj_t* obj = lv_event_get_target(event);

    uint32_t btn_id = lv_btnmatrix_get_selected_btn(obj);
    if (btn_id == LV_BTNMATRIX_BTN_NONE) return;

    const char* txt = lv_btnmatrix_get_btn_text(obj, btn_id);
    if (!txt) return;

    auto &props = instance->props;

    std::string value = txt;

    if (strcmp(txt, props.backspace) == 0) {
      if (props.on_change) {
        props.on_change("BACKSPACE");
      }
      return;
    }

    if (props.submit_sym && strcmp(txt, props.submit_sym) == 0) {
      if (props.on_submit) {
        props.on_submit("SUBMIT");
      }
      return;
    }

    if (props.on_change) {
      props.on_change(value);
    }
  }
}