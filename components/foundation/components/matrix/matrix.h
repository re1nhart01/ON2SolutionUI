
#pragma once
#include "matrix_props.h"
#include "components/component.h"

namespace foundation
{
  static void matrix_event_adapter(lv_event_t *event);

  class Matrix final : public Component<MatrixProps> {
  private:

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

      if (this->props.btn_map && this->props.backspace && this->props.submit_sym) {
        lv_btnmatrix_set_map(obj, this->props.btn_map);
        lv_btnmatrix_set_btn_width(obj, this->props.submit_pos, 2);
        lv_btnmatrix_set_btn_ctrl(obj, this->props.submit_pos, LV_BTNMATRIX_CTRL_CHECKED);

        lv_obj_add_event_cb(obj, matrix_event_adapter, LV_EVENT_ALL, this);
      }

      lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);

      do_rebuild();

      return this->get_component();
    };

    void do_rebuild() override
    {
      lv_obj_t* obj = this->get_component();
      if (!obj) return;

      lv_obj_set_layout(obj, LV_LAYOUT_FLEX);
      lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_ROW);
      lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_CENTER,
                            LV_FLEX_ALIGN_CENTER);

      this->set_active(this->props.is_visible);

      if (auto style = styling(); style->get_is_dirty()) {
        lv_obj_invalidate(obj);
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

    if (strcmp(txt, "<-") == 0) {
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