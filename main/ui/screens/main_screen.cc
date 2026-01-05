#include "../../components/foundation/components/component.h"

#include "../../components/foundation/core/shortcuts.h"
#include "../../components/foundation/core/state/state.h"
#include "../../components/foundation/core/style_store/style_store.h"
#include "components/button/button_props.h"
#include "components/modal/modal_props.h"
#include "components/text/text_props.h"
#include "components/view/view.h"
#include "components/view/view_props.h"
#include "core/ref_store/ref_store.h"
#include "lg/dataset/deserializer.cc"
#include "lg/store/global_store.h"
#include "protocols/uart/uart_proto.h"
#include "ui/localization.hh"
#include "ui/styles/common_styles.h"
#include "ui/components/info_modal/info_modal.h"

#include <algorithm>
#include <lg/dataset/store/dataset.store.h>

struct PinCodeScreenProps;
using namespace foundation;

class MainScreen;
struct MainScreenProps final : BaseProps<MainScreenProps, MainScreen>
{};


class MainScreen final : public NavigationScreen<MainScreenProps>
{
  MainScreenProps props;
  std::unique_ptr<StyleStorage> styles;
  std::unique_ptr<UartHandler> uart_handler = nullptr;
  $$InfoModal info_modal = nullptr;
  TaskHandle_t xHandle = nullptr;
  Reactive<int> reactive_moto_lvgl;
public:
  explicit MainScreen(StackNavigator *stack, const MainScreenProps &props)
      : NavigationScreen(stack, props), props(props),
        styles(std::make_unique<StyleStorage>()),
        reactive_moto_lvgl(0)
  {
    style_screen_register(*this->styles);
  }

  ~MainScreen() override { ESP_LOGI("main_screen", "Main screen destroyed"); };

  void on_focus() override
  {
    NavigationScreen::on_focus();
    this->uart_handler = std::make_unique<UartHandler>(
      UART_NUM_2, GPIO_NUM_43, GPIO_NUM_44, 9600, 16384);
    // start_random_updater();
    ESP_LOGI("main_screen", "on_FOCUS");
    this->uart_handler->init();
    this->uart_handler->enable_rx(true);
    this->add_uart_data_event();
  };

  void on_blur() override
  {
    NavigationScreen::on_blur();
    this->uart_handler->remove_all_event_listeners();
    if (xHandle != nullptr) {
        vTaskDelete(xHandle);
        xHandle = nullptr;
    }
  };

  void update_specific_label(const std::string &value,
                             const std::shared_ptr<Ref<Text>> &ref) const
  {
    if(ref->get() == nullptr && !ref->is_ready())
      return;
    ref->get()->set_state([value](TextProps &props) { props.value(value); });
  }

  void update_specific_circular(short value,
                           const std::shared_ptr<Ref<CircularProgress>> &ref)
  {
    if(ref->get() == nullptr && !ref->is_ready())
      return;
    ref->get()->set_state(
      [value](CircularProgressProps &props) { props.value(value); });
  }

  void show_info_modal()
  {
    DatasetSystemInfo system_info = DatasetStore::getInstance()->get().system_info;

    info_modal = $InfoModal(InfoModalProps::up()
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
                              .set_restart_seconds(99));

    info_modal->show();
  }

  void start_random_updater() {
    xTaskCreate(
        [](void* pvParameters) {
            auto* self = static_cast<MainScreen*>(pvParameters);

            while (true) {
                if (self) {
                auto* packet_str = new std::string(CH_PACKETS[esp_random() % 15]);
                  lv_async_call([](void* arg) {
                      auto* p = static_cast<std::string*>(arg);
                      Dataset dataset = DatasetStore::getInstance()->get();
                      parse(&dataset, p->c_str(), p->length());
                      DatasetStore::getInstance()->set(dataset);

                      delete p;
                }, packet_str);
                }

                ESP_LOGI("main_screen", "start_random_updater, 2");

                vTaskDelay(pdMS_TO_TICKS(1000));
            }
        },
        "rand_task", 16384, this, 5, &xHandle
    );
  }

  void add_uart_data_event()
  {
    auto* moto_lvgl = &this->reactive_moto_lvgl;
    this->uart_handler->add_event_listener(UartTypes::UartHandlerEvent{
      .key_v = const_cast<char *>("read_data_dto"),
      .event = UART_DATA,
      .delegate = [moto_lvgl](const UartTypes::UartCallbackResponse &uart_data) {
        if(strlen(uart_data.response.packet) <= 0) return;

          struct AsyncStructure
          {
            std::string packet;
            Reactive<int>* reactive_moto_lvgl;
          };

          auto args = new AsyncStructure{ .packet = uart_data.response.packet, .reactive_moto_lvgl = moto_lvgl };

            lv_async_call([](void* arg) {
                auto* p = static_cast<AsyncStructure*>(arg);
                Dataset dataset = DatasetStore::getInstance()->get();
                parse(&dataset, p->packet.c_str(), p->packet.length());
                DatasetStore::getInstance()->set(dataset);
              if (p->reactive_moto_lvgl)
                {
                  p->reactive_moto_lvgl->set([](const int& prev){ return prev + 1; });
                }
                delete p;
          }, args);

      }});
  }

  $$View render_header()
  {
    auto navigator_ref = this->navigation_ref;

    return $View(
      ViewProps::up()
        .set_style($s("header.container"))
        .set_children(Children{
          $View(
            ViewProps::up()
              .set_style($s("header.labels.container"))
              .set_children(Children{
                $Text(
                  TextProps::up()
                    .watch<Dataset>(DatasetStore::getInstance(), "channels", [](Text* self, const Dataset& value) {
                      int count = value.operative_data.channels_count;
                      self->set_state([count](TextProps &props) {
                          props.value(std::format("Channels: {}", count));
                      });
                    })
                    .value("Channels: 0")),
                $Text(TextProps::up()
                        .watch<Dataset>(DatasetStore::getInstance(), "inputs", [](Text* self, const Dataset& value) {
                          int inputs = value.operative_data.inputs;
                          self->set_state([inputs](TextProps &props) {
                             props.value(std::format("Inputs: {}", inputs));
                          });
                        })
                        .value("Inputs: 0")),
                $Text(TextProps::up()
                        .watch<Dataset>(DatasetStore::getInstance(), "outputs", [](Text* self, const Dataset& value) {
                          int outputs = value.operative_data.outputs;
                          self->set_state([outputs](TextProps &props) {
                              props.value(std::format("Outputs: {}", outputs));
                          });
                        })
                        .value("Outputs: 0")),
              })
              .merge(header_labels_container_props)),
          $View(ViewProps::up()
                  .set_style($s("header.container"))
                  .set_children(Children{
                    $Button(ButtonProps::up()
                              .set_style($s("header.button"))
                              .set_child($Text(
                                TextProps::up()
                                  .set_style($s("header.label"))
                                  .value(locales::en::header_information)))
                              .click([this](lv_event_t *e) {
                                this->show_info_modal();
                              })),

                    $Button(ButtonProps::up()
                              .set_style($s("header.button"))
                              .set_child(
                                $Text(TextProps::up()
                                        .set_style($s("header.label"))
                                        .value(locales::en::header_settings)))
                              .click([navigator_ref](lv_event_t *e) {
                                navigator_ref->navigate("/pin_code");
                              })),
                  })
                  .merge(header_container_right_props)),
        })
        .merge(header_container_props));
  }

  $$Button render_footer()
  {
    return $Button(ButtonProps::up()
                     .set_style($s("footer.button"))
                     .set_child($Text(
                       TextProps::up()
                         .watch<Dataset>(DatasetStore::getInstance(), "main_button", [](Text* self, const Dataset& value) {
                            std::string status_str = GetTextValueFromStatus(GetStatusFromTextValue(value.operative_data.status.data()));
                            self->set_state([status_str](TextProps &props) {
                                props.value(status_str);
                            });
                          })
                         .set_style($s("header.label"))
                         .value(locales::en::status))));
  }

  $$View render_body()
  {
    auto make_circle = [&](const std::string &ref_name, int index) {
      return $Circular(
        CircularProgressProps::up()
          .watch<Dataset>(DatasetStore::getInstance(), "outputs", [index, ref_name](CircularProgress* self, const Dataset& value) {
            short val = (ref_name == "oxygen_level")
                        ? value.operative_data.oxygen_levels[index]
                        : value.operative_data.oxygen_speed[index];

            self->set_state([val](CircularProgressProps &props) {
                props.value(val);
            });
          })
          .label("%")
          .show_label(true)
          .min(0)
          .max(100)
          .value(0)
          .w(100)
          .h(100));
    };

    return $View(ViewProps::up()
                   .set_style($s("header.container"))
                   .set_children(Children{
                     $Text(TextProps::up()
                             .set_style($s("header.label"))
                             .value(locales::en::oxygen_level)),
                     $View(ViewProps::up()
                             .set_style($s("header.labels.container"))
                             .set_children(Children{
                               make_circle("oxygen_level", 0),
                               make_circle("oxygen_level", 1),
                               make_circle("oxygen_level", 2),
                             })
                             .w(LV_PCT(100))
                             .h(110)
                             .justify(LV_FLEX_ALIGN_SPACE_AROUND)
                             .items(LV_FLEX_ALIGN_CENTER)
                             .track_cross(LV_FLEX_ALIGN_CENTER)
                             .direction(LV_FLEX_FLOW_ROW)),
                     $Text(TextProps::up()
                             .set_style($s("header.label"))
                             .value(locales::en::oxygen_rate)),
                     $View(ViewProps::up()
                             .set_style($s("header.labels.container"))
                             .set_children(Children{
                               make_circle("oxygen_rate", 0),
                               make_circle("oxygen_rate", 1),
                               make_circle("oxygen_rate", 2),
                             })
                             .w(LV_PCT(100))
                             .h(110)
                             .justify(LV_FLEX_ALIGN_SPACE_AROUND)
                             .items(LV_FLEX_ALIGN_CENTER)
                             .track_cross(LV_FLEX_ALIGN_CENTER)
                             .direction(LV_FLEX_FLOW_ROW)),
                   })
                   .w(LV_PCT(100))
                   .h(LV_PCT(62))
                   .justify(LV_FLEX_ALIGN_START)
                   .items(LV_FLEX_ALIGN_CENTER)
                   .track_cross(LV_FLEX_ALIGN_START)
                   .direction(LV_FLEX_FLOW_COLUMN));
  }

  lv_obj_t *render() override
  {
    return this->delegate($View(
      ViewProps::up()
            .set_style([](Styling& style) {
              style.setTextColor(lv_color_make(255, 255, 255));
              style.setPadding(0, 0, 0, 0);
              style.setBorderRadius(0);
              style.setBorder(lv_color_make(255, 255, 255), 0, 0);
            })
            .set_children(Children{
          $StatusBar(
            StatusBarProps::up()
              .set_background_color(lv_color_hex(0x2A2A2A))
              .set_height(30)
              .set_children(Children{
                $Text(TextProps::up()
                        .watch<Dataset>(DatasetStore::getInstance(), "moto_hours", [](Text* self, const Dataset& value) {
                          std::string hours = value.operative_data.moto_hours.data();
                          self->set_state([hours](TextProps &props) {
                          props.value(hours);
                        });
                        })
                        .value("06:10 AM")
                        .set_style($s("status_bar.time"))),
                $Text(TextProps::up()
                        .value("ON2 Solution")
                        .set_style($s("status_bar.logo"))),
                $Text(TextProps::up()
                        .watch<int>(&reactive_moto_lvgl, "reactive_moto_lvgl", [](Text* self, const int& value) {
                            self->set_state([value](TextProps& props) { props.value(std::format("LVGL Seconds: {}", value)); });
                        })
                        .value("LVGL Seconds: 0")
                        .set_style($s("status_bar.battery"))),
              })),
          this->render_header(),
          this->render_body(),
          this->render_footer(),
        })
        .merge(screen_container_props)));
  }

  const Styling* styling() const override
  {
    this->style.setTextColor(lv_color_make(255, 255, 255));
    this->style.setPadding(0, 0, 0, 0);
    this->style.setBorderRadius(0);
    this->style.setBorder(lv_color_make(255, 255, 255), 0, 0);

    return &this->style;
  }

  MainScreen *append(lv_obj_t *obj) override
  {
    lv_obj_set_parent(obj, get_component());
    return this;
  }
};