#include "pincode_screen.h"

#include "constants/assets.h"
#include "control_config.hh"
#include "lg/dataset/serializer.h"
#include "ui/styles/common_styles.h"

using namespace foundation;

namespace ON2Solutions {
  void PinCodeScreen::component_did_mount() {
    execute_typed_command(this->props.uart, parser::SendableCommands::RequestData, static_cast<int>(parser::AllRequest));
  };

  void PinCodeScreen::show_info_modal() {
    const auto login = this->login_state.get();

    info_modal = $Modal(ModalProps::up().set_content(
        $View(ViewProps::up()
                  .set_style([](Styling& style) {
                    style.setBackgroundColor(ERROR_COLOR_3);
                    style.setBorder(ERROR_COLOR, 1, LV_OPA_100);
                  })
                  .set_children(children(
                      $Text(TextProps::up()
                                .set_style([](Styling& style) {
                                  style.setTextColor(ERROR_COLOR);
                                })
                                .value("Pin code is incorrect, try again"))))
                  .w(LV_PCT(40))
                  .h(120)
                  .justify(LV_FLEX_ALIGN_CENTER)
                  .items(LV_FLEX_ALIGN_CENTER)
                  .track_cross(LV_FLEX_ALIGN_CENTER)
                  .direction(LV_FLEX_FLOW_COLUMN))));

    info_modal->show();
  }

  $$Button PinCodeScreen::render_nav_button() const {
    if (this->navigation_ref == nullptr)
      return $Button(ButtonProps::up());

    return $Button(
        ButtonProps::up()
            .click([this](lv_event_t*) {
              this->navigation_ref->navigate("/main", false);
            })
            .set_style([](Styling& style) {
              style.set_clear_default();
              style.setWidth(32);
              style.setHeight(32);
              style.setPadding(0);
              style.setBorderRadius(8);
              style.setBackgroundOpacity(LV_OPA_100);
              style.setBackgroundColor(TERTIARY_BG);
            })
            .set_child($Image(
                ImageProps::up().source(assets::Close).width(10).height(10)))

    );
  }

  void PinCodeScreen::validate_and_login() {
    const auto login = this->login_state.get();

    if (login.empty())
      return;
    if (login == USER_LOGIN) {
      execute_typed_command(this->props.uart,
                            parser::SendableCommands::RequestData,
                          static_cast<int>(parser::SettingsRequest));
      vTaskDelay(pdMS_TO_TICKS(200));
      this->navigation_ref->navigate("/settings");
    } else {
      show_info_modal();
    }
  }

  lv_obj_t* PinCodeScreen::render() {
    VNode::render();
    auto login_v = &this->login_state;

    return this->delegate($View(
        ViewProps::up()
            .set_style([](Styling& style) {
              style.setPadding(0);
              style.setBorderRadius(0);
              style.setBorder(PRIMARY_BG, 0, LV_OPA_0);
              style.setBackgroundImage(assets::Background1);
            })
            .set_children(children($View(
                ViewProps::up()
                    .set_style([](Styling& style) {
                      style.setBackgroundColor(PRIMARY_BG);
                      style.setBorderRadius(16);
                      style.setBorder(BORDER_SECONDARY, 1, LV_OPA_100);
                    })
                    .set_children(children(
                        $View(
                            ViewProps::up()
                                .w(LV_PCT(100))
                                .h(40)
                                .direction(LV_FLEX_FLOW_ROW)
                                .items(LV_FLEX_ALIGN_CENTER)
                                .track_cross(LV_FLEX_ALIGN_CENTER)
                                .justify(LV_FLEX_ALIGN_SPACE_BETWEEN)
                                .set_style(NoPaddingApply)
                                .set_children(children(
                                    $View(ViewProps::up()
                                              .w(LV_PCT(20))
                                              .h(40)
                                              .direction(LV_FLEX_FLOW_ROW)
                                              .items(LV_FLEX_ALIGN_CENTER)
                                              .justify(LV_FLEX_ALIGN_START)
                                              .track_cross(LV_FLEX_ALIGN_CENTER)
                                              .set_style(NoPaddingApply)
                                              .set_children(children(
                                                  this->render_nav_button()))),
                                    $View(
                                        ViewProps::up()
                                            .w(LV_PCT(78))
                                            .h(40)
                                            .set_style(NoPaddingApply)
                                            .direction(LV_FLEX_FLOW_ROW)
                                            .items(LV_FLEX_ALIGN_CENTER)
                                            .justify(LV_FLEX_ALIGN_CENTER)
                                            .track_cross(LV_FLEX_ALIGN_CENTER)
                                            .set_children(children(
                                                $Text(TextProps::up().value(
                                                    locales::en::
                                                        system_auth_header))))),
                                    $View(ViewProps::up()
                                              .w(LV_PCT(20))
                                              .set_style(NoPaddingApply)
                                              .set_children(children($Fragment(
                                                  FragmentProps::up()))))))),
                        $TextInput(TextInputProps::up()
                                       .set_is_one_line(true)
                                       .set_disabled(true)
                                       .hint("Pin")
                                       .watch<std::string>(
                                           &this->login_state, "login",
                                           [](const TextInput* self,
                                              const std::string& value) {
                                             if (!self)
                                               return;
                                             self->set_text(value.c_str());
                                           })
                                       .set_length(32)),
                        $Matrix(MatrixProps::up()
                                    .set_style([](Styling& style) {
                                      style.setWidth(250);
                                      style.setHeight(250);
                                      style.setBorder(PRIMARY_BG, 0, LV_OPA_0);
                                      style.setBorderRadius(0);
                                    })
                                    .set_btn_style([](Styling& style) {
                                      style.setBorderRadius(8);
                                      style.setBorder(PRIMARY_BG, 0, LV_OPA_0);
                                      style.setBackgroundOpa(0);
                                      style.setBackgroundColor(TERTIARY_BG);
                                      style.setFont(&lv_font_montserrat_14);
                                      style.setShadow(PRIMARY_BG, 0, LV_OPA_0);
                                    })
                                    .set_btn_map(btnm_map, LV_SYMBOL_LEFT)
                                    .set_btn_bg_color(TERTIARY_BG)
                                    .change([login_v,
                                             this](const std::string& value) {
                                      auto curr = login_state.get();
                                      if (value != "BACKSPACE") {
                                        login_v->set(curr + value);
                                      } else {
                                        auto new_value =
                                            curr.size() == 0
                                                ? curr
                                                : curr.substr(0,
                                                              curr.size() - 1);
                                        login_v->set(new_value);
                                      }
                                    })),
                        $Button(ButtonProps::up()
                                    .set_style([](Styling& style) {
                                      style.setFont(&lv_font_montserrat_14);
                                      style.setBackgroundColor(PRIMARY_COLOR_2);
                                      style.setBorderRadius(12);
                                      style.setSize(250, 44);
                                      style.setBorder(lv_color_hex(0x5B5AFF), 0,
                                                      0);
                                      style.setPadding(8, 8, 16, 16);
                                      style.setBorderRadius(14);
                                    })
                                    .set_child($Text(TextProps::up().value(
                                        locales::en::sign_in)))
                                    .click([this](lv_event_t* e) {
                                      validate_and_login();
                                    }))))
                    .merge(pincode_screen_sign_form_props))))
            .merge(centered_screen_container_props)));
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