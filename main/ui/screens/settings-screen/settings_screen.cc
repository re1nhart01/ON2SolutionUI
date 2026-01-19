#include "settings_screen.h";

#include <lg/dataset/store/dataset.store.h>

namespace ON2Solutions {

  void SettingsScreen::on_focus() {
    // this->uart_handler->init();
    // this->uart_handler->enable_rx(true);
  }

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

  //TODO: make that one param depends from other
  $$Stepper SettingsScreen::make_param(
      const ParamSpec& spec, float value, float dependable,
      const Delegate<void(float), 32>& on_change, short width = 185) const {
    auto [min, max] = calculate_dynamic_range(spec, dependable);
    return $Stepper(StepperProps::up()
                        .set_label(spec.label)
                        .value(value)
                        .set_step(spec.step)
                        .set_precision(spec.precision)
                        .range(min, max)
                        .size(width, 55)
                        .on_change([on_change](float v) {
                          if (on_change)
                            on_change(v);
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
                make_param(
                    OxygenShiftASpec, dataset.oxygen_sensor_offset[0], 0,
                    []() {}, 165),
                make_param(
                    OxygenShiftBSpec, dataset.oxygen_sensor_offset[1], 0,
                    []() {}, 165),
                make_param(
                    OxygenShiftCSpec, dataset.oxygen_sensor_offset[2], 0,
                    []() {}, 165),
                make_param(
                    OxygenShiftDSpec, dataset.oxygen_sensor_offset[3], 0,
                    []() {}, 165),
                render_section_header(locales::en::oxygen_flow_setting),
                make_param(
                    FlowShiftASpec, dataset.flow_sensor_offset[0], 0, []() {},
                    165),
                make_param(
                    FlowShiftBSpec, dataset.flow_sensor_offset[1], 0, []() {},
                    165),
                make_param(
                    FlowShiftCSpec, dataset.flow_sensor_offset[2], 0, []() {},
                    165),
                make_param(
                    FlowShiftDSpec, dataset.flow_sensor_offset[3], 0, []() {},
                    165)))
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
                make_param(CompressorDelaySpec, dataset.compressor_delay_sec, 0,
                           []() {}),
                make_param(RunUpTimeSpec, dataset.run_up_delay_sec, 0, []() {}),
                make_param(PreStartTimeSpec, dataset.prestart_time_sec, 0,
                           []() {}),
                make_param(LowLimitTimeSpec,
                           dataset.low_limit_oxygen_concentration, 0, []() {})))
            .w(LV_PCT(100))
            .h(LV_SIZE_CONTENT));
  }

  $$View SettingsScreen::render_limits_tab() const {
    parser::DatasetSettings dataset =
        parser::DatasetStore::getInstance()->get().settings;

    return $View(ViewProps::up()
                     .direction(LV_FLEX_FLOW_ROW_WRAP)
                     .justify(LV_FLEX_ALIGN_CENTER)
                     .set_children(children(
                         make_param(WorkConcentrationSpec,
                                    dataset.compressor_delay_sec, 0, []() {}),
                         make_param(LowLimitConcentrationSpec,
                                    dataset.compressor_delay_sec, 0, []() {}),
                         make_param(LowLimitTimeSpec,
                                    dataset.compressor_delay_sec, 0, []() {}),
                         make_param(TankHighLimitSpec,
                                    dataset.compressor_delay_sec, 0, []() {}),
                         make_param(TankLowLimitSpec,
                                    dataset.compressor_delay_sec, 0, []() {}),
                         make_param(TempOverheatSpec,
                                    dataset.compressor_delay_sec, 0, []() {}),
                         dataset.compressor_delay_sec, 0, []() {},
                         make_param(FlowErrorSpec, dataset.compressor_delay_sec,
                                    0, []() {}),
                         make_param(ErrorCountSpec,
                                    dataset.compressor_delay_sec, 0, []() {})))
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
                make_param(ValveHighTimeSpec, dataset.spv_on_time_sec, 0,
                           []() {}),
                make_param(ValveLowTimeSpec, dataset.spv_off_time_sec, 0,
                           []() {}),
                $Button(ButtonProps::up()
                            .set_child(
                                $Text(TextProps::up().value("Hour Run Reset")))
                            .click([](lv_event_t* e) { /* Reset logic */ }))))
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