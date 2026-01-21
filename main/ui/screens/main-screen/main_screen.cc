#include "main_screen.h"

#include <ui/styles/theme.h>

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

  void MainScreen::add_uart_data_event() {
    this->uart_handler->add_event_listener(UartTypes::UartHandlerEvent{
        .key_v = const_cast<char*>("read_data_dto"),
        .event = UART_DATA,
        .delegate = [](const UartTypes::UartCallbackResponse& uart_data) {
          if (strlen(uart_data.response.packet) <= 0)
            return;

          struct AsyncStructure {
            std::string packet;
          };

          ESP_LOGI("main_screen", "Received data from UART %s",
                   uart_data.response.packet);

          auto args = new AsyncStructure{.packet = uart_data.response.packet};

          lv_async_call(
              [](void* arg) {
                auto* p = static_cast<AsyncStructure*>(arg);
                Dataset dataset = DatasetStore::getInstance()->get();
                parse(&dataset, p->packet.c_str(), p->packet.length());
                DatasetStore::getInstance()->set(dataset);

                delete p;
              },
              args);
        }});
  }

#pragma region UI
  $$View MainScreen::render_header() {
    auto navigator_ref = this->navigation_ref;

    return $View(
        ViewProps::up()
            .set_style(HeaderContainerApply)
            .set_children(children(
                $View(
                    ViewProps::up()
                        .set_style(HeaderLabelContainerApply)
                        .set_children(children(
                            $View(
                                ViewProps::up()
                                    .direction(LV_FLEX_FLOW_ROW)
                                    .items(LV_FLEX_ALIGN_CENTER)
                                    .track_cross(LV_FLEX_ALIGN_CENTER)
                                    .w(LV_PCT(50))
                                    .h(LV_PCT(100))
                                    .set_style([](Styling& s) {
                                      s.setGap(8, 8);
                                      NoPaddingApply(s);
                                    })
                                    .set_children(children(
                                        $Text(TextProps::up().value("Inputs:")),
                                        $DotIndicator(
                                            DotIndicatorProps::up()
                                                .set_dot_amount(8)
                                                .w(50)
                                                .h(20)
                                                .watch<Dataset>(
                                                    DatasetStore::getInstance(),
                                                    "inputs",
                                                    [](DotIndicator* self,
                                                       const Dataset& value) {
                                                      self->set_state(
                                                          [v = value
                                                                   .operative_data
                                                                   .inputs](
                                                              DotIndicatorProps&
                                                                  p) {
                                                            p.set_value_hex(v);
                                                          });
                                                    }))))),

                            $View(
                                ViewProps::up()
                                    .direction(LV_FLEX_FLOW_ROW)
                                    .items(LV_FLEX_ALIGN_CENTER)
                                    .track_cross(LV_FLEX_ALIGN_CENTER)
                                    .w(LV_PCT(50))
                                    .h(LV_PCT(100))
                                    .set_style([](Styling& s) {
                                      s.setGap(8, 8);
                                      NoPaddingApply(s);
                                    })
                                    .set_children(children(
                                        $Text(
                                            TextProps::up().value("Outputs:")),
                                        $DotIndicator(
                                            DotIndicatorProps::up()
                                                .set_dot_amount(16)
                                                .w(100)
                                                .h(20)
                                                .watch<Dataset>(
                                                    DatasetStore::getInstance(),
                                                    "outputs",
                                                    [](DotIndicator* self,
                                                       const Dataset& value) {
                                                      self->set_state(
                                                          [v = value
                                                                   .operative_data
                                                                   .outputs](
                                                              DotIndicatorProps&
                                                                  p) {
                                                            p.set_value_hex(v);
                                                          });
                                                    })))))))
                        .merge(header_labels_container_props)),

                $View(
                    ViewProps::up()
                        .set_style(HeaderContainerApply)
                        .set_children(children(
                            $Button(
                                ButtonProps::up()
                                    .set_style(HeaderButtonApply)
                                    .watch<Dataset>(
                                        DatasetStore::getInstance(), "errors",
                                        [this](Button* self,
                                               const Dataset& value) {
                                          const bool hasError =
                                              value.operative_data.errors != 0;
                                          update_styles<ButtonProps>(
                                              self, [hasError](Styling& style) {
                                                style.setBackgroundColor(
                                                    hasError ? ERROR_COLOR
                                                             : NO_ERROR_COLOR);
                                              });
                                        })
                                    .set_child($Text(
                                        TextProps::up()
                                            .set_style(HeaderLabelApply)
                                            .value(locales::en::header_errors)))
                                    .click([this](lv_event_t* e) {
                                      this->show_errors_modal();
                                    })),
                            $Button(ButtonProps::up()
                                        .set_style(HeaderButtonApply)
                                        .set_child($Text(
                                            TextProps::up()
                                                .set_style(HeaderLabelApply)
                                                .value(locales::en::
                                                           header_information)))
                                        .click([this](lv_event_t* e) {
                                          this->show_info_modal();
                                        })),
                            $Button(
                                ButtonProps::up()
                                    .set_style(HeaderButtonApply)
                                    .set_child($Text(
                                        TextProps::up()
                                            .set_style(HeaderLabelApply)
                                            .value(
                                                locales::en::header_settings)))
                                    .click([navigator_ref](lv_event_t* e) {
                                      navigator_ref->navigate("/pin_code");
                                    }))))
                        .merge(header_container_right_props))))
            .merge(header_container_props));
  }

  $$View MainScreen::render_body() const {
    auto make_circle = [&](const std::string& ref_name, int index) {
      return $Meter(
          MeterProps::up()
              .watch<Dataset>(
                  DatasetStore::getInstance(), "outputs",
                  [index, ref_name](Meter* self, const Dataset& value) {
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
              .w(135)
              .h(135));
    };

    return $View(
        ViewProps::up()
            .set_style(HeaderContainerApply)
            .set_children(children(
                $Text(TextProps::up()
                          .set_style(HeaderLabelApply)
                          .value(locales::en::oxygen_level)),
                $View(
                    ViewProps::up()
                        .set_style(NoPaddingApply)
                        .set_children(children(make_circle("oxygen_level", 1),
                                               make_circle("oxygen_level", 2),
                                               make_circle("oxygen_level", 3)))
                        .w(LV_PCT(100))
                        .h(135)
                        .set_overflow(true)
                        .justify(LV_FLEX_ALIGN_SPACE_AROUND)
                        .items(LV_FLEX_ALIGN_CENTER)
                        .track_cross(LV_FLEX_ALIGN_CENTER)
                        .direction(LV_FLEX_FLOW_ROW)),
                $Text(TextProps::up()
                          .set_style(HeaderLabelApply)
                          .value(locales::en::oxygen_rate)),
                $View(ViewProps::up()
                          .set_style(NoPaddingApply)
                          .set_children(children(make_circle("oxygen_rate", 1),
                                                 make_circle("oxygen_rate", 2),
                                                 make_circle("oxygen_rate", 3)))
                          .w(LV_PCT(100))
                          .set_overflow(true)
                          .h(135)
                          .justify(LV_FLEX_ALIGN_SPACE_AROUND)
                          .items(LV_FLEX_ALIGN_CENTER)
                          .track_cross(LV_FLEX_ALIGN_CENTER)
                          .direction(LV_FLEX_FLOW_ROW))))
            .w(LV_PCT(100))
            .h(LV_PCT(100))
            .justify(LV_FLEX_ALIGN_START)
            .items(LV_FLEX_ALIGN_CENTER)
            .track_cross(LV_FLEX_ALIGN_START)
            .direction(LV_FLEX_FLOW_COLUMN));
  }

  $$Animated MainScreen::render_animated_alarm() const {
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
                                  props.value(is_alarm_now
                                                  ? fmt_str("Alarm to "
                                                            "reset: %d",
                                                            count)
                                                  : "ON2 Systems");
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
                          .value("ON2 Systems"))));
  }

  $$View MainScreen::render_body_left() const {
    auto make_circle = [&](const std::string& ref_name, int index) {
      return $Meter(
          MeterProps::up()
              .set_text_style(
                  [](Styling& style) { style.setFont(&lv_font_montserrat_20); })
              .watch<Dataset>(
                  DatasetStore::getInstance(), "outputs",
                  [index, ref_name](Meter* self, const Dataset& value) {
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
              .w(270)
              .h(270));
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

  $$View MainScreen::render_footer() const {
    return $View(
        ViewProps::up()
            .set_children(children($Button(
                ButtonProps::up()
                    .set_style(FooterButtonApply)
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
                    }))))
            .set_overflow(true)
            .set_style([](Styling& style) {
              style.setPadding(0, 0, 16, 16);
              style.setBorderRadius(0);
              style.setBorder(lv_color_make(255, 255, 255), 0, 0);
            }));
  }

  lv_obj_t* MainScreen::render() {
    VNode::render();
    ESP_LOGI("main screen", "render");

    return this->delegate($View(
        ViewProps::up()
            .set_style([](Styling& style) {
              style.setTextColor(lv_color_make(255, 255, 255));
              style.setPadding(0, 0, 0, 0);
              style.setBorderRadius(0);
              style.setBorder(lv_color_make(255, 255, 255), 0, 0);
            })
            .set_children(children(
                $StatusBar(
                    StatusBarProps::up()
                        .set_background_color(lv_color_hex(0x2A2A2A))
                        .set_height(30)
                        .set_children(children(
                            $Text(TextProps::up()
                                      .watch<Dataset>(
                                          DatasetStore::getInstance(),
                                          "moto_hours",
                                          [](Text* self, const Dataset& value) {
                                            std::string hours =
                                                value.operative_data.moto_hours
                                                    .data();
                                            self->set_state(
                                                [hours](TextProps& props) {
                                                  props.value(hours);
                                                });
                                          })
                                      .value("06:10 AM")),
                            render_animated_alarm(),
                            $Text(TextProps::up()
                                      .watch<Dataset>(
                                          DatasetStore::getInstance(),
                                          "channels",
                                          [](Text* self, const Dataset& value) {
                                            int count = value.operative_data
                                                            .channels_count;
                                            self->set_state(
                                                [count](TextProps& props) {
                                                  props.value(fmt_str(
                                                      "Channels: %d", count));
                                                });
                                          })
                                      .value("Channels: 0"))))),
                $ScrollView(
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
                                          children(this->render_header(),
                                                   this->render_body_left()))),
                            $View(ViewProps::up()
                                      .set_style(NoPaddingApply)
                                      .w(LV_PCT(100))
                                      .h(LV_PCT(100))
                                      .set_children(
                                          children(this->render_body())))))),
                this->render_footer()))
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