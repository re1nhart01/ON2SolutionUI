
#include "info_screen.h"

#include <constants/localization.h>
#include <control_config.hh>
#include <lg/dataset/store/dataset.store.h>
#include <lg/dataset/types.h>
#include "lg/dataset/serializer.h"
#include "protocols/uart/uart_proto.h"


namespace ON2Solutions {

  void InfoScreen::on_focus() {
    execute_typed_command(this->props.uart, parser::SendableCommands::RequestData, static_cast<int>(InfoRequest));
  }

  void InfoScreen::component_did_mount() {
    ESP_LOGI("preloader_screen", "Preloading screen");
  }

  $$CommonHeader InfoScreen::render_header() const {
    return $CommonHeader(CommonHeaderProps::up());
  }

  using SystemInfoGetter = Delegate<std::string(const Dataset&)>;

  struct SystemField {
    const char* label;
    SystemInfoGetter getter;
  };

  static const SystemField system_fields[] = {
      {"Device Name",
       [](const Dataset& s) {
         return std::string(s.system_info.device_name.data());
       }},
      {"Firmware Ver",
       [](const Dataset& s) {
         return std::string(s.system_info.firmware_version.data());
       }},
      {"Loader Ver",
       [](const Dataset& s) {
         return std::string(s.system_info.loader_version.data());
       }},
      {"Module Name",
       [](const Dataset& s) {
         return std::string(s.system_info.module_name.data());
       }},
      {"Module Ver",
       [](const Dataset& s) {
         return std::string(s.system_info.module_version.data());
       }},
      {"Checksum",
       [](const Dataset& s) {
         return std::string(s.system_info.firmware_checksum.data());
       }},
      {"Serial Number",
       [](const Dataset& s) {
         return std::string(s.system_info.serial_number.data());
       }},
      {"LAN IP",
       [](const Dataset& s) {
         return std::string(s.system_info.lan_ip_address.data());
       }},
      {"WIFI IP",
       [](const Dataset& s) {
         return std::string(s.system_info.wifi_ip_address.data());
       }},
      {"LCD FIRMWARE", [](const Dataset& s) { return LCD_FIRMWARE_VERSION; }},
      {"LCD BOOTLOADER", [](const Dataset& s) { return LCD_LOADER_VERSION; }},
      {"LCD RELEASE DATE", [](const Dataset& s) { return LCD_FIRMWARE_RELEASE_DATE; }}
  };

  $$View InfoScreen::create_status_row(int index) {
    const auto& field_info = system_fields[index];

    const auto& current_data = DatasetStore::getInstance()->get();

    std::string initial_value = field_info.getter(*current_data);

    return $View(
        ViewProps::up()
            .w(696)
            .h(38)
            .flow(FlexPreset::RowBetween)
            .set_style([index](Styling& style) {
              style.setBackgroundColor(index % 2 == 0 ? TERTIARY_BG
                                                      : PRIMARY_BG);
              style.setPadding(0, 0, 16, 16);
              style.setBorder();
              style.setGap(0, 20);
              style.setBorderRadius(8);
            })
            .set_children(children(
                $Text(TextProps::up().value(field_info.label)),
                $Text(
                    TextProps::up()
                        .value(initial_value)
                        .watch<parser::Dataset>(
                            parser::DatasetStore::getInstance(), "system_info",
                            [field_info](VNode* self,
                                         const parser::Dataset& value) {
                              if (!self || !self->get_component())
                                return;

                              const std::string val = field_info.getter(value);
                              lv_label_set_text(self->get_component(),
                                                val.c_str());
                            })))));
  }

  $$View InfoScreen::render_body() {
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
                          .value(locales::en::header_information)
                          .set_style([](Styling& style) {
                            style.setFont(&lv_font_montserrat_18);
                          })),
                $PaginatedList(
                    PaginatedListProps()
                        .w(712)
                        .h(355)
                        .set_renderer(std::size(system_fields),
                                      [this](int index) {
                                        return this->create_status_row(index);
                                      })
                        .set_items_per_page(6)
                        .set_style([](Styling& style) {
                          style.setBorder(PRIMARY_BG, 0, LV_OPA_0);
                          style.setShadow(lv_color_hex(0x1018281A), 12, 0);
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

  InfoScreen* InfoScreen::append(lv_obj_t* obj) {
    lv_obj_set_parent(obj, get_component());
    return this;
  }
}  // namespace ON2Solutions
