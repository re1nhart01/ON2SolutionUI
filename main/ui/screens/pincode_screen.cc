#include <utility>

#include "../../components/foundation/components/component.h"
#include "../../components/foundation/core/shortcuts.h"


using namespace foundation;

class PinCodeScreen;
struct PinCodeScreenProps : BaseProps<PinCodeScreenProps, PinCodeScreen>{};

class PinCodeScreen : public Component<PinCodeScreenProps>
{
  PinCodeScreenProps props;
  std::shared_ptr<StackNavigator> navigator;

public:
  explicit PinCodeScreen(std::shared_ptr<StackNavigator> stack,
                      const PinCodeScreenProps &props)
      : Component(nullptr, nullptr, props)
  {
    this->props = props;
    this->navigator = std::move(stack);
  }

  ~PinCodeScreen() override = default;

  lv_obj_t *render() override {
    return this->delegate(
        $View(
            ViewProps::up()
                .set_style(this->styling())
                .set_children(Children{
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
