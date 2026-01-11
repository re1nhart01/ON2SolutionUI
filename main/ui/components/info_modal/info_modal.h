

#pragma once
#include "core/shortcuts.h"
#include "core/styling/styling.h"
#include "ui/components/info_modal/info_modal_props.h"
#include "ui/localization.hh"

using namespace foundation;
namespace ON2Solutions {
  class InfoModal;

  using $$InfoModal = std::unique_ptr<InfoModal>;

  inline $$InfoModal $InfoModal(InfoModalProps args) {
    return std::make_unique<InfoModal>(std::move(args));
  }

  class InfoModal final : public Component<InfoModalProps> {
  public:
    using Component::props;
    $$Modal modal = nullptr;

    explicit InfoModal(InfoModalProps&& props) : Component(nullptr, nullptr, std::move(props)) {
      this->parent = nullptr;

      if (this->props.ref != nullptr) {
        this->props.ref->set(this);
      }
    };

    ~InfoModal() override
    {
      if (this->props.ref != nullptr) {
        this->props.ref->unlink();
      }
    };

    inline $$View makeRow(const char* key, const char* value, uint16_t height = 28) const
    {
      return $View(ViewProps::up()
        .h(height)
        .set_style([](Styling& style) {
          style.setBackgroundColor(lv_color_hex(0xffffff));
          style.setBackgroundOpa(LV_OPA_COVER);
          style.setBorder(lv_color_make(0, 0, 0), 0, LV_OPA_TRANSP);
          style.setBorderRadius(0);
          style.setPadding(0, 0, 0, 0);
        })
        .direction(LV_FLEX_FLOW_ROW)
        .items(LV_FLEX_ALIGN_CENTER)
        .track_cross(LV_FLEX_ALIGN_CENTER)
        .justify(LV_FLEX_ALIGN_SPACE_BETWEEN)
        .set_children(children(
          $Text(TextProps::up()
            .value(key)
          ),

          $Text(TextProps::up()
            .value(value)
          )
        ))
      );
    }

    lv_obj_t * render() override
    {
      Component::render();

      this->modal = $Modal(ModalProps::up().set_content($View(
        ViewProps::up()
          .w(LV_PCT(80))
          .h(LV_PCT(80))
          .set_children(children(
            $FlatList(FlatListProps::up()
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
                  .set_children(children(
                    this->makeRow(locales::en::info_device, this->props.device),
                    this->makeRow(locales::en::info_loader, this->props.loader),
                    this->makeRow(locales::en::info_fw, this->props.fw),
                    this->makeRow(locales::en::info_fw_checksum, this->props.fw_checksum),
                    this->makeRow(locales::en::info_module_name, this->props.module_name),
                    this->makeRow(locales::en::info_module_fw, this->props.module_fw),
                    this->makeRow(locales::en::info_serial_number, this->props.serial_number),
                    this->makeRow(locales::en::info_ethernet_ip, this->props.ethernet_ip),
                    this->makeRow(locales::en::info_wifi_ip, this->props.wifi_ip),
                    this->makeRow(locales::en::info_lcd_fw, this->props.lcd_fw),
                    this->makeRow(locales::en::info_lcd_loader, this->props.lcd_loader),
                    this->makeRow(locales::en::info_lcd_partition, this->props.lcd_partition),
                    $Button(
                      ButtonProps::up()
                        .label(locales::en::button_close)
                        .click([this](lv_event_t *) { this->close(); }))
                  )))
              )))
          ))
          .direction(LV_FLEX_FLOW_COLUMN)))
      );

      return this->modal->get_component();
    };

    void show() {
      if (this->modal == nullptr) {
        this->render();
      }
      this->modal->show();
    }

    void close() const {
      this->modal->close();
    }

    const Styling* styling() const override
    {
      this->style.setTextColor(lv_color_make(255, 255, 255));
      this->style.setPadding(0, 0, 16, 16); // Padding T R B L
      this->style.setBorder(lv_color_make(0, 0, 0), 0, 0);
      this->style.setFont(&lv_font_montserrat_12);
      this->style.setBorderRadius(0);

      return &this->style;
    };

    InfoModal* append(lv_obj_t* obj) override
    {
      lv_obj_set_parent(obj, get_component());
      return this;
    };
  };
}