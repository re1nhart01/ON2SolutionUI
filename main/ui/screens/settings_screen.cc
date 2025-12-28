#include "../../components/foundation/components/component.h"
#include "../../components/foundation/core/shortcuts.h"

#include "esp_log.h"
#include "ui/localization.hh"
#include "ui/styles/common_styles.h"

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


  $$View make_row(const char* prefix)
  {
    return $View(
        ViewProps::up()
            .set_children(Children{
                make_stepper(prefix),
                make_stepper(prefix),
                make_stepper(prefix),
                make_stepper(prefix),
            })
              .set_style($s("common.no_padding"))
              .direction(LV_FLEX_FLOW_ROW)
              .justify(LV_FLEX_ALIGN_SPACE_AROUND)
              .items(LV_FLEX_ALIGN_CENTER)
              .track_cross(LV_FLEX_ALIGN_CENTER)
              .w(LV_PCT(100))
              .h(110)
    );
  }

  $$View render_other_settings()
  {
    return $View(
        ViewProps::up()
            .set_style($s("header.container"))
            .set_children(Children{
                $Text(
                    TextProps::up()
                        .set_style($s("header.label"))
                        .value(locales::en::other)
                ),

                make_row("Compressor delay 1"),
                make_row("Compressor delay 2"),
                make_row("Compressor delay 3"),
                make_row("Compressor delay 4"),
            })
            .direction(LV_FLEX_FLOW_COLUMN)
            .justify(LV_FLEX_ALIGN_START)
            .items(LV_FLEX_ALIGN_CENTER)
            .track_cross(LV_FLEX_ALIGN_START)
            .w(LV_PCT(100))
    );
  }


  $$Stepper make_stepper(const char* label)
  {
      return $Stepper(
          StepperProps::up()
              .range(0, 255)
              .value(0)
              .set_step(0.5f)
              .set_precision(1)
              .btn_width(30)
              .size(150, 50)
              .set_label(label)
              .btn_width(30)
              .format(3, 0)
              .on_change([](float v) {
                  ESP_LOGI("STEPPER", "value = %.1f", v);
              })
      );
  }


  $$View render_oxygen_offset()
  {
    return $View(
        ViewProps::up()
            .set_style($s("header.container"))
            .set_children(Children{
                $Text(
                    TextProps::up()
                        .set_style($s("header.label"))
                        .value(locales::en::oxygen_offset)
                ),
                $View(
                    ViewProps::up()
                        .set_style($s("header.labels.container"))
                        .set_children(Children{
                          $Stepper(StepperProps::up()
                            .range(0, 255)
                            .value(0)
                            .set_step(0.5f)
                            .set_precision(1)
                            .size(150, 50)
                            .set_label("Compressor delay 1")
                            .btn_width(30)
                            .format(3, 0)
                            .on_change([](float v) {
                              ESP_LOGI("TAG", "tagagaga %d", v);
                            })
                          ),
                          $Stepper(StepperProps::up()
                            .range(0, 255)
                            .value(0)
                            .set_step(0.5f)
                            .set_precision(1)
                            .size(150, 50)
                            .set_label("Compressor delay 1")
                            .btn_width(30)
                            .format(3, 0)
                            .on_change([](float v) {
                              ESP_LOGI("TAG", "tagagaga %d", v);
                            })
                          ),
                          $Stepper(StepperProps::up()
                            .range(0, 255)
                            .value(0)
                            .set_step(0.5f)
                            .set_precision(1)
                            .size(150, 50)
                            .set_label("Compressor delay 1")
                            .btn_width(30)
                            .format(3, 0)
                            .on_change([](float v) {
                              ESP_LOGI("TAG", "tagagaga %d", v);
                            })
                          ),
                          $Stepper(StepperProps::up()
                            .range(0, 255)
                            .value(0)
                            .set_step(0.5f)
                            .set_precision(1)
                            .size(150, 50)
                            .set_label("Compressor delay 1")
                            .btn_width(30)
                            .format(3, 0)
                            .on_change([](float v) {
                              ESP_LOGI("TAG", "tagagaga %d", v);
                            })
                          ),
                        })
                        .set_style($s("common.no_padding"))
                        .w(LV_PCT(100))
                        .h(90)
                        .justify(LV_FLEX_ALIGN_SPACE_AROUND)
                        .items(LV_FLEX_ALIGN_CENTER)
                        .track_cross(LV_FLEX_ALIGN_CENTER)
                        .direction(LV_FLEX_FLOW_ROW)
                ),
            })
            .w(LV_PCT(100))
            .h(110)
            .justify(LV_FLEX_ALIGN_START)
            .items(LV_FLEX_ALIGN_CENTER)
            .track_cross(LV_FLEX_ALIGN_START)
            .direction(LV_FLEX_FLOW_COLUMN)
    );
  }

  $$View render_flow_offset()
  {
    return $View(
        ViewProps::up()
            .set_style($s("header.container"))
            .set_children(Children{
                $Text(
                    TextProps::up()
                        .set_style($s("header.label"))
                        .value(locales::en::flow_offset)
                ),
                $View(
                    ViewProps::up()
                        .set_style($s("header.labels.container"))
                        .set_children(Children{
                          $Stepper(StepperProps::up()
                            .range(0, 255)
                            .value(0)
                            .set_step(0.5f)
                            .set_precision(1)
                            .size(150, 50)
                            .set_label("Compressor delay 1")
                            .btn_width(30)
                            .format(3, 0)
                            .on_change([](float v) {
                              ESP_LOGI("TAG", "tagagaga %d", v);
                            })
                          ),
                          $Stepper(StepperProps::up()
                            .range(0, 255)
                            .value(0)
                            .set_step(0.5f)
                            .set_precision(1)
                            .size(150, 50)
                            .set_label("Compressor delay 1")
                            .btn_width(30)
                            .format(3, 0)
                            .on_change([](float v) {
                              ESP_LOGI("TAG", "tagagaga %d", v);
                            })
                          ),
                          $Stepper(StepperProps::up()
                            .range(0, 255)
                            .value(0)
                            .set_step(0.5f)
                            .set_precision(1)
                            .size(150, 50)
                            .set_label("Compressor delay 1")
                            .btn_width(30)
                            .format(3, 0)
                            .on_change([](float v) {
                              ESP_LOGI("TAG", "tagagaga %d", v);
                            })
                          ),
                          $Stepper(StepperProps::up()
                            .range(0, 255)
                            .value(0)
                            .set_step(0.5f)
                            .set_precision(1)
                            .size(150, 50)
                            .set_label("Compressor delay 1")
                            .btn_width(30)
                            .format(3, 0)
                            .on_change([](float v) {
                              ESP_LOGI("TAG", "tagagaga %d", v);
                            })
                          ),
                        })
                        .set_style($s("common.no_padding"))
                        .w(LV_PCT(100))
                        .h(90)
                        .justify(LV_FLEX_ALIGN_SPACE_AROUND)
                        .items(LV_FLEX_ALIGN_CENTER)
                        .track_cross(LV_FLEX_ALIGN_CENTER)
                        .direction(LV_FLEX_FLOW_ROW)
                ),
            })
            .w(LV_PCT(100))
            .h(110)
            .justify(LV_FLEX_ALIGN_START)
            .items(LV_FLEX_ALIGN_CENTER)
            .track_cross(LV_FLEX_ALIGN_START)
            .direction(LV_FLEX_FLOW_COLUMN)
    );
  }


  lv_obj_t* render() override
    {
        auto navigator_ref = this->navigation_ref;

         return this->delegate(
           $ScrollView(
            ScrollViewProps::up()
                .set_style(this->styling())
                .set_children(Children{
                    $StatusBar(StatusBarProps::up()),
                  $View(ViewProps::up()
                  .set_children(Children{
                    $View(ViewProps::up().set_children(Children{
                      $Button(ButtonProps::up()
                       .set_child(
                         $Text(
                             TextProps::up()
                                 .value("Back")
                         )
                       )
                       .click([navigator_ref](lv_event_t* e){
                         navigator_ref->reset_to("/main");
                       })),
                    })
                    .set_style($s("common.no_padding"))
                    .w(LV_PCT(20))
                    .direction(LV_FLEX_FLOW_ROW)
                    .items(LV_FLEX_ALIGN_CENTER)
                    .justify(LV_FLEX_ALIGN_START)
                  ),
                    $Text(TextProps::up().value(locales::en::header_settings)),
                    $View(ViewProps::up().set_children(Children{$Fragment(FragmentProps::up())}).w(LV_PCT(20)).set_style($s("common.no_padding"))),
                  })
                .set_style($s("header.container"))
                .merge(header_container_props)),
                  render_oxygen_offset(),
                  render_flow_offset(),
                  render_other_settings(),
                })
            .scrollbar(LV_SCROLLBAR_MODE_OFF)
            .w(LV_PCT(100))
        ));
    }



  $$Styling styling() override {
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
