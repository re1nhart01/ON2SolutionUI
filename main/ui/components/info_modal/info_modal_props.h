
#pragma once

namespace ON2Solutions {
  class InfoModal;

  struct InfoModalProps final
      : foundation::BaseProps<InfoModalProps, InfoModal> {
    std::string_view device{};
    std::string_view loader{};
    std::string_view fw{};
    std::string_view fw_checksum{};
    std::string_view module_name{};
    std::string_view module_fw{};
    std::string_view serial_number{};
    std::string_view ethernet_ip{};
    std::string_view wifi_ip{};
    std::string_view lcd_fw{};
    std::string_view lcd_loader{};
    std::string_view lcd_partition{};
    std::string_view hour_run{};
    uint32_t restart_seconds = 0;
    std::string_view action_label{};

    InfoModalProps& set_device(std::string_view v) {
      device = v;
      return *this;
    }
    InfoModalProps& set_loader(std::string_view v) {
      loader = v;
      return *this;
    }
    InfoModalProps& set_fw(std::string_view v) {
      fw = v;
      return *this;
    }
    InfoModalProps& set_fw_checksum(std::string_view v) {
      fw_checksum = v;
      return *this;
    }
    InfoModalProps& set_module_name(std::string_view v) {
      module_name = v;
      return *this;
    }
    InfoModalProps& set_module_fw(std::string_view v) {
      module_fw = v;
      return *this;
    }
    InfoModalProps& set_serial_number(std::string_view v) {
      serial_number = v;
      return *this;
    }
    InfoModalProps& set_ethernet_ip(std::string_view v) {
      ethernet_ip = v;
      return *this;
    }
    InfoModalProps& set_wifi_ip(std::string_view v) {
      wifi_ip = v;
      return *this;
    }
    InfoModalProps& set_lcd_fw(std::string_view v) {
      lcd_fw = v;
      return *this;
    }
    InfoModalProps& set_lcd_loader(std::string_view v) {
      lcd_loader = v;
      return *this;
    }
    InfoModalProps& set_lcd_partition(std::string_view v) {
      lcd_partition = v;
      return *this;
    }
    InfoModalProps& set_hour_run(std::string_view v) {
      hour_run = v;
      return *this;
    }
    InfoModalProps& set_restart_seconds(uint32_t v) {
      restart_seconds = v;
      return *this;
    }
    InfoModalProps& set_action_label(std::string_view v) {
      action_label = v;
      return *this;
    }
  };
}  // namespace ON2Solutions