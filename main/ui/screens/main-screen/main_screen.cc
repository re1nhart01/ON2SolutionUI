#include "main_screen.h"

#include "constants/theme.h"

using namespace foundation;
using namespace ON2Solutions::parser;

static const char* current_packet = CH_PACKETS[0];

namespace ON2Solutions {
  void MainScreen::on_focus() {
    NavigationScreen::on_focus();
    this->uart_handler = std::make_unique<UartHandler>(
        UART_NUM_2, GPIO_NUM_43, GPIO_NUM_44, 9600, 16384);
    // start_random_updater();
    ESP_LOGI("main_screen", "on_FOCUS");
    this->uart_handler->init();
    this->uart_handler->enable_rx(true);
    this->add_uart_data_event();
  };

  void MainScreen::on_blur() {
    NavigationScreen::on_blur();
    this->uart_handler->stop();
    // if (xHandle != nullptr) {
    // vTaskDelete(xHandle);
    // xHandle = nullptr;
    // }
  };

  void MainScreen::execute_status_trigger() const {
    auto command = SerializableCommand<const char*>{
        .command = SendableCommands::StatusCommand,
    };

    std::string serialized = serialize(command);

    auto status = this->uart_handler->send(serialized);
  }

  template <typename C>
  void MainScreen::update_styles(Component<C>* component,
                                 const Delegate<void(Styling&)>& style) {
    component->set_state([style](C& props) { props.set_style(style); });
  }

  void MainScreen::show_info_modal() {
    DatasetSystemInfo system_info =
        DatasetStore::getInstance()->get().system_info;

    info_modal = $InfoModal(
        std::move(InfoModalProps::up()
                      .set_device(system_info.device_name.data())
                      .set_loader(system_info.loader_version.data())
                      .set_fw(system_info.firmware_version.data())
                      .set_fw_checksum(system_info.firmware_checksum.data())
                      .set_module_name(system_info.module_name.data())
                      .set_module_fw(system_info.firmware_version.data())
                      .set_serial_number(system_info.serial_number.data())
                      .set_ethernet_ip(system_info.lan_ip_address.data())
                      .set_wifi_ip(system_info.wifi_ip_address.data())
                      .set_lcd_fw("x.x.x")
                      .set_lcd_loader("x.x.x")
                      .set_lcd_partition("x.x.x")
                      .set_restart_seconds(99)));

    info_modal->show();
  }

  void MainScreen::show_errors_modal() {
    DatasetOperative operative_data =
        DatasetStore::getInstance()->get().operative_data;

    error_modal = $ErrorModal(
        std::move(ErrorModalProps::up().set_error_hex(operative_data.errors)));

    error_modal->show();
  }

  void MainScreen::start_random_updater() {
    xTaskCreate(
        [](void* pvParameters) {
          auto* const self = static_cast<MainScreen*>(pvParameters);
          while (true) {
            if (self) {
              current_packet = CH_PACKETS[esp_random() % 15];
              const char* p = current_packet;

              if (lvgl_port_lock(-1)) {
                Dataset dataset = DatasetStore::getInstance()->get();
                parse(&dataset, p, strlen(p));

                DatasetStore::getInstance()->set(dataset);

                lvgl_port_unlock();
              }
            }
            vTaskDelay(pdMS_TO_TICKS(1500));
          }
        },
        "rand_task", 16384, this, 5, &xHandle);
  }

  void MainScreen::add_uart_data_event() const {
    this->uart_handler->add_event_listener(UartTypes::UartHandlerEvent{
        .key_v = const_cast<char*>("read_data_dto"),
        .event = UART_DATA,
        .delegate = [](const UartTypes::UartCallbackResponse& uart_data) {
          if (strlen(uart_data.response.packet) <= 0)
            return;

          ESP_LOGI("main_screen", "Received data from UART %s",
                   uart_data.response.packet);

          if (uart_data.response.packet &&
              strlen(uart_data.response.packet) > 0) {

            Dataset dataset = DatasetStore::getInstance()->get();
            parse(&dataset, uart_data.response.packet, uart_data.response.len);
            DatasetStore::getInstance()->set(dataset);
          }
        }});
  }

#pragma region UI
  $$CommonHeader MainScreen::render_header() const {
    return $CommonHeader(CommonHeaderProps::up());
  }

  $$View MainScreen::render_card() const {
    return $View(
        ViewProps::up()
            .w(318)
            .h(328)
            .direction(LV_FLEX_FLOW_COLUMN)
            .flow(FlexPreset::SpaceCenter)
            .set_style([](Styling& style) {

            })
            .set_children(children(
                $SpecificCircular(CircularSelectorType::O2, 0, false),
                $View(ViewProps::up()
                          .w(LV_PCT(100))
                          .h(24)
                          .set_style([](Styling& style) {
                            style.setGap(8, 8);
                            style.setPadding(0);
                            style.setBorder(PRIMARY_BG, 0, LV_OPA_0);
                          })
                          .flow(FlexPreset::RowCenter)
                          .set_children(children($ConnectionStat(),
                                                 $Text(TextProps::up().value(
                                                     "Oxygen Level"))))))));
  }

  $$View MainScreen::render_body() const {
    auto navigation = navigation_ref;

    return $View(ViewProps::up()
                     .w(LV_PCT(100))
                     .h(LV_PCT(100))
                     .direction(LV_FLEX_FLOW_COLUMN)
                     .justify(LV_FLEX_ALIGN_START)
                     .items(LV_FLEX_ALIGN_CENTER)
                     .track_cross(LV_FLEX_ALIGN_START)
                     .set_style([](Styling& style) {
                       style.setBackgroundOpa(LV_OPA_0);
                       style.setPadding(0);
                       style.setBorder(PRIMARY_BG, 0, LV_OPA_0);
                     })
                     .set_children(children(
                         $View(ViewProps::up()
                                   .w(LV_PCT(100))
                                   .h(LV_SIZE_CONTENT)
                                   .direction(LV_FLEX_FLOW_ROW)
                                   .justify(LV_FLEX_ALIGN_SPACE_BETWEEN)
                                   .items(LV_FLEX_ALIGN_CENTER)
                                   .track_cross(LV_FLEX_ALIGN_START)
                                   .set_style([](Styling& style) {
                                     style.setBackgroundOpa(LV_OPA_0);
                                     style.setPadding(0);
                                     style.setBorder(PRIMARY_BG, 0, LV_OPA_0);
                                   })
                                   .set_children(children(
                                       this->render_card(), this->render_card(),
                                       $HighButton(assets::Right, [navigation](lv_event_t* _) {
                                         navigation->navigate("/charts");
                                       })))),
                         this->render_footer())));
  }

  $$View MainScreen::render_footer() const {
    return $View(
        ViewProps::up()
            .set_children(children(
            $TimerView(),
            $Button(
                ButtonProps::up()
                    .set_style([](Styling& style) {
                      style.setFont(&lv_font_montserrat_16);
                      style.setBackgroundColor(button_color_by_status(
                          parser::DatasetStatuses::StandBy));
                      style.setBorderRadius(12);
                      style.setSize(300, 44);
                      style.setBorder(lv_color_hex(0x5B5AFF), 0, 0);
                      style.setPadding(8, 8, 16, 16);
                      style.setBorderRadius(14);
                    })
                    .watch<Dataset>(
                        DatasetStore::getInstance(), "main_button_body",
                        [](Button* self, const Dataset& value) {
                          auto color =
                              button_color_by_status(GetStatusFromTextValue(
                                  value.operative_data.status.data()));
                          self->set_state([color](ButtonProps& props) {
                            props.set_style([color](Styling& style) {
                              style.setBackgroundColor(color);
                            });
                          });
                        })
                    .set_child($Text(
                        TextProps::up()
                            .watch<Dataset>(
                                DatasetStore::getInstance(), "main_button",
                                [](Text* self, const Dataset& value) {
                                  std::string status_str =
                                      GetTextValueFromStatus(
                                          GetStatusFromTextValue(
                                              value.operative_data.status
                                                  .data()));
                                  self->set_state(
                                      [status_str](TextProps& props) {
                                        props.value(status_str);
                                      });
                                })
                            .set_style(HeaderLabelApply)
                            .value(locales::en::status)))
                    .click([this](lv_event_t* e) {
                      this->execute_status_trigger();
                    })),
            $FooterFragment()
        ))
            .flow(FlexPreset::RowCenter)
            .set_style([](Styling& style) {
              style.setPadding(0);
              style.setGap(8,8);
              style.setBorderRadius(0);
              style.setBorder(PRIMARY_BG, 0, 0);
            }));
  }

  lv_obj_t* MainScreen::render() {
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
                $View(ViewProps::up()
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
                              $View(ViewProps::up()
                                        .w(800 - 56)
                                        .h(LV_PCT(100))
                                        .set_style([](Styling& style) {
                                          style.setBackgroundColor(
                                              SECONDARY_BG);
                                          style.setBorderRadius(0);
                                          style.setPadding(16, 16, 16, 16);
                                          style.setBorder(SECONDARY_BG, 0,
                                                          LV_OPA_0);
                                        })
                                        .set_children(
                                            children(this->render_body()))))))))
            .merge(screen_container_props)));
  }

#pragma endregion UI

  const Styling* MainScreen::styling() const {
    this->style.setTextColor(lv_color_make(255, 255, 255));
    this->style.setPadding(0, 0, 0, 0);
    this->style.setBorderRadius(0);
    this->style.setBorder(lv_color_make(255, 255, 255), 0, 0);

    return &this->style;
  }

  MainScreen* MainScreen::append(lv_obj_t* obj) {
    lv_obj_set_parent(obj, get_component());
    return this;
  }
}  // namespace ON2Solutions

/*
* $ScrollView(
                    ScrollViewProps::up()
                        .set_style(NoPaddingApply)
                        .w(LV_PCT(100))
                        .h(LV_PCT(75))
                        .direction(LV_FLEX_FLOW_ROW)
                        .scroll(LV_DIR_HOR)
                        .scrollbar(LV_SCROLLBAR_MODE_OFF)
                        .snap(LV_SCROLL_SNAP_CENTER, LV_SCROLL_SNAP_NONE)
                        .set_elastic(false)
                        .set_momentum(true)
                        .set_children(children(
                            $View(ViewProps::up()
                                      .set_style(NoPaddingApply)
                                      .w(LV_PCT(100))
                                      .h(LV_PCT(100))
                                      .set_children(
                                          children(this->render_body_left()))),
                            $View(ViewProps::up()
                                      .set_style(NoPaddingApply)
                                      .w(LV_PCT(100))
                                      .h(LV_PCT(100))
                                      .set_children(
                                          children(this->render_body()))))))
 *
 *
 *
 *$$Animated MainScreen::render_animated_alarm() const {
    return $Animated(
        AnimatedProps::up()
            .from(255)
            .to(120)
            .stop(255)
            .time(600)
            .playback(600)
            .wait(1500)
            .easing(lv_anim_path_ease_in_out)
            .set_control("alarm_anim", alarm_control)
            .set_auto_start(false)
            .repeat(LV_ANIM_REPEAT_INFINITE)
            .prop(AnimatedProps::Property::Opacity)
            .on($Text(TextProps::up()
                          .set_style(HeaderLabelApply)
                          .watch<Dataset>(
                              DatasetStore::getInstance(), "alarm_text",
                              [this](Text* self, const Dataset& value) {
                                bool is_alarm_now =
                                    value.optional.reset_countdown_sec != 0 &&
                                    GetStatusFromTextValue(
                                        value.operative_data.status.data()) ==
                                        DatasetStatuses::Alarm;
                                uint8_t count =
                                    value.optional.reset_countdown_sec;
                                ESP_LOGI("main screen", "alarm_text %d", count);

                                self->set_state([value, is_alarm_now,
                                                 count](TextProps& props) {
                                  props.value(
                                      is_alarm_now
                                          ? fmt_str("ALARM TO RESET: %d", count)
                                          : "ON2 SYSTEMS");
                                  props.set_style(is_alarm_now
                                                      ? AlarmTextStyleApply
                                                      : DefaultTextStyleApply);
                                });
                                if (is_alarm_now) {
                                  this->alarm_control->play("alarm_anim");
                                } else {
                                  this->alarm_control->stop("alarm_anim");
                                }
                              })
                          .value("ON2 SYSTEMS"))));
  }
 *
 *
 */


/*
 *
 *
 * $$View MainScreen::render_body_left() const {
    auto make_circle = [&](const std::string& ref_name, int index) {
      return $Circular(
          CircularProgressProps::up()
              .set_text_style(
                  [](Styling& style) { style.setFont(&lv_font_montserrat_20); })
              .watch<Dataset>(
                  DatasetStore::getInstance(), "outputs",
                  [index, ref_name](CircularProgress* self, const Dataset& value) {
                    float val = (ref_name == "oxygen_level")
                                    ? value.operative_data.oxygen_levels[index]
                                    : value.operative_data.oxygen_speed[index];

                    self->set_state(
                        [val](MeterProps& props) { props.value(val); });
                  })
              .label("%")
              .show_label(true)
              .min(0)
              .max(100)
              .value(0)
              .w(232)
              .h(232));
    };

    return $View(
        ViewProps::up()
            .set_style(HeaderContainerApply)
            .set_children(children(
                $View(ViewProps::up()
                          .w(LV_PCT(50))
                          .h(LV_PCT(100))
                          .set_style(NoPaddingApply)
                          .direction(LV_FLEX_FLOW_COLUMN)
                          .justify(LV_FLEX_ALIGN_START)
                          .items(LV_FLEX_ALIGN_CENTER)
                          .track_cross(LV_FLEX_ALIGN_CENTER)
                          .set_children(children(
                              $Text(TextProps::up()
                                        .set_style(LabelPaddedApply)
                                        .value(locales::en::oxygen_rate)),
                              make_circle("oxygen_level", 0)))),

                $View(ViewProps::up()
                          .w(LV_PCT(50))
                          .h(LV_PCT(100))
                          .set_style(NoPaddingApply)
                          .direction(LV_FLEX_FLOW_COLUMN)
                          .justify(LV_FLEX_ALIGN_START)
                          .items(LV_FLEX_ALIGN_CENTER)
                          .track_cross(LV_FLEX_ALIGN_CENTER)
                          .set_children(children(
                              $Text(TextProps::up()
                                        .set_style(LabelPaddedApply)
                                        .value(locales::en::oxygen_rate)),
                              make_circle("oxygen_rate", 0))))))
            .w(LV_PCT(100))
            .h(LV_PCT(100))
            .justify(LV_FLEX_ALIGN_SPACE_BETWEEN)
            .items(LV_FLEX_ALIGN_CENTER)
            .track_cross(LV_FLEX_ALIGN_START)
            .direction(LV_FLEX_FLOW_ROW));
  }
 *
 */