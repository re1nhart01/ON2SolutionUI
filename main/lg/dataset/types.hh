
#pragma once
#include <array>
#include <cstring>
#include <string>

namespace ON2Solutions::parser {
  // ST – Статус (A-StandBy, B-Booster Delay, C-Startup, D-Running, E-Producing, F-Alarm, G-Tank Full, H-Auto
  // Adjusting )
  //$AABBCCDDEEFFRQ</CH=2/ST=F/O2=17.4;0.0/FL=10.1;0.0/II=0F/IO=F0/ER=8FFFFF00/PS=51;0/HR=9:42>D6

  inline const char* CH_PACKETS[15] = {
    "$</CH=1/ST=F/O2=95.1;79.8;20.1;0.0/FL=10.2;9.8;1.1;5.5/TR=12;18/II=F/IO=FF0F/ER=8FF0FF00/PS=50;1/"
    "HR=111111111:41>",
    "$</CH=2/ST=T/O2=94.5;80.1;19.9;0.0/FL=10.0;9.9;1.2;5.6/TR=10;15/II=F/IO=FFFF/ER=8FFFFF00/PS=51;0/"
    "HR=123456789:42>",
    "$</CH=3/ST=H/O2=93.8;81.0;19.5;0.2/FL=9.8;10.3;1.4;5.3/TR=9;14/II=T/IO=0FFF/ER=8FFF0000/PS=52;0/"
    "HR=223456789:43>",
    "$</CH=4/ST=G/O2=96.0;78.9;21.0;0.0/FL=10.5;9.7;1.0;5.8/TR=14;20/II=F/IO=F0FF/ER=8F00FF00/PS=49;1/"
    "HR=323456789:44>",
    "$</CH=5/ST=F/O2=95.7;79.2;20.5;0.0/FL=10.1;9.9;1.2;5.6/TR=11;16/II=T/IO=FFAA/ER=8FFAFA00/PS=50;1/"
    "HR=423456789:45>",
    "#</OS=+2.0;+3.1;-2.0;+3.1/FS=+2.0;+3.1;-2.0;+3.1/CD=3/RU=470/PS=39/WC=93.5/LL=93.2/LT=29/EC=4/TH=58/"
    "TL=49/VH=6.8/VL=4.9/PT=A/TO=48/FE=1.1/CV=2>",
    "#</OS=+2.2;+3.3;-2.2;+3.3/FS=+2.2;+3.3;-2.2;+3.3/CD=3/RU=480/PS=40/WC=94.0/LL=93.8/LT=30/EC=4/TH=60/"
    "TL=50/VH=7.0/VL=5.0/PT=A/TO=50/FE=1.0/CV=2>",
    "#</OS=+2.4;+3.5;-2.4;+3.5/FS=+2.4;+3.5;-2.4;+3.5/CD=3/RU=490/PS=41/WC=94.5/LL=94.2/LT=31/EC=5/TH=61/"
    "TL=51/VH=7.2/VL=5.1/PT=A/TO=52/FE=0.9/CV=2>",
    "#</OS=+1.9;+3.0;-1.9;+3.0/FS=+1.9;+3.0;-1.9;+3.0/CD=4/RU=460/PS=38/WC=92.8/LL=92.5/LT=28/EC=3/TH=56/"
    "TL=48/VH=6.5/VL=4.7/PT=B/TO=46/FE=1.2/CV=3>",
    "#</OS=+2.1;+3.2;-2.1;+3.2/FS=+2.1;+3.2;-2.1;+3.2/CD=3/RU=475/PS=39/WC=93.7/LL=93.4/LT=29/EC=4/TH=59/"
    "TL=49/VH=6.9/VL=4.8/PT=A/TO=49/FE=1.0/CV=2>",
    "#</GN=ON2SYS_RevA/GV=1.6.2.3/GL=1.8/MN=EPORT-E19/MV=1.40.4/GF=C29FD7DF/EH=192.168.1.127/WF=10.10.10.9/"
    "GD=34EAE706A005>",
    "#</GN=ON2SYS_RevB/GV=1.6.2.4/GL=1.9/MN=EPORT-E20/MV=1.40.5/GF=D29FD7E0/EH=192.168.1.128/WF=10.10.10.10/"
    "GD=34EAE706A006>",
    "#</GN=ON2SYS_RevC/GV=1.6.3.0/GL=2.0/MN=EPORT-E21/MV=1.41.0/GF=E29FD7E1/EH=192.168.1.129/WF=10.10.10.11/"
    "GD=34EAE706A007>",
    "#</GN=ON2SYS_RevD/GV=1.6.4.0/GL=2.1/MN=EPORT-E22/MV=1.42.0/GF=F29FD7E2/EH=192.168.1.130/WF=10.10.10.12/"
    "GD=34EAE706A008>",
    "#</GN=ON2SYS_RevE/GV=1.6.5.0/GL=2.2/MN=EPORT-E23/MV=1.43.0/GF=029FD7E3/EH=192.168.1.131/WF=10.10.10.13/"
    "GD=34EAE706A009>",
};

  static const std::string unknown = "UN";

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

  template <typename T>
  using data_storage_array = std::array<T, 8>;

  // Map enum to char*
  inline const char* DatasetStatusToString(DatasetStatuses status)
  {
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
  inline DatasetStatuses GetStatusFromTextValue(const char* str)
  {
    if (strcmp(str, "A") == 0) return DatasetStatuses::StandBy;
    if (strcmp(str, "B") == 0) return DatasetStatuses::BoosterDelay;
    if (strcmp(str, "C") == 0) return DatasetStatuses::Startup;
    if (strcmp(str, "D") == 0) return DatasetStatuses::Running;
    if (strcmp(str, "E") == 0) return DatasetStatuses::Producing;
    if (strcmp(str, "F") == 0) return DatasetStatuses::Alarm;
    if (strcmp(str, "G") == 0) return DatasetStatuses::TankFull;
    if (strcmp(str, "H") == 0) return DatasetStatuses::AutoAdjusting;

    return DatasetStatuses::Unknown;
  }

  inline std::string GetTextValueFromStatus(const DatasetStatuses status)
  {
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
    std::array<float, 4> oxygen_levels;
    std::array<float, 4> oxygen_speed;
    std::array<uint8_t, 2> secondary_tank_pressure;  // TR
    std::array<uint8_t, 2> primary_tank_pressure;    // PS
    std::array<char, 21> moto_hours;
    uint8_t inputs;
    uint16_t outputs;
    uint32_t errors;
  };

  struct __attribute__((packed)) DatasetSettings {
    std::array<float, 4> oxygen_sensor_offset;
    std::array<float, 4> flow_sensor_offset;
    uint16_t compressor_delay_sec;
    uint16_t run_up_delay_sec;
    uint16_t prestart_time_sec;
    uint8_t low_limit_time_to_error_sec;
    float work_oxygen_concentration;
    float low_limit_oxygen_concentration;
    uint8_t error_to_alarm_count;
    uint8_t tank_high_pressure;
    uint8_t tank_low_pressure;
    float spv_on_time_sec;
    float spv_off_time_sec;
    std::array<char, 2> tank_pressure_sensor_type;
    uint8_t temperature_overheat_alarm;
    float flow_low_limit_to_error;
    uint8_t calibrate_valve_1_9_cycle;  // 0 - disable
  };

  struct Dataset {
    bool operator==(const Dataset&) const {};

    DatasetSystemInfo system_info{
        .device_name = {"UNKNOWN"},
        .firmware_version = {"UNKNOWN"},
        .loader_version = {"UNKNOWN"},
        .module_name = {"UNKNOWN"},
        .module_version = {"UNKNOWN"},
        .firmware_checksum = {"UNKNOWN"},
        .serial_number = {"UNKNOWN"},
        .lan_ip_address = {},
        .wifi_ip_address = {}};
    DatasetOperative operative_data{
        .channels_count = 1,
        .status = {'A'},
        .oxygen_levels = {0, 0, 0, 0},
        .oxygen_speed = {0, 0, 0, 0},
        .secondary_tank_pressure = {0, 0},
        .primary_tank_pressure = {0, 0},
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
        .low_limit_time_to_error_sec = 0,
        .work_oxygen_concentration = 0,
        .low_limit_oxygen_concentration = 0,
        .error_to_alarm_count = 0,
        .tank_high_pressure = 0,
        .tank_low_pressure = 0,
        .spv_on_time_sec = 0,
        .spv_off_time_sec = 0,
        .tank_pressure_sensor_type = {"A"},  //(A=analog ADC, B=contact NO, C=contact NC)
        .temperature_overheat_alarm = 0,
        .flow_low_limit_to_error = 0,
        .calibrate_valve_1_9_cycle = 0,
    };
  };
}  // namespace ON2Solutions::on2::parser