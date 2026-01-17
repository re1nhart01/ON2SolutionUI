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
    start_random_updater();
    ESP_LOGI("main_screen", "on_FOCUS");
    // this->uart_handler->init();
    // this->uart_handler->enable_rx(true);
    // this->add_uart_data_event();
  };

  void MainScreen::on_blur() {
    NavigationScreen::on_blur();
    this->uart_handler->remove_all_event_listeners();
    if (xHandle != nullptr) {
      vTaskDelete(xHandle);
      xHandle = nullptr;
    }
  };
  template <typename C>
  void MainScreen::update_styles(Component<C>* component, const Delegate<void(Styling&)>& style) {
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

    info_modal->show();
  }

  void MainScreen::start_random_updater() {
    xTaskCreate(
        [](void* pvParameters) {
          auto* const self = static_cast<MainScreen*>(pvParameters);

          while (true) {
            if (self) {
              current_packet = CH_PACKETS[esp_random() % 15];

              const char* p = current_packet;

              Dataset dataset = DatasetStore::getInstance()->get();
              parse(&dataset, p, strlen(p));
              DatasetStore::getInstance()->set(dataset);
            }
            vTaskDelay(pdMS_TO_TICKS(1500));
          }
        },
        "rand_task", 16384, this, 5, &xHandle);
  }

  void MainScreen::add_uart_data_event() {
    auto* moto_lvgl = &this->reactive_moto_lvgl;
    this->uart_handler->add_event_listener(UartTypes::UartHandlerEvent{
        .key_v = const_cast<char*>("read_data_dto"),
        .event = UART_DATA,
        .delegate = [moto_lvgl](
                        const UartTypes::UartCallbackResponse& uart_data) {
          if (strlen(uart_data.response.packet) <= 0)
            return;

          struct AsyncStructure {
            std::string packet;
            Reactive<int>* reactive_moto_lvgl;
          };

          auto args = new AsyncStructure{.packet = uart_data.response.packet,
                                         .reactive_moto_lvgl = moto_lvgl};

          lv_async_call(
              [](void* arg) {
                auto* p = static_cast<AsyncStructure*>(arg);
                Dataset dataset = DatasetStore::getInstance()->get();
                parse(&dataset, p->packet.c_str(), p->packet.length());
                DatasetStore::getInstance()->set(dataset);
                if (p->reactive_moto_lvgl) {
                  p->reactive_moto_lvgl->set(
                      [](const int& prev) { return prev + 1; });
                }
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
            .set_style($s("header.container"))
            .set_children(children(
                $View(
                    ViewProps::up()
                        .set_style($s("header.labels.container"))
                        .set_children(children(
                            $Text(TextProps::up()
                                      .watch<Dataset>(
                                          DatasetStore::getInstance(),
                                          "channels",
                                          [](Text* self, const Dataset& value) {
                                            int count = value.operative_data
                                                            .channels_count;
                                            self->set_state(
                                                [count](TextProps& props) {
                                                  props.value(std::format(
                                                      "Channels: {}", count));
                                                });
                                          })
                                      .value("Channels: 0")),
                            $Text(TextProps::up()
                                      .watch<Dataset>(
                                          DatasetStore::getInstance(), "inputs",
                                          [](Text* self, const Dataset& value) {
                                            int inputs =
                                                value.operative_data.inputs;
                                            self->set_state(
                                                [inputs](TextProps& props) {
                                                  props.value(std::format(
                                                      "Inputs: {}", inputs));
                                                });
                                          })
                                      .value("Inputs: 0")),
                            $Text(
                                TextProps::up()
                                    .watch<Dataset>(
                                        DatasetStore::getInstance(), "outputs",
                                        [](Text* self, const Dataset& value) {
                                          int outputs =
                                              value.operative_data.outputs;
                                          self->set_state(
                                              [outputs](TextProps& props) {
                                                const auto output = std::format(
                                                    "Outputs: {}", outputs);
                                                props.value(output);
                                              });
                                        })
                                    .value("Outputs: 0"))))
                        .merge(header_labels_container_props)),
                $View(
                    ViewProps::up()
                        .set_style($s("header.container"))
                        .set_children(children(
                            $Button(
                                ButtonProps::up()
                                    .set_style($s("header.button"))
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
                                            .set_style($s("header.label"))
                                            .value(locales::en::header_errors)))
                                    .click([this](lv_event_t* e) {
                                      this->show_errors_modal();
                                    })),
                            $Button(ButtonProps::up()
                                        .set_style($s("header.button"))
                                        .set_child($Text(
                                            TextProps::up()
                                                .set_style($s("header.label"))
                                                .value(locales::en::
                                                           header_information)))
                                        .click([this](lv_event_t* e) {
                                          this->show_info_modal();
                                        })),

                            $Button(
                                ButtonProps::up()
                                    .set_style($s("header.button"))
                                    .set_child($Text(
                                        TextProps::up()
                                            .set_style($s("header.label"))
                                            .value(
                                                locales::en::header_settings)))
                                    .click([navigator_ref](lv_event_t* e) {
                                      navigator_ref->navigate("/pin_code");
                                    }))))
                        .merge(header_container_right_props))))
            .merge(header_container_props));
  }

  $$View MainScreen::render_footer() const {
    return $View(
        ViewProps::up()
            .set_children(children($Button(
                ButtonProps::up()
                    .set_style($s("footer.button"))
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
                            .set_style($s("header.label"))
                            .value(locales::en::status))))))
            .set_overflow(true)
            .set_style([](Styling& style) {
              style.setPadding(0, 0, 16, 16);
              style.setBorderRadius(0);
              style.setBorder(lv_color_make(255, 255, 255), 0, 0);
            }));
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
              .w(125)
              .h(125));
    };

    return $View(
        ViewProps::up()
            .set_style($s("header.container"))
            .set_children(children(
                $Text(TextProps::up()
                          .set_style($s("header.label"))
                          .value(locales::en::oxygen_level)),
                $View(
                    ViewProps::up()
                        .set_style($s("common.no_padding"))
                        .set_children(children(make_circle("oxygen_level", 0),
                                               make_circle("oxygen_level", 1),
                                               make_circle("oxygen_level", 2)))
                        .w(LV_PCT(100))
                        .h(115)
                        .set_overflow(true)
                        .justify(LV_FLEX_ALIGN_SPACE_AROUND)
                        .items(LV_FLEX_ALIGN_CENTER)
                        .track_cross(LV_FLEX_ALIGN_CENTER)
                        .direction(LV_FLEX_FLOW_ROW)),
                $Text(TextProps::up()
                          .set_style($s("header.label"))
                          .value(locales::en::oxygen_rate)),
                $View(ViewProps::up()
                          .set_style($s("common.no_padding"))
                          .set_children(children(make_circle("oxygen_rate", 0),
                                                 make_circle("oxygen_rate", 1),
                                                 make_circle("oxygen_rate", 2)))
                          .w(LV_PCT(100))
                          .set_overflow(true)
                          .h(115)
                          .justify(LV_FLEX_ALIGN_SPACE_AROUND)
                          .items(LV_FLEX_ALIGN_CENTER)
                          .track_cross(LV_FLEX_ALIGN_CENTER)
                          .direction(LV_FLEX_FLOW_ROW))))
            .w(LV_PCT(100))
            .h(LV_PCT(66))
            .justify(LV_FLEX_ALIGN_START)
            .items(LV_FLEX_ALIGN_CENTER)
            .track_cross(LV_FLEX_ALIGN_START)
            .direction(LV_FLEX_FLOW_COLUMN));
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
                                      .value("06:10 AM")
                                      .set_style($s("status_bar.time"))),
                            $Text(TextProps::up()
                                      .value("ON2 Solution")
                                      .set_style($s("status_bar.logo"))),
                            $Text(TextProps::up()
                                      .watch<int>(
                                          &reactive_moto_lvgl,
                                          "reactive_moto_lvgl",
                                          [](Text* self, const int& value) {
                                            self->set_state(
                                                [value](TextProps& props) {
                                                  props.value(std::format(
                                                      "LVGL Seconds: {}",
                                                      value));
                                                });
                                          })
                                      .value("LVGL Seconds: 0")
                                      .set_style($s("status_bar.battery")))))),
                this->render_header(), this->render_body(),
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