//
// Created by evgeniy on 12/21/25.
//

#pragma once
#include "modal_props.h"

namespace foundation
{
  class Modal final : public Component<ModalProps> {
    lv_obj_t* overlay = nullptr;

  public:
    using Component::props;

    explicit Modal(const ModalProps& props)
        : Component(nullptr, nullptr, props) {}

    ~Modal() override {
      ESP_LOGE("MODAL", "Modal destroyed");
      close();
    }

    lv_obj_t* render() override
    {
      overlay = lv_obj_create(lv_scr_act());
      lv_obj_set_size(overlay, LV_PCT(100), LV_PCT(100));
      lv_obj_set_style_bg_color(overlay, lv_color_black(), 0);
      lv_obj_set_style_bg_opa(overlay, LV_OPA_50, 0);
      lv_obj_add_flag(overlay, LV_OBJ_FLAG_CLICKABLE);
      lv_obj_set_style_border_width(overlay, 0, 0);
      lv_obj_set_style_radius(overlay, 0, 0);
      lv_obj_set_style_pad_all(overlay, 0, 0);

      lv_obj_set_layout(overlay, LV_LAYOUT_FLEX);
      lv_obj_set_flex_flow(overlay, LV_FLEX_FLOW_COLUMN);
      lv_obj_set_flex_align(
          overlay,
          LV_FLEX_ALIGN_CENTER,
          LV_FLEX_ALIGN_CENTER,
          LV_FLEX_ALIGN_CENTER
      );

      if (props.content) {
          props.content->set_parent(overlay);
          props.content->render();
      }

      if (props.close_on_overlay) {
          lv_obj_add_event_cb(
              overlay,
              [](lv_event_t* e) {
                  auto* self = static_cast<Modal*>(lv_event_get_user_data(e));
                  self->close();
              },
              LV_EVENT_CLICKED,
              this
          );
      }

      set_component(overlay);
      return overlay;
    }

    void show()
    {
      close();
      set_parent(lv_scr_act());
      render();
    }

    void close()
    {
      if (overlay) {
          lv_obj_del(overlay);
          overlay = nullptr;
          component = nullptr;
      }
    }

    std::shared_ptr<Styling> styling() override { return {}; }
    VNode* append(lv_obj_t*) override { return this; }
  };

}
