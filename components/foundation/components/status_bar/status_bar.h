#pragma once

#include "status_bar_props.h"
#include "components/view/view.h"
#include "core/shortcuts.h"

namespace foundation
{
  class StatusBar final : public Component<StatusBarProps> {
  public:
    using Component::props;

    explicit StatusBar(const StatusBarProps& props) : Component(nullptr, nullptr, std::move(props)) {
      this->parent = nullptr;

      if (this->props.ref != nullptr) {
          this->props.ref->set(this);
      }
    };

    ~StatusBar() override
    {
      if (this->props.ref != nullptr) {
          this->props.ref->unlink();
      }
    };

    lv_obj_t * render() override
    {
      std::shared_ptr<Styling> styleV = this->styling();

      return this->delegate(
        std::make_shared<View>(
          this->parent,
          ViewProps::up()
          .set_style(styleV)
          .set_children(this->props.children)
          .w(LV_PCT(100))
          .h(this->props.height.value_or(24))
          .justify(LV_FLEX_ALIGN_SPACE_BETWEEN)
          .items(LV_FLEX_ALIGN_CENTER)
          .track_cross(LV_FLEX_ALIGN_CENTER)
          .direction(LV_FLEX_FLOW_ROW)
        )
      );
    };

    std::shared_ptr<Styling> styling() override
    {
      this->style = std::make_shared<Styling>();

      this->style->setBackgroundColor(this->props.background_color.value_or(lv_color_hex(0x303030)));

      this->style->setTextColor(lv_color_make(255, 255, 255));
      this->style->setPadding(0, 0, 16, 16); // Padding T R B L
      this->style->setBorder(lv_color_make(0, 0, 0), 0, 0);
      this->style->setFont(&lv_font_montserrat_12);
      this->style->setBorderRadius(0);

      return this->style;
    };

    StatusBar* append(lv_obj_t* obj) override
    {
      lv_obj_set_parent(obj, get_component());
      return this;
    };
  };
}