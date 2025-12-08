#pragma once
#include "circular_progress_props.h"

namespace foundation
{
  class CircularProgress final : public Component<CircularProgressProps> {
  private:
    using Component::props;
    std::shared_ptr<Text> label = nullptr;
    lv_obj_t* arc_reference = nullptr;
    bool is_show_label = false;

  public:
    explicit CircularProgress(CircularProgressProps props)
      : Component(nullptr, nullptr, std::move(props)) {
      this->parent = nullptr;

      this->is_show_label = props.show_label_default;

      if (this->props.ref != nullptr) {
          this->props.ref->set(this);
      }
    };

    ~CircularProgress() override
    {
      if (this->props.ref != nullptr) {
          this->props.ref->unlink();
      }
    };

    lv_obj_t* render() override
    {
      if (this->get_component() != nullptr) {
          this->do_rebuild();
          return this->get_component();
      }

      Component::render();

      lv_obj_t* parent_obj = this->get_parent();
      if (!parent_obj) return nullptr;

      lv_obj_t* container = lv_obj_create(parent_obj);
      this->set_component(container);

      lv_obj_clear_flag(container, LV_OBJ_FLAG_SCROLLABLE);
      lv_obj_set_style_bg_opa(container, LV_OPA_TRANSP, LV_PART_MAIN);
      lv_obj_set_style_border_width(container, 0, LV_PART_MAIN);

      this->arc_reference = lv_arc_create(container);

      this->is_show_label = this->props.show_label_default;

      if (this->props.style != nullptr) {
          lv_obj_add_style(
              this->arc_reference,
              this->props.style->getStyle(),
              LV_PART_MAIN
          );
      }

      if (props.show_label_default)
        {
          if (!this->label)
            {
              this->label = std::make_shared<Text>(
                  TextProps::up().value(std::format("{}{}", props.default_dy, props.label_symbol))
              );
              this->label->set_parent(container);
              this->label->render();
            }

          this->label->update(
              std::format("{}{}", props.default_dy, props.label_symbol)
          );

          lv_obj_center(this->label->get_component());
          lv_obj_move_foreground(this->label->get_component());
        }
      else
        {
          if (this->label) {
              lv_obj_add_flag(this->label->get_component(), LV_OBJ_FLAG_HIDDEN);
          }
        }

      this->do_rebuild();

      return container;
    }


    void do_rebuild() override
    {
      const auto container = this->get_component();
      if (!container) return;

      lv_obj_set_size(container, props.width, props.height);

      lv_obj_set_size(arc_reference, props.width, props.height);
      lv_arc_set_range(arc_reference, props.min_dy, props.max_dy);
      lv_arc_set_value(arc_reference, props.default_dy);
      lv_arc_set_bg_angles(arc_reference, 135, 45);
      lv_arc_set_rotation(arc_reference, 0);

      lv_obj_clear_flag(arc_reference, LV_OBJ_FLAG_CLICKABLE);
      lv_obj_set_style_bg_opa(arc_reference, LV_OPA_TRANSP, LV_PART_MAIN);
      lv_obj_remove_style(arc_reference, nullptr, LV_PART_KNOB);

      if (props.style != nullptr) {
          lv_obj_add_style(
              arc_reference,
              props.style->getStyle(),
              LV_PART_MAIN
          );
      }

      lv_obj_center(arc_reference);

      if (!props.show_label_default)
        {
          if (this->label) {
              lv_obj_add_flag(this->label->get_component(), LV_OBJ_FLAG_HIDDEN);
          }
        }
      this->update(props.default_dy);
    }

    std::shared_ptr<Styling> styling() override
    {
      if (this->props.style != nullptr) {
          lv_style_set_arc_color(this->props.style->getStyle(), lv_color_hex(0x000000));
      }
      return this->props.style;
    };

    CircularProgress* append(lv_obj_t* obj) override
    {
      lv_obj_set_parent(obj, get_component());
      return this;
    };

    void update(const short value) const
    {
      const short correct_value = std::clamp(value, props.min_dy, props.max_dy);
      if (this->arc_reference != nullptr) {
          lv_arc_set_value(this->arc_reference, correct_value);
      }

      if (this->is_show_label && this->label) {
          this->label->update(std::format(
              "{}{}",
              correct_value,
              this->props.label_symbol
          ));
      }
    }
  };
}