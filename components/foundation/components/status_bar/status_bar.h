#pragma once

#include "status_bar_props.h"
#include "components/view/view.h"
#include "core/shortcuts.h"

namespace foundation
{
  class StatusBar final : public Component<StatusBarProps> {
  public:
    using Component::props;

    explicit StatusBar(StatusBarProps&& props) : Component(nullptr, nullptr, std::move(props)) {
      this->apply_reactive<StatusBar>(this, this->props.reactive_delegates);
      if (this->props.ref != nullptr) {
          this->props.ref->set(this);
      }
    };

    ~StatusBar() override
    {
      if (!this->props.reactive_link.empty())
      {
        this->detach_reactives<StatusBar>(this, this->props.reactive_link);
      }
      if (this->props.ref != nullptr) {
          this->props.ref->unlink();
      }
    };

    template <typename... Nodes>
    std::vector<std::unique_ptr<foundation::VNode>> children(Nodes&&... nodes)
    {
      std::vector<std::unique_ptr<foundation::VNode>> v;
      v.reserve(sizeof...(Nodes));
      (v.push_back(std::move(nodes)), ...);
      return v;
    }

    lv_obj_t * render() override
    {
      Component::render();
      return this->delegate(
        std::make_unique<View>(
        std::move(ViewProps::up()
         .set_style([this](Styling& style) {
           style.setBackgroundColor(this->props.background_color.value_or(lv_color_hex(0x303030)));
           style.setTextColor(lv_color_make(255, 255, 255));
           style.setPadding(0, 0, 16, 16);
           style.setBorder(lv_color_make(0, 0, 0), 0, 0);
           style.setFont(&lv_font_montserrat_12);
           style.setBorderRadius(0);
         })
         .set_children(std::move(this->props.children))
         .w(LV_PCT(100))
         .h(this->props.height.value_or(24))
         .justify(LV_FLEX_ALIGN_SPACE_BETWEEN)
         .items(LV_FLEX_ALIGN_CENTER)
         .track_cross(LV_FLEX_ALIGN_CENTER)
         .direction(LV_FLEX_FLOW_ROW)
       ))
      );
    };

    const Styling* styling() const override
    {
      apply_base_style(style);

      if (props.style_override) {
          props.style_override(style);
      }

      return &style;
    }

    StatusBar* append(lv_obj_t* obj) override
    {
      lv_obj_set_parent(obj, get_component());
      return this;
    };
  };
}