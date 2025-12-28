#include "../../components/foundation/components/component.h"
#include "../../components/foundation/core/shortcuts.h"

#include "esp_log.h"
#include "ui/localization.hh"
#include "ui/styles/common_styles.h"

class SettingsScreen;

using namespace foundation;

struct SettingsScreenProps final
    : BaseProps<SettingsScreenProps, SettingsScreen>
{};

class SettingsScreen final : public NavigationScreen<SettingsScreenProps>
{
  SettingsScreenProps props;
  std::unique_ptr<StyleStorage> styles;

public:
  explicit SettingsScreen(StackNavigator *stack,
                          const SettingsScreenProps &props)
      : NavigationScreen(stack, props),
        styles(std::make_unique<StyleStorage>())
  {
    style_screen_register(*this->styles);
  }

  ~SettingsScreen() override = default;

  void component_did_mount() override
  {
    NavigationScreen::component_did_mount();
  };

  $$Stepper
  make_param(const char *label, float val, float step = 1.0f, int prec = 0)
  {
    return $Stepper(StepperProps::up()
                      .set_label(label)
                      .value(val)
                      .set_step(step)
                      .set_precision(prec)
                      .size(155, 55)
                      .btn_width(35));
  }

  $$View render_sensors_tab()
  {
    return $View(
      ViewProps::up()
        .direction(LV_FLEX_FLOW_ROW_WRAP)
        .justify(LV_FLEX_ALIGN_CENTER)
        .set_children(Children{
          make_param("Oxy-A", 94.4, 0.1, 1), make_param("Oxy-B", 94.4, 0.1, 1),
          make_param("Oxy-C", 94.4, 0.1, 1), make_param("Oxy-D", 94.4, 0.1, 1),
          make_param("Flow-A", 9.6, 0.1, 1), make_param("Flow-B", 9.7, 0.1, 1),
          make_param("Flow-C", 9.8, 0.1, 1),
          make_param("Flow-D", 9.9, 0.1, 1)})
        .w(LV_PCT(100))
        .h(LV_SIZE_CONTENT));
  }

  $$View render_timers_tab()
  {
    return $View(ViewProps::up()
                   .direction(LV_FLEX_FLOW_ROW_WRAP)
                   .justify(LV_FLEX_ALIGN_CENTER)
                   .set_children(Children{make_param("Compressor delay", 3),
                                          make_param("RunUp time", 480),
                                          make_param("Prestart time", 40),
                                          make_param("Low limit time", 30),
                                          make_param("Error to alarm", 4)})
                   .w(LV_PCT(100))
                   .h(LV_SIZE_CONTENT));
  }

  $$View render_limits_tab()
  {
    return $View(
      ViewProps::up()
        .direction(LV_FLEX_FLOW_ROW_WRAP)
        .justify(LV_FLEX_ALIGN_CENTER)
        .set_children(Children{make_param("Low oxy conc.%", 88.6, 0.1, 1),
                               make_param("Tank high press", 60),
                               make_param("Tank low press", 50),
                               make_param("Temp.C overheat", 70),
                               make_param("Flow Limit ERR", 1.2, 0.1, 1)})
        .w(LV_PCT(100))
        .h(LV_SIZE_CONTENT));
  }

  $$View render_service_tab()
  {
    return $View(
      ViewProps::up()
        .direction(LV_FLEX_FLOW_ROW_WRAP)
        .justify(LV_FLEX_ALIGN_CENTER)
        .set_children(Children{
          make_param("SPV ON time", 7.0, 0.1, 1),
          make_param("SPV OFF time", 5.0, 0.1, 1),
          make_param("Calibrate cycle", 2),
          $Button(ButtonProps::up()
                    .set_child($Text(TextProps::up().value("Hour Run Reset")))
                    .click([](lv_event_t *e) { /* Reset logic */ }))})
        .w(LV_PCT(100))
        .h(LV_SIZE_CONTENT));
  }

public:
  lv_obj_t *render() override
  {
    auto navigation_ref = this->navigation_ref;

    return this->delegate($View(
      ViewProps::up()
        .w(LV_PCT(100))
        .h(LV_PCT(100))
        .direction(LV_FLEX_FLOW_COLUMN)
        .set_children(Children{
          $View(ViewProps::up()
                  .h(60)
                  .direction(LV_FLEX_FLOW_ROW)
                  .items(LV_FLEX_ALIGN_CENTER)
                  .justify(LV_FLEX_ALIGN_SPACE_BETWEEN)
                  .set_children(Children{
                    $View(ViewProps::up()
                            .w(LV_PCT(20))
                            .h(45)
                            .direction(LV_FLEX_FLOW_ROW)
                            .items(LV_FLEX_ALIGN_CENTER)
                            .justify(LV_FLEX_ALIGN_START)
                            .set_style($s("common.no_padding"))
                            .set_children(Children{
                              $Button(ButtonProps::up()
                                        .set_child(
                                          $Text(TextProps::up().value("Back")))
                                        .click([navigation_ref](lv_event_t *e) {
                                            navigation_ref->reset_to("/main");
                                        })),
                            })),
                    $View(ViewProps::up()
                      .w(LV_PCT(78))
                      .set_style($s("common.no_padding"))
                      .direction(LV_FLEX_FLOW_ROW)
                      .items(LV_FLEX_ALIGN_CENTER)
                      .justify(LV_FLEX_ALIGN_CENTER)
                      .track_cross(LV_FLEX_ALIGN_CENTER)
                      .set_children(Children{
                        $Text(TextProps::up().value("SYSTEM SETTINGS"))
                    })),
                    $View(ViewProps::up()
                            .w(LV_PCT(20))
                            .set_style($s("common.no_padding"))
                            .set_children(
                              Children{$Fragment(FragmentProps::up())})),
                  })),

          $TabView(
            TabViewProps::up()
              .set_header_size(45)
              .set_tabs({"Sensors", "Timers", "Limits", "Service"})
              .set_children({render_sensors_tab(), render_timers_tab(),
                             render_limits_tab(), render_service_tab()}))})));
  }

  $$Styling styling() override
  {
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
