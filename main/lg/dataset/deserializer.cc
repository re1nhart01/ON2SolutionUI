
#include "deserializer.hh";
#include "esp_log.h";

extern "C" {
  #include "esp_log.h"
  #include "freertos/FreeRTOS.h"
  #include "freertos/task.h"
  #include "driver/gpio.h"
  #include "sdkconfig.h"
}

// operative data example -
// $</CH=2/ST=F/O2=94.5;80.1;19.9;0.0/FL=10.0;9.9;1.2;5.6/TR=10;15/II=F/IO=FFFF/ER=8FFFFF00/PS=51;0/HR=123456789:42>
// settings  data example -
// #</OS=+2.2;+3.3;-2.2;+3.3/FS=+2.2;+3.3;-2.2;+3.3/CD=3/RU=480/PS=40/WC=94.0/LL=93.8/LT=30/EC=4/TH=60/TL=50/VH=7.0/VL=5.0/PT=A/TO=50/FE=1.0/CV=2>
// sysinfo   data example -
// #</GN=ON2SYS_RevB/GV=1.6.2.4/GL=1.9/MN=EPORT-E20/MV=1.40.5/GF=D29FD7E0/EH=192.168.1.128/WF=10.10.10.10/GD=34EAE706A006>
//`$</CH=2/ST=F/O2=94.5;80.1;19.9;0.0/FL=10.0;9.9;1.2;5.6/TR=10;15/II=F/IO=FFFF/ER=8FFFFF00/PS=51;0/HR=123456789:42>`.replaceAll("$",
//"").replaceAll("<", "").replaceAll("/", " ").replaceAll(">", "")

namespace ON2Solutions::parser {
  using namespace ON2Solutions::parser::helpers;

  PacketType validate_type(const char* packet_string, const size_t len)
  {
    if (packet_string == nullptr || len < 40) return PacketType::UNKNOWN;

    if (packet_string[0] == '$') return PacketType::OPERATIVE;
    if (packet_string[0] == '#' && packet_string[3] == 'O') return PacketType::SETTINGS;
    if (packet_string[0] == '#' && packet_string[3] == 'G') return PacketType::SYSTEM_INFO;

    return PacketType::UNKNOWN;
  }

  void parse_selected_value(
      Dataset* dataset, const char* key, int key_len, const char* val, int val_len, PacketType type)
  {
    // Operative Data
    if (type == PacketType::OPERATIVE) {
      if (key_len == 2 && key[0] == 'C' && key[1] == 'H') {
        uint8_t out;
        if (parse_value(val, val_len, out)) {
          dataset->operative_data.channels_count = out;
        }
      } else if (key_len == 2 && key[0] == 'S' && key[1] == 'T') {
        copy_string(&dataset->operative_data.status, sizeof(dataset->operative_data.status), val, val_len);
      } else if (key_len == 2 && key[0] == 'O' && key[1] == '2') {
        std::array<float, 4> o2{0, 0, 0, 0};
        parse_list(val, val_len, o2);
        dataset->operative_data.oxygen_levels = o2;
      } else if (key_len == 2 && key[0] == 'F' && key[1] == 'L') {
        std::array<float, 4> f2{0, 0, 0, 0};
        parse_list(val, val_len, f2);
        dataset->operative_data.oxygen_speed = f2;
      } else if (key_len == 2 && key[0] == 'T' && key[1] == 'R') {
        std::array<uint8_t, 2> sp{0, 0};
        parse_list(val, val_len, sp);
        dataset->operative_data.secondary_tank_pressure = sp;
      } else if (key_len == 2 && key[0] == 'I' && key[1] == 'I') {
        uint8_t out;
        if (parse_hex(val, val_len, out)) {
          dataset->operative_data.inputs = out;
        }
      } else if (key_len == 2 && key[0] == 'I' && key[1] == 'O') {
        uint16_t out;
        if (parse_hex(val, val_len, out)) {
          dataset->operative_data.outputs = out;
        }
      } else if (key_len == 2 && key[0] == 'E' && key[1] == 'R') {
        uint32_t out;
        if (parse_hex(val, val_len, out)) {
          dataset->operative_data.errors = out;
        }
      } else if (key_len == 2 && key[0] == 'P' && key[1] == 'S') {
        std::array<uint8_t, 2> pp{0, 0};
        parse_list(val, val_len, pp);
        dataset->operative_data.primary_tank_pressure = pp;
      } else if (key_len == 2 && key[0] == 'H' && key[1] == 'R') {
        copy_string(
            &dataset->operative_data.moto_hours,
            sizeof(dataset->operative_data.moto_hours),
            val,
            val_len);
      }
    }

    // SETTINGS DATA

    if (type == PacketType::SETTINGS) {
      if (key_len == 2 && key[0] == 'O' && key[1] == 'S') {
        std::array<float, 4> oso{0, 0, 0, 0};
        parse_list(val, val_len, oso);
        dataset->settings.oxygen_sensor_offset = oso;
      } else if (key_len == 2 && key[0] == 'F' && key[1] == 'S') {
        std::array<float, 4> fso{0, 0, 0, 0};
        parse_list(val, val_len, fso);
        dataset->settings.flow_sensor_offset = fso;
      } else if (key_len == 2 && key[0] == 'C' && key[1] == 'D') {
        uint16_t out;
        if (parse_value(val, val_len, out)) {
          dataset->settings.compressor_delay_sec = out;
        }
      } else if (key_len == 2 && key[0] == 'R' && key[1] == 'U') {
        uint16_t out;
        if (parse_value(val, val_len, out)) {
          dataset->settings.run_up_delay_sec = out;
        }
      } else if (key_len == 2 && key[0] == 'P' && key[1] == 'S') {
        uint16_t out;
        if (parse_value(val, val_len, out)) {
          dataset->settings.prestart_time_sec = out;
        }
      } else if (key_len == 2 && key[0] == 'W' && key[1] == 'C') {
        float out;
        if (parse_value(val, val_len, out)) {
          dataset->settings.work_oxygen_concentration = out;
        }
      } else if (key_len == 2 && key[0] == 'L' && key[1] == 'L') {
        float out;
        if (parse_value(val, val_len, out)) {
          dataset->settings.low_limit_oxygen_concentration = out;
        }
      } else if (key_len == 2 && key[0] == 'L' && key[1] == 'T') {
        uint8_t out;
        if (parse_value(val, val_len, out)) {
          dataset->settings.low_limit_time_to_error_sec = out;
        }
      } else if (key_len == 2 && key[0] == 'E' && key[1] == 'C') {
        uint8_t out;
        if (parse_value(val, val_len, out)) {
          dataset->settings.error_to_alarm_count = out;
        }
      } else if (key_len == 2 && key[0] == 'T' && key[1] == 'H') {
        uint8_t out;
        if (parse_value(val, val_len, out)) {
          dataset->settings.tank_high_pressure = out;
        }
      } else if (key_len == 2 && key[0] == 'T' && key[1] == 'L') {
        uint8_t out;
        if (parse_value(val, val_len, out)) {
          dataset->settings.tank_low_pressure = out;
        }
      } else if (key_len == 2 && key[0] == 'V' && key[1] == 'H') {
        float out;
        if (parse_value(val, val_len, out)) {
          dataset->settings.spv_on_time_sec = out;
        }
      } else if (key_len == 2 && key[0] == 'V' && key[1] == 'L') {
        float out;
        if (parse_value(val, val_len, out)) {
          dataset->settings.spv_off_time_sec = out;
        }
      } else if (key_len == 2 && key[0] == 'P' && key[1] == 'T') {
        copy_string(
            &dataset->settings.tank_pressure_sensor_type,
            sizeof(dataset->settings.tank_pressure_sensor_type),
            val,
            val_len);
      } else if (key_len == 2 && key[0] == 'T' && key[1] == 'O') {
        uint8_t out;
        if (parse_value(val, val_len, out)) {
          dataset->settings.temperature_overheat_alarm = out;
        }
      } else if (key_len == 2 && key[0] == 'F' && key[1] == 'E') {
        float out;
        if (parse_value(val, val_len, out)) {
          dataset->settings.flow_low_limit_to_error = out;
        }
      } else if (key_len == 2 && key[0] == 'C' && key[1] == 'V') {
        uint8_t out;
        if (parse_value(val, val_len, out)) {
          dataset->settings.calibrate_valve_1_9_cycle = out;
        }
      }
    }

    // System Info
    if (type == PacketType::SYSTEM_INFO) {
      if (key_len == 2 && key[0] == 'G' && key[1] == 'N') {
        copy_string(
            &dataset->system_info.device_name,
            sizeof(dataset->system_info.device_name),
            val,
            val_len);
      } else if (key_len == 2 && key[0] == 'G' && key[1] == 'V') {
        copy_string(
            &dataset->system_info.firmware_version,
            sizeof(dataset->system_info.firmware_version),
            val,
            val_len);
      } else if (key_len == 2 && key[0] == 'G' && key[1] == 'L') {
        copy_string(
            &dataset->system_info.loader_version,
            sizeof(dataset->system_info.loader_version),
            val,
            val_len);
      } else if (key_len == 2 && key[0] == 'M' && key[1] == 'N') {
        copy_string(
            &dataset->system_info.module_name,
            sizeof(dataset->system_info.module_name),
            val,
            val_len);
      } else if (key_len == 2 && key[0] == 'M' && key[1] == 'V') {
        copy_string(
            &dataset->system_info.module_version,
            sizeof(dataset->system_info.module_version),
            val,
            val_len);
      } else if (key_len == 2 && key[0] == 'G' && key[1] == 'F') {
        copy_string(
            &dataset->system_info.firmware_checksum,
            sizeof(dataset->system_info.firmware_checksum),
            val,
            val_len);
      } else if (key_len == 2 && key[0] == 'E' && key[1] == 'H') {
        copy_string(
            &dataset->system_info.lan_ip_address,
            sizeof(dataset->system_info.lan_ip_address),
            val,
            val_len);
      } else if (key_len == 2 && key[0] == 'W' && key[1] == 'F') {
        copy_string(
            &dataset->system_info.wifi_ip_address,
            sizeof(dataset->system_info.wifi_ip_address),
            val,
            val_len);
      } else if (key_len == 2 && key[0] == 'G' && key[1] == 'D') {
        copy_string(
            &dataset->system_info.serial_number,
            sizeof(dataset->system_info.serial_number),
            val,
            val_len);
      }
    }
  }

  //$</CH=2/ST=F/O2=94.5;80.1;19.9;0.0/FL=10.0;9.9;1.2;5.6/TR=10;15/II=F/IO=FFFF/ER=8FFFFF00/PS=51;0/HR=123456789:42>
  void parse_data(char packet_start, Dataset* dataset, const char* packet_string, PacketType type)
  {
    const char* ptr = packet_string;
    ptr = strchr(ptr, '<');
    if (!ptr) return;

    bool is_start_key = false;
    bool is_start_value = false;
    const char* key_start = nullptr;
    const char* value_start = nullptr;
    const char* key_end = nullptr;

    while (*ptr) {
      if (*ptr == packet_start || *ptr == '<') {
        ptr++;
        continue;
      }

      if (*ptr == '/' && !is_start_value) {
        is_start_key = true;
        is_start_value = false;
        key_start = ptr + 1;
      } else if (*ptr == '=' && is_start_key) {
        is_start_value = true;
        is_start_key = false;

        key_end = ptr;
        value_start = ptr + 1;
      } else if ((*ptr == '/' || *ptr == '>') && is_start_value) {
        is_start_value = false;

        if (key_end && key_start && value_start) {
          auto key_len = key_end - key_start;
          auto val_len = ptr - value_start;

          parse_selected_value(dataset, key_start, key_len, value_start, val_len, type);
        }

        if (*ptr == '/') {
          is_start_key = true;
          key_start = ptr + 1;
        }

        if (*ptr == '>') {
          break;
        }
      }
      ptr++;
    }
  }

  void parse(Dataset* dataset, const char* packet_string, size_t len)
  {
    PacketType type = validate_type(packet_string, len);

    switch (type) {
      case PacketType::OPERATIVE:
        parse_data('$', dataset, packet_string, type);
        break;
      case PacketType::SETTINGS:
        parse_data('#', dataset, packet_string, type);
        break;
      case PacketType::SYSTEM_INFO:
        parse_data('#', dataset, packet_string, type);
        break;
      default:
        break;
    }
  }

}  // namespace ON2Solutions::parser
