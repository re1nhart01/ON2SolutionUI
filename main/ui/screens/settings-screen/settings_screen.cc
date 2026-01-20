#include "settings_screen.h";

#include <lg/dataset/store/dataset.store.h>
#include <lg/helpers/utils.h>

namespace ON2Solutions {

  void SettingsScreen::on_focus() {
    this->uart_handler = std::make_unique<UartHandler>(
        UART_NUM_2, GPIO_NUM_43, GPIO_NUM_44, 9600, 16384);
    // this->uart_handler->init();
    // this->uart_handler->enable_rx(true);
  }

  template <typename T>
  void SettingsScreen::update_param(const ParamSpec& spec, T value, T min,
                                    T max) const {
    auto command = parser::SerializableCommand<T>{
        .command = spec.command,
        .data = value,
        .num_sensor = spec.num_sensor,
        .min = min,
        .max = max,
        .with_num_sensor = spec.with_num_sensor,
    };
    std::string serialized = parser::serialize(command);
    auto status = this->uart_handler->send(serialized);
  }

  void SettingsScreen::update_param(const ParamSpec& spec,
                                    const char* option) const {
    auto command = parser::SerializableCommand<const char*>{
        .command = spec.command,
        .data = option,
    };
    std::string serialized = parser::serialize(command);

    ESP_LOGI("SettingsScreen", "Updating settings: %s", serialized.c_str());

    auto status = this->uart_handler->send(serialized);
  }

  void SettingsScreen::hour_run_reset() const {
    auto command = parser::SerializableCommand<const char*>{
        .command = parser::SendableCommands::ResetMotoCommand,
    };

    std::string serialized = parser::serialize(command);
    auto status = this->uart_handler->send(serialized);
  }

  template void SettingsScreen::update_param<float>(const ParamSpec& spec,
                                                    float value, float min,
                                                    float max) const;

  template void SettingsScreen::update_param<uint16_t>(const ParamSpec& spec,
                                                       uint16_t value,
                                                       uint16_t min,
                                                       uint16_t max) const;

  template void SettingsScreen::update_param<uint8_t>(const ParamSpec& spec,
                                                      uint8_t value,
                                                      uint8_t min,
                                                      uint8_t max) const;

  void SettingsScreen::on_blur() {
    NavigationScreen::on_blur();
    this->uart_handler->remove_all_event_listeners();
  };

  $$View SettingsScreen::render_section_header(
      const std::string& heading) const {

    return $View(
        ViewProps::up()
            .w(LV_PCT(100))
            .h(40)
            .set_style(NoPaddingApply)
            .set_children(children($Text(TextProps::up().value(heading))))
            .merge(centered_row));
  }

  template <typename T>
  $$Stepper SettingsScreen::make_param(const ParamSpec& spec, T value,
                                       float dependable, short width) const {
    auto [min, max] = calculate_dynamic_range(spec, dependable);
    return $Stepper(
        StepperProps::up()
            .set_label(spec.label)
            .value(value)
            .set_step(spec.step)
            .set_precision(spec.precision)
            .range(min, max)
            .template watch<parser::Dataset>(
                parser::DatasetStore::getInstance(),
                fmt_str("param_%s", spec.label),
                [spec](Stepper* self, const parser::Dataset& dataset) {
                  auto [min, max] = calculate_dynamic_range(spec, dataset);
                  self->set_state([min, max](StepperProps& props) {
                    props.range(min, max);
                  });
                })
            .size(width, 55)
            .on_change([this, spec, min, max](const float v) {
              this->update_param<T>(
                  spec, static_cast<T>(std::clamp(v, min, max)), min, max);
            })
            .btn_width(45));
  }

  $$View SettingsScreen::render_sensors_tab() const {
    parser::DatasetSettings dataset =
        parser::DatasetStore::getInstance()->get().settings;
    return $View(
        ViewProps::up()
            .direction(LV_FLEX_FLOW_ROW_WRAP)
            .justify(LV_FLEX_ALIGN_CENTER)
            .set_children(children(
                render_section_header(locales::en::oxygen_offset_setting),
                make_param(OxygenShiftASpec, dataset.oxygen_sensor_offset[0]),
                make_param(OxygenShiftBSpec, dataset.oxygen_sensor_offset[1]),
                make_param(OxygenShiftCSpec, dataset.oxygen_sensor_offset[2]),
                make_param(OxygenShiftDSpec, dataset.oxygen_sensor_offset[3]),
                render_section_header(locales::en::oxygen_flow_setting),
                make_param(FlowShiftASpec, dataset.flow_sensor_offset[0]),
                make_param(FlowShiftBSpec, dataset.flow_sensor_offset[1]),
                make_param(FlowShiftCSpec, dataset.flow_sensor_offset[2]),
                make_param(FlowShiftDSpec, dataset.flow_sensor_offset[3])))
            .w(LV_PCT(100))
            .h(LV_SIZE_CONTENT));
  }

  $$View SettingsScreen::render_timers_tab() const {
    parser::DatasetSettings dataset =
        parser::DatasetStore::getInstance()->get().settings;

    return $View(
        ViewProps::up()
            .direction(LV_FLEX_FLOW_ROW_WRAP)
            .justify(LV_FLEX_ALIGN_CENTER)
            .set_children(children(
                make_param(CompressorDelaySpec, dataset.compressor_delay_sec),
                make_param(RunUpTimeSpec, dataset.run_up_delay_sec),
                make_param(PreStartTimeSpec, dataset.prestart_time_sec,
                           dataset.run_up_delay_sec),
                make_param(LowLimitTimeSpec,
                           dataset.low_limit_time_to_error_sec, 0)))
            .w(LV_PCT(100))
            .h(LV_SIZE_CONTENT));
  }

  $$View SettingsScreen::render_limits_tab() const {
    parser::DatasetSettings dataset =
        parser::DatasetStore::getInstance()->get().settings;

    return $View(
        ViewProps::up()
            .direction(LV_FLEX_FLOW_ROW_WRAP)
            .justify(LV_FLEX_ALIGN_CENTER)
            .set_children(children(
                make_param(WorkConcentrationSpec,
                           dataset.work_oxygen_concentration, 0),
                make_param(LowLimitConcentrationSpec,
                           dataset.low_limit_oxygen_concentration,
                           dataset.work_oxygen_concentration),
                make_param(TankHighLimitSpec, dataset.tank_high_pressure,
                           dataset.tank_low_pressure),
                make_param(TankLowLimitSpec, dataset.tank_low_pressure,
                           dataset.tank_high_pressure),
                make_param(TempOverheatSpec, dataset.temperature_overheat_alarm,
                           0),
                make_param(FlowErrorSpec, dataset.flow_low_limit_to_error, 0),
                make_param(ErrorCountSpec, dataset.error_to_alarm_count, 0)))
            .w(LV_PCT(100))
            .h(LV_SIZE_CONTENT));
  }

  $$View SettingsScreen::render_service_tab() const {
    parser::DatasetSettings dataset =
        parser::DatasetStore::getInstance()->get().settings;

    return $View(
        ViewProps::up()
            .direction(LV_FLEX_FLOW_ROW_WRAP)
            .justify(LV_FLEX_ALIGN_CENTER)
            .set_children(children(
                make_param(ValveHighTimeSpec, dataset.spv_on_time_sec,
                           dataset.spv_off_time_sec),
                make_param(ValveLowTimeSpec, dataset.spv_off_time_sec,
                           dataset.spv_on_time_sec),
                make_param(CalibrateValveSpec,
                           dataset.calibrate_valve_1_9_cycle, 0),
                $View(ViewProps::up()
                          .w(165)
                          .h(85)
                          .set_style(NoPaddingApply)
                          .direction(LV_FLEX_FLOW_COLUMN)
                          .justify(LV_FLEX_ALIGN_START)
                          .items(LV_FLEX_ALIGN_CENTER)
                          .track_cross(LV_FLEX_ALIGN_CENTER)
                          .set_children(children(
                              $Text(TextProps::up()
                                        .set_style(LabelPaddedApply)
                                        .value(
                                            locales::en::pressure_sensor_type)),
                              $Dropdown(DropdownProps::up()
                                            .set_options(
                                                parser::PRESSURE_TYPE_OPTIONS)
                                            .change([this](const char* option) {
                                              this->update_param(
                                                  PressureTypeSensorSpec,
                                                  option);
                                            }))))),
                $Button(ButtonProps::up()
                            .set_child(
                                $Text(TextProps::up().value("Hour Run Reset")))
                            .click([this](lv_event_t* e) {
                              this->hour_run_reset();
                            }))))
            .w(LV_PCT(100))
            .h(LV_SIZE_CONTENT));
  }

  lv_obj_t* SettingsScreen::render() {
    auto navigation_ref = this->navigation_ref;

    return this->delegate($View(
        ViewProps::up()
            .w(LV_PCT(100))
            .h(LV_PCT(100))
            .direction(LV_FLEX_FLOW_COLUMN)
            .set_children(children(
                $View(
                    ViewProps::up()
                        .h(60)
                        .direction(LV_FLEX_FLOW_ROW)
                        .items(LV_FLEX_ALIGN_CENTER)
                        .track_cross(LV_FLEX_ALIGN_CENTER)
                        .justify(LV_FLEX_ALIGN_SPACE_BETWEEN)
                        .set_children(children(
                            $View(
                                ViewProps::up()
                                    .w(LV_PCT(30))
                                    .h(60)
                                    .direction(LV_FLEX_FLOW_ROW)
                                    .items(LV_FLEX_ALIGN_CENTER)
                                    .justify(LV_FLEX_ALIGN_START)
                                    .track_cross(LV_FLEX_ALIGN_CENTER)
                                    .set_style(NoPaddingApply)
                                    .set_children(children($Button(
                                        ButtonProps::up()
                                            .set_child(
                                                $Text(TextProps::up().value(
                                                    locales::en::button_back)))
                                            .click([navigation_ref](
                                                       lv_event_t* e) {
                                              navigation_ref->reset_to("/main");
                                            }))))),
                            $View(ViewProps::up()
                                      .w(LV_PCT(40))
                                      .h(60)
                                      .set_style(NoPaddingApply)
                                      .direction(LV_FLEX_FLOW_ROW)
                                      .items(LV_FLEX_ALIGN_CENTER)
                                      .justify(LV_FLEX_ALIGN_CENTER)
                                      .track_cross(LV_FLEX_ALIGN_CENTER)
                                      .set_children(
                                          children($Text(TextProps::up().value(
                                              locales::en::
                                                  system_settings_header))))),
                            $View(ViewProps::up()
                                      .w(LV_PCT(30))
                                      .set_style(NoPaddingApply)
                                      .set_children(children(
                                          $Fragment(FragmentProps::up()))))))),
                $TabView(
                    TabViewProps::up()
                        .set_header_size(45)
                        .set_tabs({"Sensors", "Timers", "Limits", "Service"})
                        .set_children(children(
                            render_sensors_tab(), render_timers_tab(),
                            render_limits_tab(), render_service_tab())))))));
  }

  const Styling* SettingsScreen::styling() const {
    this->style.setTextColor(lv_color_make(255, 255, 255));
    this->style.setPadding(0, 0, 0, 0);
    this->style.setBorderRadius(0);
    this->style.setBorder(lv_color_make(255, 255, 255), 0, 0);

    return &this->style;
  }

  SettingsScreen* SettingsScreen::append(lv_obj_t* obj) {
    lv_obj_set_parent(obj, get_component());
    return this;
  }
}  // namespace ON2Solutions