#include "charts_screen.h";

#include <ui/components/common_header/common_header.h>
#include <ui/components/side_bar/side_bar.h>
#include <ui/screens/main-screen/main_screen.h>

namespace ON2Solutions {
  void ChartsScreen::component_did_mount() {
    ESP_LOGI("preloader_screen", "Preloading screen");
  }

  void ChartsScreen::execute_status_trigger() const {
  //   auto command = SerializableCommand<const char*>{
  //     .command = SendableCommands::StatusCommand,
  // };
  //
  //   std::string serialized = serialize(command);
  //
  //   auto status = this->uart_handler->send(serialized);
  }

  $$CommonHeader ChartsScreen::render_header() const {
    return $CommonHeader(CommonHeaderProps::up());
  }

  $$View ChartsScreen::render_card() const {
    return $View(
        ViewProps::up()
            .w(318)
            .h(328)
            .direction(LV_FLEX_FLOW_COLUMN)
            .flow(FlexPreset::SpaceCenter)
            .set_style([](Styling& style) {

            })
            .set_children(children(
                //Circular
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

  $$View ChartsScreen::render_body() const {
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
                              $HighButton(assets::Right,
                                          [navigation](lv_event_t* _) {
                                            navigation->navigate("/main");
                                          }),
                              this->render_card(), this->render_card()))),
                this->render_footer())));
  }

  $$View ChartsScreen::render_footer() const {
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
                $FooterFragment()))
            .flow(FlexPreset::RowCenter)
            .set_style([](Styling& style) {
              style.setPadding(0);
              style.setGap(8, 8);
              style.setBorderRadius(0);
              style.setBorder(PRIMARY_BG, 0, 0);
            }));
  }

  lv_obj_t* ChartsScreen::render() {
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

  ChartsScreen* ChartsScreen::append(lv_obj_t* obj) {
    lv_obj_set_parent(obj, get_component());
    return this;
  }
}  // namespace ON2Solutions
