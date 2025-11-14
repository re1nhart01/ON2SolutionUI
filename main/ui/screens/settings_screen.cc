#include "../../components/foundation/components/component.h"
#include "../../components/foundation/core/macro.h"


extern "C" {
  #include "esp_log.h"
}

using namespace foundation;

struct settings_screen_props
{
  std::shared_ptr<Ref> ref = nullptr;
};

std::unique_ptr<KeyboardManager> admin_keyboard = std::make_unique<KeyboardManager>();
std::shared_ptr<Styling> imageStyle = std::make_shared<Styling>();

class SettingsScreen : public Component
{
  settings_screen_props props;
  std::shared_ptr<StackNavigator> navigator;

public:
  explicit SettingsScreen(std::shared_ptr<StackNavigator> stack,
                       const settings_screen_props &props)
      : Component(nullptr, nullptr)
  {
    this->props = props;
    this->navigator = std::move(stack);
    if(this->props.ref != nullptr)
      {
        this->props.ref->set(this);
      }
  }

  ~SettingsScreen() override {
    Component::~Component();
  };

  void component_did_mount() override {
  };


  lv_obj_t* render() override
    {
        auto navigator_ref = this->navigator;

        // Styles
        auto style = this->styling();
        auto text_style = std::make_shared<Styling>();
        auto btn_style = std::make_shared<Styling>();
        auto input_style = std::make_shared<Styling>();

         return this->delegate($View(
            ViewProps::up()
                .set_style(style)
                .set_children({
                    $StatusBar(
                        StatusBarProps::up()
                            .set_style(nullptr)
                    ),
                    $Text(
                        TextProps::up()
                            .set_style(text_style)
                            .value("text")
                    ),
                    $Button(
                        ButtonProps::up()
                            .set_style(btn_style)
                            .label("navigate to admin")
                            .click([navigator_ref](lv_event_t* e){
                                navigator_ref->navigate("/main");
                            })
                    ),
                    $Input(
                        TextInputProps::up()
                            .set_style(input_style)
                            .placeholder("text")
                            .set_on_submit([](const std::string& value){
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
