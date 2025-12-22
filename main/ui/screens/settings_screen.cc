#include "../../components/foundation/components/component.h"
#include "../../components/foundation/core/shortcuts.h"

#include "ui/styles/common_styles.h"
#include "esp_log.h"


class SettingsScreen;

using namespace foundation;

struct SettingsScreenProps final : BaseProps<SettingsScreenProps, SettingsScreen> {};

class SettingsScreen final : public NavigationScreen<SettingsScreenProps>
{
  SettingsScreenProps props;
  std::unique_ptr<StyleStorage> styles;

public:
  explicit SettingsScreen(StackNavigator* stack, const SettingsScreenProps &props)
  : NavigationScreen(stack, props),
    styles(std::make_unique<StyleStorage>())
  {
    style_screen_register(*this->styles);
  }

  ~SettingsScreen() override = default;

  void component_did_mount() override {
    NavigationScreen::component_did_mount();
  };


  lv_obj_t* render() override
    {
        auto navigator_ref = this->navigation_ref;

         return this->delegate(
           $View(
            ViewProps::up()
                .set_style(this->styling())
                .set_children(Children{
                    $StatusBar(
                        StatusBarProps::up()
                            .set_style(nullptr)
                    ),
                  $View(ViewProps::up()
                  .set_children(Children{
                    $Button(ButtonProps::up()
                     .set_child(
                       $Text(
                           TextProps::up()
                               .value("Back")
                       )
                     )
                     .click([navigator_ref](lv_event_t* e){
                       navigator_ref->navigate("/main");
                     }))
                  })
                .set_style($s("header.container"))
                .merge(header_container_left_props)),
                  $View(ViewProps::up()),
                  $View(ViewProps::up()),
                })
            .merge(screen_container_props)
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
