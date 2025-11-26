#include <utility>

#include "../../components/foundation/components/component.h"

#include "../../components/foundation/core/shortcuts.h"
#include "../../components/foundation/core/state/state.h"
#include "../../components/foundation/core/style_store/style_store.h"
#include "components/button/button_props.h"
#include "components/text/text_props.h"
#include "components/view/view.h"
#include "components/view/view_props.h"
#include "protocols/uart/uart_proto.h"
#include "ui/localization.hh"
#include "ui/styles/main_screen_styles.cc"
#include "lg/dataset/parser.cc"

struct PinCodeScreenProps;
using namespace foundation;

class MainScreen;
struct MainScreenProps final : BaseProps<MainScreenProps, MainScreen> {};

class MainScreen final : public NavigationScreen<MainScreenProps> {
  MainScreenProps props;
  StyleStorage* styles = nullptr;
  UartHandler* uart_handler = nullptr;
  std::shared_ptr<Ref<Text>> text_ref = nullptr;
public:
  explicit MainScreen(const std::shared_ptr<StackNavigator> &stack, const MainScreenProps &props) : NavigationScreen(stack, props) {
    this->props = props;
    this->styles = new StyleStorage();
    style_main_screen_register(*this->styles);
    this->text_ref = std::make_shared<Ref<Text>>("text");

  }

  ~MainScreen() override {
    NavigationScreen::~NavigationScreen();
    delete this->styles;
    delete this->uart_handler;
    ESP_LOGI("main_screen", "Main screen destroyed");
  };

  std::shared_ptr<Styling> $s(const std::string& key) const
  {
    return this->styles->get(key);
  }

  void on_focus() override
  {
    NavigationScreen::on_focus();
    this->uart_handler = new UartHandler(UART_NUM_1, GPIO_NUM_6, GPIO_NUM_7, 9600, 16384);
    ESP_LOGI("main_screen", "on_FOCUS");
      this->uart_handler->init();
      this->uart_handler->enable_rx(true);
      auto text_reference = this->text_ref;

      this->uart_handler->add_event_listener(UartTypes::UartHandlerEvent{
        .key_v = const_cast<char*>("read_data_dto"),
        .event = UART_DATA,
        .delegate = [text_reference](const UartTypes::UartCallbackResponse& data) {
          if (text_reference->is_ready())
            {
              text_reference->get()->set_state([data](TextProps& props) {
                const std::string rand_str = packets[random_in_range(0, 9)];
                const auto dataset = parse_into_dataset(rand_str);

                props.text = std::to_string(dataset.oxygen_levels[0]);
              });
            }
         }
      });
  };

  void on_blur() override
  {
    NavigationScreen::on_blur();
    this->uart_handler->remove_all_event_listeners();
  };

    std::shared_ptr<View> render_header() const {
    auto navigator_ref = this->navigation_ref;

    return $View(
        ViewProps::up()
            .set_style($s("header.container"))
            .set_children(Children{
                $View(
                    ViewProps::up()
                        .set_style($s("header.labels.container"))
                        .set_children(Children{
                            $Text(TextProps::up().value(std::format("Channels: {}", 3))),
                            $Text(TextProps::up().value(std::format("Inputs: {}", 3))),
                            $Text(TextProps::up().value(std::format("Outputs: {}", 3))),
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
                                    .click([navigator_ref](lv_event_t* e){
                                        navigator_ref->navigate("/pincode");
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



  std::shared_ptr<Button> render_footer() const {
    return $Button(
        ButtonProps::up()
            .set_style($s("footer.button"))
            .set_child(
                $Text(
                    TextProps::up()
                        .set_style($s("header.label"))
                        .value(locales::en::status)
                )
            )
    );
  }

  std::shared_ptr<View> render_body() const {
    auto make_circle = [&]() {
        return $Circular(
            CircularProgressProps::up()
                .label("%")
                .show_label(true)
                .min(0)
                .max(100)
                .value(20)
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
                        .set_ref(text_ref)
                        .set_style($s("header.label"))
                        .value(locales::en::oxygen_level)
                ),
                $View(
                    ViewProps::up()
                        .set_style($s("header.labels.container"))
                        .set_children(Children{
                            make_circle(),
                            make_circle(),
                            make_circle(),
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
                            make_circle(),
                            make_circle(),
                            make_circle(),
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
                            .set_style(nullptr)
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