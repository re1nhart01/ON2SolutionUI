#include "charts_screen.h"

#include <ui/components/common_header/common_header.h>
#include <ui/components/side_bar/side_bar.h>
#include <ui/screens/main-screen/main_screen.h>

#include "lg/dataset/store/flash_store.h"

namespace ON2Solutions {
  void ChartsScreen::component_did_mount() {
    ESP_LOGI("preloader_screen", "Preloading screen");
  }

  void ChartsScreen::execute_status_trigger() const {
    auto command = SerializableCommand<const char*>{
        .command = SendableCommands::StatusCommand,
    };

    std::string serialized = serialize(command);

    auto _ = this->props.uart->send(serialized);
  }

  $$CommonHeader ChartsScreen::render_header() const {
    return $CommonHeader(CommonHeaderProps::up());
  }

  $$View ChartsScreen::render_temp_with_unit(int circular_index) const {
    auto current_unit = FlashStore::getInstance()->get_value<std::string>(
                            TEMPERATURE_UNIT, TEMPERATURE_UNIT_OPTIONS[0]) ==
                                TEMPERATURE_UNIT_OPTIONS[0]
                            ? Tnc
                            : Tnf;

    return this->render_card(current_unit, locales::en::temperature,
                             circular_index, true);
  }

  $$View ChartsScreen::render_card(const CircularSelectorType& type,
                                   const char* title, uint8_t index,
                                   bool is_small = false) const {
    return $View(ViewProps::up()
                     .w(170)
                     .h(LV_PCT(100))
                     .direction(LV_FLEX_FLOW_COLUMN)
                     .flow(FlexPreset::SpaceCenter)
                     .set_style([](Styling& style) {
                       style.setBackgroundColor(PRIMARY_BG);
                       style.setBackgroundOpa(LV_OPA_0);
                       style.setBorder(PRIMARY_BG, 0, LV_OPA_0);
                       style.setPadding(0);
                     })
                     .set_children(children(
                         $SpecificCircular(type, index, is_small),
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

  $$View ChartsScreen::render_circular_wrapper(
      std::vector<std::unique_ptr<VNode>> childs,
      bool is_around = false) const {

    auto is_last_page = get_param<int>(this->props.params, "page", 0) == 1;

    return $View(ViewProps::up()
                     .w(is_last_page ? 648 : 648 - 64)
                     .h(158)
                     .flow(FlexPreset::RowBetween)
                     .justify(is_around ? LV_FLEX_ALIGN_SPACE_AROUND
                                        : LV_FLEX_ALIGN_SPACE_BETWEEN)
                     .set_style([](Styling& style) {
                       style.setBackgroundColor(PRIMARY_BG);
                       style.setPadding(12);
                       style.setBorderRadius(16);
                     })
                     .set_children(std::move(childs)));
  }

  $$View ChartsScreen::render_body() const {
    auto navigation = navigation_ref;

    auto circular_index = get_param<int>(this->props.params, "page", 0);
    auto is_last_page = circular_index == 1;

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
                            $HighButton(
                                assets::Left,
                                [navigation, is_last_page](lv_event_t* _) {
                                  navigation->navigate(
                                      is_last_page ? "/charts" : "/main",
                                      {{"page", 0}}, false);
                                }),
                            $View(
                                ViewProps::up()
                                    .w(LV_SIZE_CONTENT)
                                    .h(LV_SIZE_CONTENT)
                                    .flow(FlexPreset::ColumnCenter)
                                    .set_style(HideAllApply)
                                    .set_children(children(
                                        this->render_circular_wrapper(children(
                                            this->render_card(
                                                O2, locales::en::oxygen_level,
                                                circular_index, true),
                                            this->render_card(
                                                Fl, locales::en::oxygen_rate,
                                                circular_index, true),
                                            this->render_card(
                                                Ps, locales::en::tank_pressure,
                                                circular_index, true))),
                                        this->render_circular_wrapper(
                                            children(
                                                this->render_card(
                                                    Tr,
                                                    locales::en::booster_psi,
                                                    circular_index, true),
                                                this->render_temp_with_unit(
                                                    circular_index)),
                                            true)))

                                    ),

                            !is_last_page ? static_cast<VNodePtr>($HighButton(
                                                assets::Right,
                                                [navigation](lv_event_t* _) {
                                                  navigation->navigate(
                                                      "/charts", {{"page", 1}},
                                                      false);
                                                }))
                                          : static_cast<VNodePtr>($Fragment(
                                                FragmentProps::up()))))),
                this->render_footer())));
  }

  $$View ChartsScreen::render_footer() const {

    const auto dataset = DatasetStore::getInstance()->get();

    const auto current_status = dataset->operative_data.status;
    uint8_t count = dataset->optional.reset_countdown_sec;

    const std::string status_str =
        GetTextValueFromStatus(GetStatusFromTextValue(current_status.data()));

    const auto current_opacity =
        GetStatusFromTextValue(current_status.data()) == DatasetStatuses::Alarm
            ? LV_OPA_100
            : LV_OPA_0;

    return $View(
        ViewProps::up()
            .set_children(children(
                $TimerView(this->alarm_control, fmt_str("%d", count),
                           current_opacity),
                $Button(
                    ButtonProps::up()
                        .set_style([current_status](Styling& style) {
                          style.setFont(&lv_font_montserrat_16);
                          style.setBackgroundColor(button_color_by_status(
                              GetStatusFromTextValue(current_status.data())));
                          style.setBorderRadius(12);
                          style.setSize(300, 44);
                          style.setBorder(lv_color_hex(0x5B5AFF), 0, 0);
                          style.setPadding(8, 8, 16, 16);
                          style.setBorderRadius(14);
                        })
                        .watch<Dataset>(
                            DatasetStore::getInstance(), "main_button_body",
                            [](Button* self, const Dataset& value) {
                              if (!self)
                                return;
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
                                      if (!self)
                                        return;
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
                                .value(status_str)))
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
                              $Sidebar(
                                  SidebarProps::up()
                                      .set_uart_handler(this->props.uart.get())
                                      .set_stack(this->navigation_ref)),
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
