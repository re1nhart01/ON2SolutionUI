#include "main_screen.h"

#include <hal/uart_ll.h>
#include "constants/theme.h"

using namespace foundation;
using namespace ON2Solutions::parser;

namespace ON2Solutions {
  void MainScreen::on_focus() {
    NavigationScreen::on_focus();
  };

  void MainScreen::on_blur() {
    NavigationScreen::on_blur();
  };

  void MainScreen::execute_status_trigger() const {
    if (!this->props.uart) {
      return;
    }

    auto command = SerializableCommand<const char*>{
        .command = SendableCommands::StatusCommand,
    };

    std::string serialized = serialize(command);

    auto _ = this->props.uart->send(serialized);
  }

  template <typename C>
  void MainScreen::update_styles(Component<C>* component,
                                 const Delegate<void(Styling&)>& style) {
    component->set_state([style](C& props) { props.set_style(style); });
  }

#pragma region UI
  $$CommonHeader MainScreen::render_header() const {
    return $CommonHeader(CommonHeaderProps::up());
  }

  $$View MainScreen::render_card(const CircularSelectorType& type,
                                 const char* title, uint8_t index) const {
    return $View(ViewProps::up()
                     .w(318)
                     .h(328)
                     .direction(LV_FLEX_FLOW_COLUMN)
                     .flow(FlexPreset::SpaceCenter)
                     .set_style([](Styling& style) {

                     })
                     .set_children(children(
                         $SpecificCircular(type, index, false),
                         $View(ViewProps::up()
                                   .w(LV_PCT(100))
                                   .h(24)
                                   .set_style([](Styling& style) {
                                     style.setGap(8, 8);
                                     style.setPadding(0);
                                     style.setBorder(PRIMARY_BG, 0, LV_OPA_0);
                                   })
                                   .flow(FlexPreset::RowCenter)
                                   .set_children(children(
                                       $ConnectionStat(),
                                       $Text(TextProps::up().value(fmt_str(
                                           "%s %d", title, index + 1)))))))));
  }

  $$View MainScreen::render_body() const {
    auto navigation = navigation_ref;

    return $View(
        ViewProps::up()
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
                $View(
                    ViewProps::up()
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
                            this->render_card(O2, locales::en::oxygen_level, 0),
                            this->render_card(Ps, locales::en::tank_psi, 0),
                            $HighButton(assets::Right,
                                        [navigation](lv_event_t* _) {
                                          navigation->navigate(
                                              "/charts", {{"page", 0}}, false);
                                        })))),
                this->render_footer())));
  }

  $$View MainScreen::render_footer() const {
    return $View(
        ViewProps::up()
            .set_children(children(
                $TimerView(this->alarm_control),
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
                              if (!self) return;
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
                                      if (!self) return;
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
                $FooterFragment()))
            .flow(FlexPreset::RowCenter)
            .set_style([](Styling& style) {
              style.setPadding(0);
              style.setGap(8, 8);
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