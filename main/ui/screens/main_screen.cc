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
#include "lg/dataset/parser.cc"
#include "lg/store/global_store.h"
#include "protocols/uart/uart_proto.h"
#include "ui/localization.hh"
#include "ui/styles/common_styles.h"

#include <algorithm>

struct PinCodeScreenProps;
using namespace foundation;

class MainScreen;
struct MainScreenProps final : BaseProps<MainScreenProps, MainScreen> {};

class MainScreen final : public NavigationScreen<MainScreenProps> {
  MainScreenProps props;
  std::unique_ptr<StyleStorage> styles;
  std::unique_ptr<UartHandler> uart_handler = nullptr;
  std::unique_ptr<SharedRefStore<12>> ref_store;
  std::shared_ptr<Modal> info_modal = nullptr;
public:
  explicit MainScreen(StackNavigator* stack, const MainScreenProps &props)
    : NavigationScreen(stack, props),
      props(props),
      styles(std::make_unique<StyleStorage>()),
      ref_store(std::make_unique<SharedRefStore<12>>())
    {
        style_screen_register(*this->styles);
    }

  ~MainScreen() override {
    ESP_LOGI("main_screen", "Main screen destroyed");
  };

  void on_focus() override
  {
    NavigationScreen::on_focus();
    this->uart_handler = std::make_unique<UartHandler>(UART_NUM_1, GPIO_NUM_43, GPIO_NUM_44, 9600, 16384);
    ESP_LOGI("main_screen", "on_FOCUS");
      this->uart_handler->init();
      this->uart_handler->enable_rx(true);
      this->add_uart_data_event();
  };

  void on_blur() override
  {
    NavigationScreen::on_blur();
    this->uart_handler->remove_all_event_listeners();
  };


  void update_specific_label(const std::string& value, const std::shared_ptr<Ref<Text>>& ref)
  {
    if (ref->get() == nullptr && !ref->is_ready()) return;
    ref->get()->set_state(
      [value](TextProps& props) {
        props.value(value);
    });
  }

  void update_specific_circular(short value, const std::shared_ptr<Ref<CircularProgress>>& ref)
  {
    if (ref->get() == nullptr && !ref->is_ready()) return;
    ref->get()->set_state(
      [value](CircularProgressProps& props) {
        props.value(value);
    });
  }

  void show_info_modal()
  {
    info_modal = $Modal(
        ModalProps::up()
            .set_content(
                $View(ViewProps::up().set_children(Children{
                    $Text(TextProps::up().value("I am modal")),
                    $Button(
                        ButtonProps::up()
                            .set_style($s("header.button"))
                            .label("Press")
                            .click([this](lv_event_t*) {
                              this->info_modal->close();
                            })
                    ),
                })
            .justify(LV_FLEX_ALIGN_CENTER)
            .items(LV_FLEX_ALIGN_CENTER)
            .track_cross(LV_FLEX_ALIGN_CENTER)
            .direction(LV_FLEX_FLOW_COLUMN))
            )
    );

    info_modal->show();
  }

  void add_uart_data_event()
  {
    this->uart_handler->add_event_listener(UartTypes::UartHandlerEvent{
        .key_v = const_cast<char*>("read_data_dto"),
        .event = UART_DATA,
        .delegate = [this](const UartTypes::UartCallbackResponse& uart_data) {
            if (uart_data.response.packet.empty()) return;

            const DatasetDTO unpacked_uart_data = parse_into_dataset(uart_data.response.packet);

            struct AsyncUpdateContext {
                MainScreen* screen = nullptr;
                DatasetDTO data{};
            };

            auto* ctx = new AsyncUpdateContext{
                .screen = this,
                .data = unpacked_uart_data
            };

            lv_async_call(
                [](void* user_data) {
                    auto *ctx = static_cast<AsyncUpdateContext *>(user_data);

                    const auto text_channels_ref = ctx->screen->ref_store->get<Text>("text_channels");
                    const auto text_inputs_ref = ctx->screen->ref_store->get<Text>("text_inputs");
                    const auto text_outputs_ref = ctx->screen->ref_store->get<Text>("text_outputs");
                    const auto main_button_ref = ctx->screen->ref_store->get<Text>("main_button");
                    const auto status_bar_ref = ctx->screen->ref_store->get<Text>("status_bar_moto");
                    const auto status_bar_lvgl_ref = ctx->screen->ref_store->get<Text>("status_bar_lvgl");

                  ctx->screen->update_specific_label(
                        std::format("Channels: {}", ctx->data.channels),
                        text_channels_ref
                    );

                    ctx->screen->update_specific_label(
                        std::format("Inputs: {}", ctx->data.channels),
                        text_inputs_ref
                    );

                    ctx->screen->update_specific_label(
                        std::format("Outputs: {}", ctx->data.channels),
                        text_outputs_ref
                    );

                    ctx->screen->update_specific_label(
                        ctx->data.moto_hours,
                        status_bar_ref
                    );

                    ctx->screen->update_specific_label(
                      GetTextValueFromStatus(ctx->data.status),
                      main_button_ref
                    );

                    for (size_t i = 0; i < 3; i++)
                    {
                        auto oxygen_level = static_cast<short>(std::round(ctx->data.oxygen_levels[i]));
                        auto oxygen_rate = static_cast<short>(std::round(ctx->data.oxygen_speed[i]));

                        ctx->screen->update_specific_circular(
                          oxygen_level,
                          ctx->screen->ref_store->get<CircularProgress>(std::format("oxygen_level_{}", i))
                      );

                        ctx->screen->update_specific_circular(
                          oxygen_rate,
                          ctx->screen->ref_store->get<CircularProgress>(std::format("oxygen_rate_{}", i))
                       );
                    }

                    GlobalStore::getInstance()->getMotoHoursState()->set(
                      [ctx, status_bar_lvgl_ref](const uint32_t prev) {
                      if(prev == UINT32_MAX) return prev;
                      const auto newValue = prev + 1;

                      ctx->screen->update_specific_label(
                        std::format("LVGL Seconds: {}", newValue),
                        status_bar_lvgl_ref
                      );

                      return newValue;
                    });

                    delete ctx;
                },
                ctx
            );
        }
    });
  }

    std::shared_ptr<View> render_header() {
    auto navigator_ref = this->navigation_ref;

    return $View(
        ViewProps::up()
            .set_style($s("header.container"))
            .set_children(Children{
                $View(
                    ViewProps::up()
                        .set_style($s("header.labels.container"))
                        .set_children(Children{
                            $Text(TextProps::up().set_ref(this->ref_store->create<Text>("text_channels")).value("Channels: 0")),
                            $Text(TextProps::up().set_ref(this->ref_store->create<Text>("text_inputs")).value("Inputs: 0")),
                            $Text(TextProps::up().set_ref(this->ref_store->create<Text>("text_outputs")).value("Outputs: 0")),
                        })
                        .merge(header_labels_container_props)
                ),
                $View(
                    ViewProps::up()
                        .set_style($s("header.container"))
                        .set_children(Children{
                            $Button(
                                ButtonProps::up()
                                    .set_style($s("header.button"))
                                    .set_child(
                                        $Text(
                                            TextProps::up()
                                                .set_style($s("header.label"))
                                                .value(locales::en::header_information)
                                        )
                                    )
                                    .click([this](lv_event_t* e){
                                      this->show_info_modal();
                                    })
                            ),

                            $Button(
                                ButtonProps::up()
                                    .set_style($s("header.button"))
                                    .set_child(
                                        $Text(
                                            TextProps::up()
                                                .set_style($s("header.label"))
                                                .value(locales::en::header_settings)
                                        )
                                    )
                                    .click([navigator_ref](lv_event_t* e){
                                        navigator_ref->navigate("/pin_code");
                                    })
                            ),
                        })
                        .merge(header_container_right_props)
                ),
            })
            .merge(header_container_props)
    );
}



  std::shared_ptr<Button> render_footer() {
    return $Button(
        ButtonProps::up()
            .set_style($s("footer.button"))
            .set_child(
                $Text(
                    TextProps::up()
                        .set_ref(this->ref_store->create<Text>("main_button"))
                        .set_style($s("header.label"))
                        .value(locales::en::status)
                )
            )
    );
  }

  std::shared_ptr<View> render_body() {
    auto make_circle = [&](const std::string& ref_name) {
        return $Circular(
            CircularProgressProps::up()
                .set_ref(this->ref_store->create<CircularProgress>(ref_name))
                .label("%")
                .show_label(true)
                .min(0)
                .max(100)
                .value(0)
                .w(100)
                .h(100)
        );
    };

    return $View(
        ViewProps::up()
            .set_style($s("header.container"))
            .set_children(Children{
                $Text(
                    TextProps::up()
                        .set_style($s("header.label"))
                        .value(locales::en::oxygen_level)
                ),
                $View(
                    ViewProps::up()
                        .set_style($s("header.labels.container"))
                        .set_children(Children{
                            make_circle("oxygen_level_0"),
                            make_circle("oxygen_level_1"),
                            make_circle("oxygen_level_2"),
                        })
                        .w(LV_PCT(100))
                        .h(110)
                        .justify(LV_FLEX_ALIGN_SPACE_AROUND)
                        .items(LV_FLEX_ALIGN_CENTER)
                        .track_cross(LV_FLEX_ALIGN_CENTER)
                        .direction(LV_FLEX_FLOW_ROW)
                ),
                $Text(
                    TextProps::up()
                        .set_style($s("header.label"))
                        .value(locales::en::oxygen_rate)
                ),
                $View(
                    ViewProps::up()
                        .set_style($s("header.labels.container"))
                        .set_children(Children{
                            make_circle("oxygen_rate_0"),
                            make_circle("oxygen_rate_1"),
                            make_circle("oxygen_rate_2"),
                        })
                        .w(LV_PCT(100))
                        .h(110)
                        .justify(LV_FLEX_ALIGN_SPACE_AROUND)
                        .items(LV_FLEX_ALIGN_CENTER)
                        .track_cross(LV_FLEX_ALIGN_CENTER)
                        .direction(LV_FLEX_FLOW_ROW)
                ),
            })
            .w(LV_PCT(100))
            .h(LV_PCT(62))
            .justify(LV_FLEX_ALIGN_START)
            .items(LV_FLEX_ALIGN_CENTER)
            .track_cross(LV_FLEX_ALIGN_START)
            .direction(LV_FLEX_FLOW_COLUMN)
    );
}



  lv_obj_t* render() override {
    return this->delegate(
        $View(
            ViewProps::up()
                .set_style(this->styling())
                .set_children(Children{
                  $StatusBar(
                    StatusBarProps::up()
                        .set_background_color(lv_color_hex(0x2A2A2A))
                        .set_height(30)
                        .set_children(Children{
                            $Text(TextProps::up().set_ref(ref_store->create<Text>("status_bar_moto")).value("06:10 AM").set_style($s("status_bar.time"))),
                            $Text(TextProps::up().value("ON2 Solution").set_style($s("status_bar.logo"))),
                            $Text(TextProps::up().set_ref(ref_store->create<Text>("status_bar_lvgl")).value("LVGL Seconds: 0").set_style($s("status_bar.battery"))),
                        })
                    ),
                    this->render_header(),
                    this->render_body(),
                    this->render_footer(),
                })
                .merge(screen_container_props)
        )
    );
  }



  std::shared_ptr<Styling> styling() override {
    this->style = std::make_shared<Styling>();

    this->style->setTextColor(lv_color_make(255, 255, 255));
    this->style->setPadding(0, 0, 0, 0);
    this->style->setBorderRadius(0);
    this->style->setBorder(lv_color_make(255, 255, 255), 0, 0);

    return this->style;
  }

  MainScreen* append(lv_obj_t *obj) override
  {
    lv_obj_set_parent(obj, get_component());
    return this;
  }
};