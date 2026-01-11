
#pragma once

namespace ON2Solutions {
  class InfoModal;

  struct InfoModalProps final
      : foundation::BaseProps<InfoModalProps, InfoModal> {
    const char* device = nullptr;

    const char* loader = nullptr;
    const char* fw = nullptr;
    const char* fw_checksum = nullptr;
    const char* module_name = nullptr;
    const char* module_fw = nullptr;
    const char* serial_number = nullptr;
    const char* ethernet_ip = nullptr;
    const char* wifi_ip = nullptr;
    const char* lcd_fw = nullptr;
    const char* lcd_loader = nullptr;
    const char* lcd_partition = nullptr;
    const char* hour_run = nullptr;
    uint32_t restart_seconds = 0;
    const char* action_label = nullptr;

    InfoModalProps& set_device(const char* v) {
      device = v;
      return *this;
    }

    InfoModalProps& set_loader(const char* v) {
      loader = v;
      return *this;
    }

    InfoModalProps& set_fw(const char* v) {
      fw = v;
      return *this;
    }

    InfoModalProps& set_fw_checksum(const char* v) {
      fw_checksum = v;
      return *this;
    }

    InfoModalProps& set_module_name(const char* v) {
      module_name = v;
      return *this;
    }

    InfoModalProps& set_module_fw(const char* v) {
      module_fw = v;
      return *this;
    }

    InfoModalProps& set_serial_number(const char* v) {
      serial_number = v;
      return *this;
    }

    InfoModalProps& set_ethernet_ip(const char* v) {
      ethernet_ip = v;
      return *this;
    }

    InfoModalProps& set_wifi_ip(const char* v) {
      wifi_ip = v;
      return *this;
    }

    InfoModalProps& set_lcd_fw(const char* v) {
      lcd_fw = v;
      return *this;
    }

    InfoModalProps& set_lcd_loader(const char* v) {
      lcd_loader = v;
      return *this;
    }

    InfoModalProps& set_lcd_partition(const char* v) {
      lcd_partition = v;
      return *this;
    }

    InfoModalProps& set_hour_run(const char* v) {
      hour_run = v;
      return *this;
    }

    InfoModalProps& set_restart_seconds(uint32_t v) {
      restart_seconds = v;
      return *this;
    }

    InfoModalProps& set_action_label(const char* v) {
      action_label = v;
      return *this;
    }
  };
}  // namespace ON2Solutions