#include "../../components/foundation/components/component.h"
#include "../../components/foundation/core/shortcuts.h"

class SettingsScreen;
extern "C" {
  #include "esp_log.h"
}

using namespace foundation;

struct SettingsScreenProps final
    : BaseProps<SettingsScreenProps, SettingsScreen>
{};

auto admin_keyboard
  = std::make_unique<KeyboardManager>();
auto imageStyle = std::make_shared<Styling>();

class SettingsScreen final : public NavigationScreen<SettingsScreenProps>
{
  SettingsScreenProps props;

public:
  explicit SettingsScreen(const std::shared_ptr<StackNavigator> &stack, const SettingsScreenProps &props) : NavigationScreen(stack, props)
  {
    this->props = props;
  }

  ~SettingsScreen() override = default;

  void component_did_mount() override {
    NavigationScreen::component_did_mount();
  };


  lv_obj_t* render() override
    {
        auto navigator_ref = this->navigation_ref;

         return this->delegate($View(
            ViewProps::up()
                .set_style(style)
                .set_children(Children{
                    $StatusBar(
                        StatusBarProps::up()
                            .set_style(nullptr)
                    ),
                    $Text(
                        TextProps::up()
                            .value("text")
                    ),
                    $Button(
                        ButtonProps::up()
                            .label("navigate to admin")
                            .click([navigator_ref](lv_event_t* e){
                                navigator_ref->navigate("/main");
                            })
                    ),
                    $Input(
                        TextInputProps::up()
                            .hint("text")
                            .submit([](const std::string& value){
                                ESP_LOGI("LOG", "%s", value.c_str());
                                admin_keyboard->hide();
                            }),
                        admin_keyboard.get()
                    ),
                })
                .w(LV_PCT(100))
                .h(LV_PCT(100))
                .justify(LV_FLEX_ALIGN_SPACE_BETWEEN)
                .items(LV_FLEX_ALIGN_CENTER)
                .track_cross(LV_FLEX_ALIGN_CENTER)
                .direction(LV_FLEX_FLOW_COLUMN)
        ));
    }



  std::shared_ptr<Styling> styling() override {
    this->style = std::make_shared<Styling>();

    this->style->setTextColor(lv_color_make(255, 255, 255));
    this->style->setPadding(0, 0, 0, 0);
    this->style->setBorderRadius(0);
    this->style->setBorder(lv_color_make(255, 255, 255), 0, 0);

    return this->style;
  }

  SettingsScreen *append(lv_obj_t *obj) override
  {
    lv_obj_set_parent(obj, get_component());
    return this;
  }
};
