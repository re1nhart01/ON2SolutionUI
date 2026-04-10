#pragma once
#include "circular_progress_props.h"

namespace foundation {
  class CircularProgress final : public Component<CircularProgressProps> {
   private:
    using Component::props;
    std::unique_ptr<Text> label = nullptr;
    lv_obj_t* arc_reference = nullptr;
    mutable Styling arc_style{};
    bool is_show_label = false;

   public:
    explicit CircularProgress(CircularProgressProps&& props)
        : Component(nullptr, nullptr, std::move(props)) {
      this->apply_reactive<CircularProgress>(this,
                                             this->props.reactive_delegates);

      this->is_show_label = props.show_label_default;

      if (this->props.ref != nullptr) {
        this->props.ref->set(this);
      }
    };

    ~CircularProgress() override {
      if (!this->props.reactive_link.empty()) {
        this->detach_reactives<CircularProgress>(this,
                                                 this->props.reactive_link);
      }
      if (this->props.ref != nullptr) {
        this->props.ref->unlink();
      }
    };

    lv_obj_t* render() override {
      Component::render();
      if (this->get_component() != nullptr) {
        this->do_rebuild();
        return this->get_component();
      }

      lv_obj_t* parent_obj = this->get_parent();
      if (!parent_obj)
        return nullptr;

      lv_obj_t* container = lv_obj_create(parent_obj);
      this->set_component(container);

      lv_obj_clear_flag(container, LV_OBJ_FLAG_SCROLLABLE);
      lv_obj_set_style_bg_opa(container, LV_OPA_TRANSP, LV_PART_MAIN);
      lv_obj_set_style_border_width(container, 0, LV_PART_MAIN);

      this->arc_reference = lv_arc_create(container);

      this->is_show_label = this->props.show_label_default;

      if (props.style_override) {
        props.style_override(style);
        lv_obj_add_style(arc_reference, style.getStyle(), LV_PART_MAIN);
      }

      if (props.arc_style_override) {
        props.arc_style_override(arc_style);
        lv_obj_add_style(arc_reference, arc_style.getStyle(),
                         LV_PART_INDICATOR);
      }

      if (props.show_label_default) {
        if (!this->label) {
          this->label = std::make_unique<Text>(
              TextProps::up()
                  .set_style(this->props.text_style_override)
                  .value(std::format("{:.{}f}{}", props.current_value,
                                     props.precision, props.label_symbol)));
          this->label->set_parent(container);
          this->label->render();
        }

        this->label->update(
            std::format("{}{}", props.current_value, props.label_symbol));

        lv_obj_center(this->label->get_component());
        lv_obj_move_foreground(this->label->get_component());
      } else {
        if (this->label) {
          lv_obj_add_flag(this->label->get_component(), LV_OBJ_FLAG_HIDDEN);
        }
      }

      this->do_rebuild();

      return container;
    }

    void do_rebuild() override {
      const auto container = this->get_component();
      if (!container)
        return;

      lv_obj_set_size(container, props.width, props.height);

      lv_obj_set_size(arc_reference, props.width, props.height);
      lv_arc_set_range(arc_reference, props.min_dy, props.max_dy);
      lv_arc_set_value(arc_reference,
                       static_cast<int32_t>(props.current_value));
      lv_arc_set_bg_angles(arc_reference, 135, 45);
      lv_arc_set_rotation(arc_reference, 0);

      lv_obj_clear_flag(arc_reference, LV_OBJ_FLAG_CLICKABLE);
      lv_obj_set_style_bg_opa(arc_reference, LV_OPA_TRANSP, LV_PART_MAIN);
      lv_obj_remove_style(arc_reference, nullptr, LV_PART_KNOB);

      if (auto style = styling(); style->get_is_dirty()) {
        lv_obj_invalidate(container);
      }

      if (props.arc_style_override) {
        props.arc_style_override(arc_style);
      }

      lv_obj_refresh_style(arc_reference, LV_PART_INDICATOR, LV_STYLE_PROP_ANY);
      lv_obj_refresh_style(arc_reference, LV_PART_MAIN, LV_STYLE_PROP_ANY);

      lv_obj_center(arc_reference);

      this->update_label_text();
    }

    const Styling* styling() const override {
      apply_base_style(style);

      if (props.style_override) {
        props.style_override(style);
      }

      return &style;
    }

    CircularProgress* append(lv_obj_t* obj) override {
      lv_obj_set_parent(obj, get_component());
      return this;
    };

    void update_label_text() const {
      if (!label)
        return;
      auto* obj = label->get_component();
      if (!obj)
        return;

      std::string text = std::format("{:.{}f}{}", props.current_value,
                                     props.precision, props.label_symbol);

      lv_label_set_text(obj, text.c_str());
    }
  };
}  // namespace foundation