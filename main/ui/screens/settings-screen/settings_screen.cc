#include "settings_screen.h";

#include <lg/dataset/store/dataset.store.h>
#include <lg/helpers/utils.h>
#include <ui/components/common_header/common_header.h>
#include <ui/components/side_bar/side_bar.h>

namespace ON2Solutions {

  void SettingsScreen::on_focus() {
    NavigationScreen::on_focus();
  }

  void SettingsScreen::on_blur() {
    NavigationScreen::on_blur();
  };

  int find_index(const std::vector<const char*>& options,
                 const std::string& value) {
    auto it = std::find_if(
        options.begin(), options.end(),
        [&value](const char* opt) { return std::string(opt) == value; });

    if (it != options.end()) {
      return std::distance(options.begin(), it);
    }
    return 0;
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

    ESP_LOGI("SettingsScreen", "Updating settings: %s", serialized.c_str());

    auto status = this->props.uart->send(serialized);
  }

  void SettingsScreen::update_param(const ParamSpec& spec,
                                    const char* option) const {
    auto command = parser::SerializableCommand<const char*>{
        .command = spec.command,
        .data = option,
    };
    std::string serialized = parser::serialize(command);

    ESP_LOGI("SettingsScreen", "Updating settings: %s", serialized.c_str());

    auto status = this->props.uart->send(serialized);
  }

  void SettingsScreen::hour_run_reset() const {
    auto command = parser::SerializableCommand<const char*>{
        .command = parser::SendableCommands::ResetMotoCommand,
    };

    std::string serialized = parser::serialize(command);
    auto status = this->props.uart->send(serialized);
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

  $$CommonHeader SettingsScreen::render_header() const {
    return $CommonHeader(CommonHeaderProps::up());
  }

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
  $$View SettingsScreen::make_param(const ParamSpec& spec, T value,
                                    float dependable, short width) const {
    auto [min, max] = calculate_dynamic_range(spec, dependable);
    return $View(
        ViewProps::up()
            .w(338)
            .h(52)
            .flow(FlexPreset::RowBetween)
            .set_style([](Styling& style) { style.setPadding(0, 0, 16, 16); })
            .set_children(children(
                $Text(TextProps::up().value(spec.label)),
                $Stepper(
                    StepperProps::up()
                        .value(value)
                        .set_step(spec.step)
                        .set_precision(spec.precision)
                        .range(min, max)
                        .btn_width(36)
                        .btn_height(32)
                        .set_style([](Styling& style) { style.setPadding(0); })
                        .set_spinbox_style(
                            [](Styling& style) { style.setPadding(0); })
                        .size(56, 32)
                        .template watch<parser::Dataset>(
                            parser::DatasetStore::getInstance(),
                            fmt_str("param_%s", spec.label),
                            [spec](Stepper* self,
                                   const parser::Dataset& dataset) {
                              auto [min, max] =
                                  calculate_dynamic_range(spec, dataset);
                              self->set_state([min, max](StepperProps& props) {
                                props.range(min, max);
                              });
                            })
                        .on_change([this, spec, min, max](const float v) {
                          this->debounce->exec([this, spec, v, min, max]() {
                            this->update_param<T>(
                                spec, static_cast<T>(std::clamp(v, min, max)),
                                min, max);
                          });
                        })))));
  }

  $$View SettingsScreen::render_sensors_tab() const {
    parser::DatasetSettings dataset =
        parser::DatasetStore::getInstance()->get()->settings;
    return $View(
        ViewProps::up()
            .direction(LV_FLEX_FLOW_ROW_WRAP)
            .justify(LV_FLEX_ALIGN_CENTER)
            .set_style([](Styling& style) {
              style.setPadding(12);
              style.setBorderRadius(12);
              style.setBorder(PRIMARY_BG, 0, LV_OPA_0);
            })
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
        parser::DatasetStore::getInstance()->get()->settings;

    return $View(
        ViewProps::up()
            .direction(LV_FLEX_FLOW_ROW_WRAP)
            .justify(LV_FLEX_ALIGN_CENTER)
            .set_style([](Styling& style) {
              style.setPadding(12);
              style.setBorderRadius(12);
              style.setBorder(PRIMARY_BG, 0, LV_OPA_0);
            })
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
        parser::DatasetStore::getInstance()->get()->settings;

    return $View(
        ViewProps::up()
            .direction(LV_FLEX_FLOW_ROW_WRAP)
            .justify(LV_FLEX_ALIGN_CENTER)
            .set_style([](Styling& style) {
              style.setPadding(12);
              style.setBorderRadius(12);
              style.setBorder(PRIMARY_BG, 0, LV_OPA_0);
            })
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
        parser::DatasetStore::getInstance()->get()->settings;

    return $View(
        ViewProps::up()
            .direction(LV_FLEX_FLOW_ROW_WRAP)
            .justify(LV_FLEX_ALIGN_CENTER)
            .set_style([](Styling& style) {
              style.setPadding(12);
              style.setBorderRadius(12);
              style.setBorder(PRIMARY_BG, 0, LV_OPA_0);
            })
            .set_children(children(
                make_param(ValveHighTimeSpec, dataset.spv_on_time_sec,
                           dataset.spv_off_time_sec),
                make_param(ValveLowTimeSpec, dataset.spv_off_time_sec,
                           dataset.spv_on_time_sec),
                make_param(CalibrateValveSpec,
                           dataset.calibrate_valve_1_9_cycle, 0),

                $View(
                    ViewProps::up()
                        .w(338)
                        .h(52)
                        .flow(FlexPreset::RowBetween)
                        .set_style([](Styling& style) {
                          style.setPadding(0, 0, 16, 16);
                        })
                        .set_children(children(
                            $Text(TextProps::up().value(
                                locales::en::pressure_sensor_type)),
                            $Dropdown(
                                DropdownProps::up()
                                    .set_selected(find_index(
                                        parser::PRESSURE_TYPE_OPTIONS,
                                        std::string(
                                            std::begin(
                                                dataset
                                                    .tank_pressure_sensor_type),
                                            std::end(
                                                dataset
                                                    .tank_pressure_sensor_type))))
                                    .set_options(parser::PRESSURE_TYPE_OPTIONS)
                                    .change([this](const std::string& option) {
                                      this->debounce->exec([this, option]() {
                                        this->update_param(
                                            PressureTypeSensorSpec,
                                            option.c_str());
                                      });
                                    }))))),
                $Button(ButtonProps::up()
                            .set_child($Text(TextProps::up().value(
                                locales::en::hour_run_reset)))
                            .click([this](lv_event_t* e) {
                              this->hour_run_reset();
                            }))))
            .w(LV_PCT(100))
            .h(LV_SIZE_CONTENT));
  }

  lv_obj_t* SettingsScreen::render() {
    VNode::render();

    return this->delegate($View(
        ViewProps::up()
            .set_style([](Styling& style) {
              style.setTextColor(lv_color_make(255, 255, 255));
              style.setPadding(0, 0, 0, 0);
              style.setBorderRadius(0);
              style.setBorder(lv_color_make(255, 255, 255), 0, 0);
              style.setGap(0, 0);
            })
            .set_children(children(
                this->render_header(),
                $View(
                    ViewProps::up()
                        .w(LV_PCT(100))
                        .h(LV_PCT(100))
                        .set_style(NoPaddingApply)
                        .direction(LV_FLEX_FLOW_ROW)
                        .justify(LV_FLEX_ALIGN_SPACE_BETWEEN)
                        .items(LV_FLEX_ALIGN_START)
                        .track_cross(LV_FLEX_ALIGN_SPACE_BETWEEN)
                        .set_children(children(
                            $Sidebar(SidebarProps::up().set_stack(
                                this->navigation_ref)),
                            $View(
                                ViewProps::up()
                                    .w(800 - 56)
                                    .h(LV_PCT(100))
                                    .set_style([](Styling& style) {
                                      style.setBackgroundColor(SECONDARY_BG);
                                      style.setBorderRadius(0);
                                      style.setPadding(16, 16, 16, 16);
                                      style.setBorder(SECONDARY_BG, 0,
                                                      LV_OPA_0);
                                    })
                                    .set_children(children($TabView(
                                        TabViewProps::up()
                                            .set_header_size(45)
                                            .set_tabs({"Sensors", "Timers",
                                                       "Limits", "Service"})
                                            .set_children(children(
                                                render_sensors_tab(),
                                                render_timers_tab(),
                                                render_limits_tab(),
                                                render_service_tab())))))))))))
            .merge(screen_container_props)));
  }

  /*
  *
   *
   */

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