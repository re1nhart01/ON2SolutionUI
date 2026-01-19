#pragma once

#include "core/styling/styling.h"
#include "meter_props.h"

namespace foundation
{
  class Meter final : public Component<MeterProps> {
    lv_meter_indicator_t* indic = nullptr;
    lv_obj_t* label_obj = nullptr;

  public:
    explicit Meter(MeterProps&& props)
      : Component(nullptr, nullptr, std::move(props)) {
      this->apply_reactive<Meter>(this, this->props.reactive_delegates);
    };

    ~Meter() override {
      if (this->props.ref != nullptr) this->props.ref->unlink();
      if (!this->props.reactive_link.empty()) {
        this->detach_reactives<Meter>(this, this->props.reactive_link);
      }
    };

    lv_obj_t* render() override
    {
      Component::render();
      lv_obj_t* parent_obj = this->get_parent();
      if (!parent_obj) return nullptr;

      set_component(lv_meter_create(parent_obj));
      lv_obj_t* obj = this->get_component();

      lv_obj_set_size(obj, props.width, props.height);
      lv_obj_set_style_pad_all(obj, 0, 0);

      lv_meter_scale_t* scale = lv_meter_add_scale(obj);
      lv_meter_set_scale_ticks(obj, scale, 41, 2, 10, lv_palette_main(LV_PALETTE_GREY));
      lv_meter_set_scale_major_ticks(obj, scale, 8, 4, 15, lv_color_black(), 10);

      lv_meter_indicator_t* static_indic = lv_meter_add_arc(obj, scale, 3, lv_palette_main(LV_PALETTE_BLUE), 0);
      lv_meter_set_indicator_start_value(obj, static_indic, 0);
      lv_meter_set_indicator_end_value(obj, static_indic, 20);

      static_indic = lv_meter_add_arc(obj, scale, 3, lv_palette_main(LV_PALETTE_RED), 0);
      lv_meter_set_indicator_start_value(obj, static_indic, 80);
      lv_meter_set_indicator_end_value(obj, static_indic, 100);

      indic = lv_meter_add_needle_line(obj, scale, 4, lv_palette_main(LV_PALETTE_GREY), -10);
      lv_meter_set_indicator_value(obj, indic, static_cast<int>(props.current_v));

      if (props.show_label_default) {
        label_obj = lv_label_create(obj);
        lv_obj_align(label_obj, LV_ALIGN_CENTER, 0, props.height / 3);
        update_label_text();
      }

      return obj;
    };

    void do_rebuild() override
    {
      lv_obj_t* obj = this->get_component();
      if (!obj || !indic) return;

      lv_meter_set_indicator_value(obj, indic, static_cast<int>(props.current_v));

      if (auto style = styling(); style->get_is_dirty()) {
        lv_obj_invalidate(obj);
      }

      if (label_obj) {
        update_label_text();
      }
    };

    void update_label_text() const {
      if (!label_obj) return;
      char buf[32];

      snprintf(buf, sizeof(buf), "%.1f%s", props.current_v, props.label_symbol.c_str());
      lv_label_set_text(label_obj, buf);
    }

    const Styling* styling() const override {
      apply_base_style(style);
      if (props.style_override) props.style_override(style);
      return &style;
    }

    Meter* append(lv_obj_t* obj) override {
      lv_obj_set_parent(obj, get_component());
      return this;
    };
  };
}