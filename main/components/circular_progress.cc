#include <utility>

#include "circular_progress.hh"
#include "component.hh"
#include "macro.hh"

namespace foundation {
  CircularProgress::CircularProgress(circular_props  props)
      : Component(nullptr, nullptr), props(std::move(props)) {
    this->parent = nullptr;

    this->is_show_label = props.show_label_default;

    if (this->props.ref != nullptr) {
        this->props.ref->set(this);
    }
  }

  lv_obj_t* CircularProgress::render() {
    Component::render();

    lv_obj_t* parent_obj = this->get_parent();
    if (!parent_obj) return nullptr;

    auto props = this->props;

    lv_obj_t* container = lv_obj_create(parent_obj);
    lv_obj_set_size(container, props.w + 5, props.h + 5);
    lv_obj_clear_flag(container, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_opa(container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(container, 0, LV_PART_MAIN);

    this->set_component(container);

    this->arc_reference = lv_arc_create(container);
    lv_obj_set_size(arc_reference, props.w, props.h);
    lv_arc_set_range(arc_reference, props.min_dy, props.max_dy);
    lv_arc_set_value(arc_reference, props.default_dy);
    lv_arc_set_bg_angles(arc_reference, 135, 45);
    lv_arc_set_rotation(arc_reference, 0);
    lv_obj_clear_flag(arc_reference, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_bg_opa(arc_reference, LV_OPA_TRANSP, 0);
    lv_obj_remove_style(arc_reference, nullptr, LV_PART_KNOB);
    lv_obj_center(arc_reference);

    if (style != nullptr) {
        lv_obj_add_style(arc_reference, this->styling()->getStyle(), LV_PART_MAIN);
    }

    if (this->is_show_label) {
        this->label = $text(text_props{
            .text = "100%",
        });

        this->label->set_parent(container);
        this->label->render();

        lv_obj_align_to(this->label->get_component(), arc_reference, LV_ALIGN_CENTER, 0, 0);
        lv_obj_move_foreground(this->label->get_component());
    }

    return container;
  }

  void CircularProgress::show_label(const bool value) const {

  };

  void CircularProgress::update(const short value) const {
    if (const auto arc = this->get_component(); arc != nullptr) {
        const short correct_value =
         value < this->props.min_dy ?
          this->props.min_dy :
          value > this->props.max_dy ?
          this->props.max_dy : value;
      lv_arc_set_value(arc, correct_value);
      if (this->is_show_label) {
          this->label->update(std::format("{}{}", value, this->props.label_symbol));
      }
    }

  };

  std::shared_ptr<Styling> CircularProgress::styling() {
    if (this->props.style != nullptr) {
        lv_style_set_arc_color(this->props.style->getStyle(), lv_color_hex(0x000000));
    }
    return this->props.style;
  }

  CircularProgress* CircularProgress::append(lv_obj_t* obj) {
    lv_obj_set_parent(obj, get_component());
    return this;
  }

} // namespace foundation
