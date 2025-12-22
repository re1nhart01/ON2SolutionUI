#include "control_config.hh"

#include <utility>

#include "../../components/foundation/components/component.h"
#include "../../components/foundation/core/shortcuts.h"

#include "ui/styles/common_styles.h"
#include "core/state/state.h"

#include <esp_log.h>

using namespace foundation;

class PinCodeScreen;
struct PinCodeScreenProps final : BaseProps<PinCodeScreenProps, PinCodeScreen>{};

class PinCodeScreen : public NavigationScreen<PinCodeScreenProps>
{
  KeyboardManager keyboard;
  State<std::string> password_state = State<std::string>("");
  State<std::string> login_state = State<std::string>("");
  PinCodeScreenProps props;
  std::unique_ptr<StyleStorage> styles;
  std::shared_ptr<Modal> info_modal = nullptr;

public:
  explicit PinCodeScreen(const std::shared_ptr<StackNavigator> &stack, const PinCodeScreenProps &props)
  : NavigationScreen(stack, props),
    keyboard(KeyboardManager{}),
    props(props),
    styles(std::make_unique<StyleStorage>())
  {
    style_screen_register(*this->styles);
  }

  ~PinCodeScreen() override = default;

  void component_did_mount() override
  {

  };

  void show_info_modal()
  {
    const auto login = this->login_state.get();
    const auto password = this->password_state.get();

    info_modal = $Modal(
        ModalProps::up()
            .set_content(
                $View(ViewProps::up().set_children(Children{
                    $Text(TextProps::up().value("Login or Password is incorrect, try again")),
                })
            .w(LV_PCT(40))
            .h(120)
            .justify(LV_FLEX_ALIGN_CENTER)
            .items(LV_FLEX_ALIGN_CENTER)
            .track_cross(LV_FLEX_ALIGN_CENTER)
            .direction(LV_FLEX_FLOW_COLUMN))
            )
    );

    info_modal->show();
  }

  void validate_and_login()
  {
    const auto login = this->login_state.get();
    const auto password = this->password_state.get();

    if (login.empty() || password.empty()) return;
    if (login == USER_LOGIN && password == USER_PASSWORD)
    {
      this->navigation_ref->navigate("/settings");
    } else
    {
      show_info_modal();
    }
  }

  lv_obj_t *render() override {
    VNode::render();
    auto navigator_ref = this->navigation_ref;
    auto password_v = &this->password_state;
    auto login_v = &this->login_state;

    return this->delegate(
        $View(
            ViewProps::up()
                .set_style(this->styling())
                .set_children(Children{
                  $StatusBar(
                      StatusBarProps::up()
                          .set_style(nullptr)
                  ),
                  $View(ViewProps::up()
                    .set_children(Children{
                      $Button(ButtonProps::up()
                       .set_child(
                         $Text(
                             TextProps::up()
                                 .value("Back")
                         )
                       )
                       .click([navigator_ref](lv_event_t* e){
                           navigator_ref->goBack();
                       }))
                    })
                  .set_style($s("header.container"))
                  .merge(header_container_left_props)),
                  $View(ViewProps::up().set_children(Children{
                    $TextInput(TextInputProps::up()
                          .set_is_one_line(true)
                          .set_keyboard(&keyboard)
                          .hint("Login")
                          .set_length(32)
                          .on_changed_h([login_v](const std::string& value) {
                            login_v->set(value);
                    })),
                    $TextInput(TextInputProps::up()
                          .set_is_secure(true)
                          .set_is_one_line(true)
                          .set_keyboard(&keyboard)
                          .hint("Password")
                          .set_length(64)
                          .on_changed_h([password_v](const std::string& value) {
                            password_v->set(value);
                    })),
                    $Button(ButtonProps::up()
                       .set_child(
                         $Text(
                             TextProps::up()
                                 .value("Sign In")
                         )
                       )
                       .click([this](lv_event_t* e){
                         validate_and_login();
                       }))
                  })
                  .set_style($s("header.container"))
                  .merge(pincode_screen_sign_form_props)
                  ),
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

  PinCodeScreen* append(lv_obj_t *obj) override
  {
    lv_obj_set_parent(obj, get_component());
    return this;
  }
};
