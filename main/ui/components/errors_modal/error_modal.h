#pragma once

#include "core/shortcuts.h"
#include "core/styling/styling.h"
#include "ui/localization.h"

#include <lg/helpers/utils.h>
#include <ui/styles/theme.h>

using namespace foundation;
namespace ON2Solutions {
  class ErrorModal;

  struct ErrorModalProps final
      : foundation::BaseProps<ErrorModalProps, ErrorModal> {
    uint32_t error_hex = 0;

    ErrorModalProps& set_error_hex(const uint32_t v) {
      error_hex = v;
      return *this;
    }
  };

  using $$ErrorModal = std::unique_ptr<ErrorModal>;

  inline $$ErrorModal $ErrorModal(ErrorModalProps&& args) {
    return std::make_unique<ErrorModal>(std::forward<ErrorModalProps>(args));
  }

  class ErrorModal final : public Component<ErrorModalProps> {
   public:
    using Component::props;
    $$Modal modal = nullptr;

    explicit ErrorModal(ErrorModalProps&& props)
        : Component(nullptr, nullptr, std::move(props)) {
      this->parent = nullptr;

      if (this->props.ref != nullptr) {
        this->props.ref->set(this);
      }
    };

    ~ErrorModal() override {
      if (this->props.ref != nullptr) {
        this->props.ref->unlink();
      }
    };

    inline $$View makeRow(short index, const char* text, bool is_error,
                          uint16_t height = 28) const {
      return $View(
          ViewProps::up()
              .h(height)
              .set_style([](Styling& style) {
                style.setBackgroundColor(lv_color_hex(0xffffff));
                style.setBackgroundOpa(LV_OPA_COVER);
                style.setBorder(lv_color_make(0, 0, 0), 0, LV_OPA_TRANSP);
                style.setBorderRadius(0);
                style.setPadding(8, 8, 6, 6);
              })
              .direction(LV_FLEX_FLOW_ROW)
              .items(LV_FLEX_ALIGN_CENTER)
              .track_cross(LV_FLEX_ALIGN_CENTER)
              .justify(LV_FLEX_ALIGN_START)
              .set_children(children(
                  $View(
                      ViewProps::up()
                          .watch<parser::Dataset>(
                              parser::DatasetStore::getInstance(), "error",
                              [index](View* self,
                                      const parser::Dataset& value) {
                                bool is_enabled = is_up_bit_pos(
                                    value.operative_data.errors, index);

                                self->set_state([is_enabled](ViewProps& props) {
                                  props.set_style([is_enabled](Styling& style) {
                                    style.setBackgroundColor(
                                        is_enabled ? ERROR_COLOR
                                                   : NO_ERROR_COLOR);
                                  });
                                });
                              })
                          .w(25)
                          .h(15)
                          .set_style([is_error](Styling& style) {
                            style.setBackgroundColor(is_error ? ERROR_COLOR
                                                              : NO_ERROR_COLOR);
                            style.setBackgroundOpa(LV_OPA_COVER);
                            style.setBorder(lv_color_make(0, 0, 0), 0,
                                            LV_OPA_TRANSP);
                            style.setBorderRadius(2);
                            style.setPadding(0, 0, 0, 0);
                          })),

                  $View(ViewProps::up().w(2).h(1).set_style([](Styling& style) {
                    style.setBackgroundOpa(LV_OPA_TRANSP);
                    style.setPadding(0, 0, 0, 0);
                    style.setBorder(lv_color_make(0, 0, 0), 0, LV_OPA_TRANSP);
                  })),

                  $Text(TextProps::up().value(text)))));
    }

    inline Children renderRows() const {
      Children rows;
      rows.reserve(33);

      for (short i = 0; i < std::size(locales::en::info_bits_error); i++) {
        rows.push_back(this->makeRow(i, locales::en::info_bits_error[i],
                                     is_up_bit_pos(this->props.error_hex, i)));
      }

      rows.push_back(
          $Button(ButtonProps::up()
                      .label(locales::en::button_close)
                      .click([this](lv_event_t*) { this->close(); })));

      return rows;
    }

    lv_obj_t* render() override {
      Component::render();

      this->modal = $Modal(ModalProps::up().set_content($View(
          ViewProps::up()
              .w(LV_PCT(80))
              .h(LV_PCT(90))
              .set_children(children($FlatList(
                  FlatListProps::up()
                      .w(LV_PCT(100))
                      .h(LV_PCT(100))
                      .set_style([](Styling& style) {
                        style.setPadding(10, 10, 10, 10);
                        style.setBorderRadius(0);
                        style.setBorder(lv_color_make(255, 255, 255), 0, 0);
                      })
                      .set_children(children(
                          $View(ViewProps::up()
                                    .direction(LV_FLEX_FLOW_COLUMN)
                                    .w(LV_PCT(100))
                                    .h(LV_SIZE_CONTENT)
                                    .set_style(NoPaddingApply)
                                    .set_children(renderRows())))))))
              .direction(LV_FLEX_FLOW_COLUMN))));

      return this->modal->get_component();
    };

    void show() {
      if (this->modal == nullptr) {
        this->render();
      }
      this->modal->show();
    }

    void close() const { this->modal->close(); }

    const Styling* styling() const override {
      this->style.setTextColor(lv_color_make(255, 255, 255));
      this->style.setPadding(0, 0, 16, 16);  // Padding T R B L
      this->style.setBorder(lv_color_make(0, 0, 0), 0, 0);
      this->style.setFont(&lv_font_montserrat_12);
      this->style.setBorderRadius(0);

      return &this->style;
    };

    ErrorModal* append(lv_obj_t* obj) override {
      lv_obj_set_parent(obj, get_component());
      return this;
    };
  };
}  // namespace ON2Solutions