#include <utility>

#include "../../components/foundation/components/component.h"
#include "../../components/foundation/core/macro.h"

namespace foundation
{
  class StackNavigator;
}

using namespace foundation;

struct pincode_screen_props
{
  std::shared_ptr<foundation::Ref> ref = nullptr;
};



class PinCodeScreen : public foundation::Component
{
  pincode_screen_props props;
  std::shared_ptr<StackNavigator> navigator;

public:
  explicit PinCodeScreen(std::shared_ptr<StackNavigator> stack,
                      const pincode_screen_props &props)
      : Component(nullptr, nullptr)
  {
    this->props = props;
    this->navigator = std::move(stack);
    if(this->props.ref != nullptr)
      {
        this->props.ref->set(this);
      }
  }

  ~PinCodeScreen() override = default;

  lv_obj_t *render() override {
    return this->delegate(
        $View(
            ViewProps::up()
                .set_style(this->styling())
                .set_children({
                    $StatusBar(
                        StatusBarProps::up()
                            .set_style(nullptr)
                    )
                })
                .w(LV_PCT(100))
                .h(LV_PCT(100))
                .justify(LV_FLEX_ALIGN_START)
                .items(LV_FLEX_ALIGN_CENTER)
                .track_cross(LV_FLEX_ALIGN_CENTER)
                .direction(LV_FLEX_FLOW_COLUMN)
        )
    );
  }



  std::shared_ptr<Styling> styling() override {
    this->style = std::make_shared<Styling>();

    this->style->setTextColor(lv_color_make(255, 255, 255));
    this->style->setPadding(0, 0, 0, 0);
    this->style->setBorderRadius(0);
    this->style->setBorder(lv_color_make(255, 255, 255), 0, 0);

    return this->style;
  }

  PinCodeScreen* append(lv_obj_t *obj) override
  {
    lv_obj_set_parent(obj, get_component());
    return this;
  }
};
