
#pragma once
#include <constants/localization.h>
#include <array>
#include <cstring>
#include <string>
#include <vector>

namespace ON2Solutions::parser {
  // ST – Статус (A-StandBy, B-Booster Delay, C-Startup, D-Running, E-Producing, F-Alarm, G-Tank Full, H-Auto
  // Adjusting )
  //$AABBCCDDEEFFRQ</CH=2/ST=F/O2=17.4;0.0/FL=10.1;0.0/II=0F/IO=F0/ER=8FFFFF00/PS=51;0/HR=9:42>D6

  static const std::string unknown = "UN";

  static constexpr std::array<const char*, 8> PRESSURE_TYPE_OPTIONS{
      "A", "B", "C", "D", "E", "F", "G", "H"};
  static constexpr std::array<const char*, 4>
      PRIMARY_SCREEN_OXYGEN_SENSOR_OPTIONS{"A", "B", "C", "D"};
  static constexpr std::array<const char*, 2>
      PRIMARY_SCREEN_PRESSURE_SENSOR_OPTIONS{
          "A",
          "B",
      };

  enum class DatasetStatuses {
    StandBy,
    BoosterDelay,
    Startup,
    Running,
    Producing,
    Alarm,
    TankFull,
    AutoAdjusting,
    Unknown,
  };

  enum BootStatus {
    BootloaderReady = 0,
    UploadWaiting = 1,
    FirmwareUploading = 2,
    UploadedSuccessfully = 3,
    UploadFailure = 4,
  };

  template <typename T>
  using data_storage_array = std::array<T, 8>;

  // Map enum to char*
  inline const char* DatasetStatusToString(DatasetStatuses status) {
    switch (status) {
      case DatasetStatuses::StandBy:
        return "A";
      case DatasetStatuses::BoosterDelay:
        return "B";
      case DatasetStatuses::Startup:
        return "C";
      case DatasetStatuses::Running:
        return "D";
      case DatasetStatuses::Producing:
        return "E";
      case DatasetStatuses::Alarm:
        return "F";
      case DatasetStatuses::TankFull:
        return "G";
      case DatasetStatuses::AutoAdjusting:
        return "H";
      default:
        return "?";
    }
  }

  // Map char* to enum
  inline DatasetStatuses GetStatusFromTextValue(const char* str) {
    if (strcmp(str, "A") == 0)
      return DatasetStatuses::StandBy;
    if (strcmp(str, "B") == 0)
      return DatasetStatuses::BoosterDelay;
    if (strcmp(str, "C") == 0)
      return DatasetStatuses::Startup;
    if (strcmp(str, "D") == 0)
      return DatasetStatuses::Running;
    if (strcmp(str, "E") == 0)
      return DatasetStatuses::Producing;
    if (strcmp(str, "F") == 0)
      return DatasetStatuses::Alarm;
    if (strcmp(str, "G") == 0)
      return DatasetStatuses::TankFull;
    if (strcmp(str, "H") == 0)
      return DatasetStatuses::AutoAdjusting;

    return DatasetStatuses::Unknown;
  }

  inline const char* GetBootloaderStatus(int8_t str) {
    if (str == 0)
      return locales::en::bootloader_status_1;
    if (str == 1)
      return locales::en::bootloader_status_2;
    if (str == 2)
      return locales::en::bootloader_status_3;
    if (str == 3)
      return locales::en::bootloader_status_4;

    return "";
  }

  inline std::string GetTextValueFromStatus(const DatasetStatuses status) {
    switch (status) {
      case DatasetStatuses::StandBy:
        return "STAND BY";
      case DatasetStatuses::BoosterDelay:
        return "BOOSTER DELAY";
      case DatasetStatuses::Startup:
        return "STARTUP";
      case DatasetStatuses::Running:
        return "RUNNING";
      case DatasetStatuses::Producing:
        return "PRODUCING";
      case DatasetStatuses::Alarm:
        return "ALARM";
      case DatasetStatuses::TankFull:
        return "TANK FULL";
      case DatasetStatuses::AutoAdjusting:
        return "AUTO ADJUSTING";
      default:
        return "UNKNOWN";
    }
  }

  struct __attribute__((packed)) DatasetSystemInfo {
    std::array<char, 33> device_name;        // GN
    std::array<char, 33> firmware_version;   // GV
    std::array<char, 17> loader_version;     // GL
    std::array<char, 17> module_name;        // MN
    std::array<char, 17> module_version;     // MV
    std::array<char, 17> firmware_checksum;  // GF
    std::array<char, 13> serial_number;      // GD
    std::array<char, 17> lan_ip_address;     // EH
    std::array<char, 17> wifi_ip_address;    // WH
  };

  struct __attribute__((packed)) DatasetOperative {
    uint8_t channels_count;
    std::array<char, 3> status;
    float primary_screen_oxygen_sensor;               // PO
    float primary_screen_pressure_sensor;             // PP
    std::array<float, 4> oxygen_levels;
    std::array<float, 4> oxygen_speed;
    std::array<uint8_t, 2> secondary_tank_pressure;  // TR
    std::array<uint8_t, 2> primary_tank_pressure;    // PS
    std::array<float, 2> tank_temperatures;
    std::array<char, 21> moto_hours;
    uint8_t inputs;
    uint16_t outputs;
    uint32_t errors;
  };

  struct __attribute__((packed)) DatasetSettings {
    std::array<float, 4> oxygen_sensor_offset;            // OS
    std::array<float, 4> flow_sensor_offset;              // FS
    uint16_t compressor_delay_sec;                        // CD
    uint16_t run_up_delay_sec;                            // RU
    uint16_t prestart_time_sec;                           // PS
    float work_oxygen_concentration;                      // WC
    float low_limit_oxygen_concentration;                 // LL
    uint8_t low_limit_time_to_error_sec;                  // LT
    uint8_t error_to_alarm_count;                         // EC
    uint8_t tank_high_pressure;                           // TH
    uint8_t tank_low_pressure;                            // TL
    float spv_on_time_sec;                                // VH
    float spv_off_time_sec;                               // VL
    std::array<char, 2> tank_pressure_sensor_type;        // PT
    std::array<char, 2> primary_screen_oxygen_sensor_st;  // PO
    std::array<char, 2> primary_screen_pressure_sensor_st;// PP
    uint8_t temperature_overheat_alarm;             // TO
    float flow_low_limit_to_error;                  // FE
    uint8_t calibrate_valve_1_9_cycle;              // 0 - disable // CV
  };

  struct __attribute__((packed)) DatasetOptional {
    uint8_t reset_countdown_sec;
    int8_t bootloader_mode;
  };

  struct Dataset {
    bool operator==(const Dataset&) const { return true; };

    DatasetSystemInfo system_info{.device_name = {""},
                                  .firmware_version = {""},
                                  .loader_version = {""},
                                  .module_name = {""},
                                  .module_version = {""},
                                  .firmware_checksum = {""},
                                  .serial_number = {""},
                                  .lan_ip_address = {},
                                  .wifi_ip_address = {}};
    DatasetOperative operative_data{
        .channels_count = 1,
        .status = {'A'},
        .primary_screen_oxygen_sensor = 0,
        .primary_screen_pressure_sensor = 0,
        .oxygen_levels = {0, 0, 0, 0},
        .oxygen_speed = {0, 0, 0, 0},
        .secondary_tank_pressure = {0, 0},
        .primary_tank_pressure = {0, 0},
        .tank_temperatures = {0,0},
        .moto_hours = {"0:0"},
        .inputs = 0,
        .outputs = 0,
        .errors = 0xffffffff,
    };
    DatasetSettings settings{
        .oxygen_sensor_offset = {0, 0, 0, 0},
        .flow_sensor_offset = {0, 0, 0, 0},
        .compressor_delay_sec = 0,
        .run_up_delay_sec = 0,
        .prestart_time_sec = 0,
        .work_oxygen_concentration = 0,
        .low_limit_oxygen_concentration = 0,
        .low_limit_time_to_error_sec = 0,
        .error_to_alarm_count = 0,
        .tank_high_pressure = 0,
        .tank_low_pressure = 0,
        .spv_on_time_sec = 0,
        .spv_off_time_sec = 0,
        .tank_pressure_sensor_type =
            {"A"},  //(A=analog ADC, B=contact NO, C=contact NC)
        .primary_screen_oxygen_sensor_st = {"A"},
        .primary_screen_pressure_sensor_st = {"A"},
        .temperature_overheat_alarm = 0,
        .flow_low_limit_to_error = 0,
        .calibrate_valve_1_9_cycle = 0,
    };
    DatasetOptional optional{
        .reset_countdown_sec = 0,
        .bootloader_mode = -1,
    };
  };
}  // namespace ON2Solutions::parser