#include "errors_screen.h";

#include <constants/localization.h>

namespace ON2Solutions {
  void ErrorsScreen::component_did_mount() {
    ESP_LOGI("preloader_screen", "Preloading screen");
  }

  $$CommonHeader ErrorsScreen::render_header() const {
    return $CommonHeader(CommonHeaderProps::up());
  }

  struct ErrorModel {
    const char* text;
    bool is_error;
  };

  std::vector<ErrorModel> ErrorsScreen::generate_data_models() const {
    std::vector<ErrorModel> items;

    const auto error_bits =
        DatasetStore::getInstance()->get().operative_data.errors;

    for (size_t i = 0; i < std::size(locales::en::info_bits_error); i++) {
      if (is_up_bit_pos(error_bits, static_cast<short>(i))) {
        items.push_back({locales::en::info_bits_error[i], true});
      }
    }

    return items;
  };

  $$View ErrorsScreen::create_status_row(const std::string& label,
                                         bool is_error, int index) {
    return $View(ViewProps::up()
                     .w(696)
                     .h(38)
                     .flow(FlexPreset::RowStart)
                     .set_style([index](Styling& style) {
                       style.setBackgroundColor(index % 2 == 0 ? TERTIARY_BG
                                                               : PRIMARY_BG);
                       style.setPadding(0, 0, 16, 16);
                       style.setBorder();
                       style.setGap(0, 20);
                       style.setBorderRadius(8);
                     })
                     .set_children(children(
                         $View(ViewProps::up().w(28).h(16).set_style(
                             [is_error](Styling& style) {
                               style.setBackgroundColor(
                                   is_error ? ERROR_COLOR : NO_ERROR_COLOR);
                               style.setBorderRadius(4);
                               style.setBorder();
                             })),
                         $Text(TextProps::up().value(label).set_style(
                             [](Styling& style) {
                               style.setTextColor(lv_color_hex(0x333333));
                               style.setFont(&lv_font_montserrat_14);
                             })))));
  }

  $$View ErrorsScreen::render_body() {
    const auto initial_data = this->generate_data_models();

    return $View(
        ViewProps::up()
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
                $Text(TextProps::up()
                          .value(locales::en::header_errors)
                          .set_style([](Styling& style) {
                            style.setFont(&lv_font_montserrat_18);
                          })),
                $PaginatedList(
                    PaginatedListProps()
                        .w(712)
                        .h(355)
                        .watch<parser::Dataset>(
                            parser::DatasetStore::getInstance(), "error",
                            [this](PaginatedList* self,
                                   const parser::Dataset& value) {
                              if (!self)
                                return;

                              static uint32_t last_update = 0;
                              static uint32_t last_error_bits = UINT32_MAX;
                              const uint32_t error_bits =
                                  value.operative_data.errors;

                              const uint32_t now = xTaskGetTickCount();
                              constexpr uint32_t interval = pdMS_TO_TICKS(250);

                              if (error_bits == last_error_bits || now - last_update < interval) {
                                return;
                              }

                              last_update = now;

                              const auto new_data =
                                  this->generate_data_models();

                              self->set_state(
                                  [this, new_data](PaginatedListProps& props) {
                                    props.set_renderer(
                                        new_data.size(),
                                        [this, new_data](const int index) {
                                          return this->create_status_row(
                                              new_data[index].text,
                                              new_data[index].is_error, index);
                                        });
                                  });
                            })
                        .set_renderer(initial_data.size(),
                                      [this, initial_data](const int index) {
                                        return this->create_status_row(
                                            initial_data[index].text,
                                            initial_data[index].is_error,
                                            index);
                                      })
                        .set_items_per_page(6)
                        .set_style([](Styling& style) {
                          style.setBorder(PRIMARY_BG, 0, LV_OPA_0);
                          style.setShadow(lv_color_hex(0x1018281A), 12, 0);
                        })))));
  }

  lv_obj_t* ErrorsScreen::render() {
    VNode::render();

    return this->delegate($View(
        ViewProps::up()
            .set_style([](Styling& style) {
              style.setTextColor(lv_color_make(255, 255, 255));
              style.setPadding(0, 0, 0, 0);
              style.setBorderRadius(0);
              style.setBorder(PRIMARY_BG, 0, LV_OPA_0);
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

  ErrorsScreen* ErrorsScreen::append(lv_obj_t* obj) {
    lv_obj_set_parent(obj, get_component());
    return this;
  }
}  // namespace ON2Solutions
