
#include "info_screen.h";

namespace ON2Solutions {
  void InfoScreen::component_did_mount() {
    ESP_LOGI("preloader_screen", "Preloading screen");
  }
  $$CommonHeader InfoScreen::render_header() const {
    return $CommonHeader(CommonHeaderProps::up());
  }

  struct ErrorModel {
    std::string text;
    bool is_error;
  };

  std::vector<ErrorModel> InfoScreen::generate_test_data_models() {
    std::vector<ErrorModel> items;

    // Датчики (Error = true)
    for (int i = 1; i <= 4; ++i) {
      items.push_back(
          {"Oxygen sensor " + std::to_string(i) + " disconnected", true});
    }

    // Резервні канали (Error = false/true чергується)
    for (int i = 1; i <= 21; ++i) {
      items.push_back(
          {"Reserved channel (" + std::to_string(i) + ")", (i % 2 != 0)});
    }

    return items;
  }

  $$View InfoScreen::create_status_row(const std::string& label,
                                       bool is_error) {
    return $View(
        ViewProps::up()
            .w(680)  // Трохи менше ширини контейнера
            .h(45)
            .flow(FlexPreset::RowBetween)
            .set_style([](Styling& style) {
              style.setBackgroundColor(lv_color_hex(0xFFFFFF));
              style.setBorderRadius(8);
              style.setPadding(0, 15, 0, 15);  // Відступи всередині рядка
              style.setGap(0, 15);
            })
            .set_children(children(
                // Кольоровий статусний прямокутник
                $View(ViewProps::up().w(35).h(18).set_style(
                    [is_error](Styling& style) {
                      style.setBackgroundColor(is_error
                                                   ? lv_color_hex(0xE53935)
                                                   : lv_color_hex(0x4CAF50));
                      style.setBorderRadius(4);
                    })),
                // Текст повідомлення
                $Text(
                    TextProps::up().value(label).set_style([](Styling& style) {
                      style.setTextColor(lv_color_hex(0x333333));
                      style.setFont(&lv_font_montserrat_14);
                    })))));
  }

  $$View InfoScreen::render_body() {
    auto data = generate_test_data_models();

    return $View(ViewProps::up()
                     .w(LV_PCT(100))
                     .h(LV_PCT(100))
                     .direction(LV_FLEX_FLOW_COLUMN)
                     .justify(LV_FLEX_ALIGN_START)
                     .items(LV_FLEX_ALIGN_START)
                     .track_cross(LV_FLEX_ALIGN_START)
                     .set_style([](Styling& style) {
                       style.setBackgroundOpa(LV_OPA_0);
                       style.setPadding(0);
                       style.setBorder(PRIMARY_BG, 0, LV_OPA_0);
                     })
                     .set_children(children(
                         $Text(TextProps::up().value("Information")),
                         $PaginatedList(
                             PaginatedListProps()
                                 .w(720)
                                 .h(355)
                                 .set_items_per_page(4)
                                 .set_renderer(data.size(),
                                               [this, data](int index) {
                                                 return this->create_status_row(
                                                     data[index].text,
                                                     data[index].is_error);
                                               })
                                 .set_style([](Styling& style) {
                                   // style.align(LV_ALIGN_CENTER, 0, 0);
                                 })))));
  }

  lv_obj_t* InfoScreen::render() {
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

  InfoScreen* InfoScreen::append(lv_obj_t* obj) {
    lv_obj_set_parent(obj, get_component());
    return this;
  }
}  // namespace ON2Solutions
