#include "pincode_screen.hh"

using namespace foundation;

namespace ON2Solutions {
  void PinCodeScreen::component_did_mount() {};

  void PinCodeScreen::show_info_modal() {
    const auto login = this->login_state.get();
    const auto password = this->password_state.get();

    info_modal = $Modal(ModalProps::up().set_content(
        $View(ViewProps::up()
                  .set_children(children($Text(TextProps::up().value(
                      "Login or Password is incorrect, try again"))))
                  .w(LV_PCT(40))
                  .h(120)
                  .justify(LV_FLEX_ALIGN_CENTER)
                  .items(LV_FLEX_ALIGN_CENTER)
                  .track_cross(LV_FLEX_ALIGN_CENTER)
                  .direction(LV_FLEX_FLOW_COLUMN))));

    info_modal->show();
  }

  void PinCodeScreen::validate_and_login() {
    const auto login = this->login_state.get();
    const auto password = this->password_state.get();

    if (login.empty() || password.empty())
      return;
    if (login == USER_LOGIN && password == USER_PASSWORD) {
      this->navigation_ref->navigate("/settings");
    } else {
      show_info_modal();
    }
  }

  lv_obj_t* PinCodeScreen::render() {
    VNode::render();
    auto password_v = &this->password_state;
    auto login_v = &this->login_state;
    auto navigation_ref = this->navigation_ref;

    return this->delegate($View(
        ViewProps::up()
            .set_style([](Styling& style) {
              style.setTextColor(lv_color_make(255, 255, 255));
              style.setPadding(0, 0, 0, 0);
              style.setBorderRadius(0);
              style.setBorder(lv_color_make(255, 255, 255), 0, 0);
            })
            .set_children(children(
                $StatusBar(StatusBarProps::up()),
                $View(
                    ViewProps::up()
                        .w(LV_PCT(98))
                        .h(60)
                        .direction(LV_FLEX_FLOW_ROW)
                        .items(LV_FLEX_ALIGN_CENTER)
                        .track_cross(LV_FLEX_ALIGN_CENTER)
                        .justify(LV_FLEX_ALIGN_SPACE_BETWEEN)
                        .set_children(children(
                            $View(
                                ViewProps::up()
                                    .w(LV_PCT(20))
                                    .h(60)
                                    .direction(LV_FLEX_FLOW_ROW)
                                    .items(LV_FLEX_ALIGN_CENTER)
                                    .justify(LV_FLEX_ALIGN_START)
                                    .track_cross(LV_FLEX_ALIGN_CENTER)
                                    .set_style($s("common.no_padding"))
                                    .set_children(children($Button(
                                        ButtonProps::up()
                                            .set_child(
                                                $Text(TextProps::up().value(
                                                    locales::en::button_back)))
                                            .click([navigation_ref](
                                                       lv_event_t* e) {
                                              navigation_ref->goBack();
                                            }))))),
                            $View(ViewProps::up()
                                      .w(LV_PCT(78))
                                      .h(60)
                                      .set_style($s("common.no_padding"))
                                      .direction(LV_FLEX_FLOW_ROW)
                                      .items(LV_FLEX_ALIGN_CENTER)
                                      .justify(LV_FLEX_ALIGN_CENTER)
                                      .track_cross(LV_FLEX_ALIGN_CENTER)
                                      .set_children(
                                          children($Text(TextProps::up().value(
                                              locales::en::
                                                  system_auth_header))))),
                            $View(ViewProps::up()
                                      .w(LV_PCT(20))
                                      .set_style($s("common.no_padding"))
                                      .set_children(children(
                                          $Fragment(FragmentProps::up()))))))),
                $View(ViewProps::up()
                          .set_children(children(
                              $TextInput(
                                  TextInputProps::up()
                                      .set_is_one_line(true)
                                      .set_keyboard(keyboard)
                                      .hint("Login")
                                      .set_length(32)
                                      .on_changed_h(
                                          [login_v](const std::string& value) {
                                            login_v->set(value);
                                          })),
                              $TextInput(TextInputProps::up()
                                             .set_is_secure(true)
                                             .set_is_one_line(true)
                                             .set_keyboard(keyboard)
                                             .hint("Password")
                                             .set_length(64)
                                             .on_changed_h(
                                                 [password_v](
                                                     const std::string& value) {
                                                   password_v->set(value);
                                                 })),
                              $Button(ButtonProps::up()
                                          .set_child($Text(
                                              TextProps::up().value("Sign In")))
                                          .click([this](lv_event_t* e) {
                                            validate_and_login();
                                          }))))
                          .set_style($s("header.container"))
                          .merge(pincode_screen_sign_form_props))))
            .merge(screen_container_props)));
  }

  const Styling* PinCodeScreen::styling() const {
    this->style.setTextColor(lv_color_make(255, 255, 255));
    this->style.setPadding(0, 0, 0, 0);
    this->style.setBorderRadius(0);
    this->style.setBorder(lv_color_make(255, 255, 255), 0, 0);

    return &this->style;
  }

  PinCodeScreen* PinCodeScreen::append(lv_obj_t* obj) {
    lv_obj_set_parent(obj, get_component());
    return this;
  }
}  // namespace ON2Solutions