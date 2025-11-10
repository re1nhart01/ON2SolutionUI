
#include <array>
#include <cstring>
#include <string>

//ST – Статус (A-StandBy, B-Booster Delay, C-Startup, D-Running, E-Producing, F-Alarm, G-Tank Full, H-Auto Adjusting )
//$AABBCCDDEEFFRQ</CH=2/ST=F/O2=17.4;0.0/FL=10.1;0.0/II=0F/IO=F0/ER=8FFFFF00/PS=51;0/HR=9:42>D6

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

template<typename T>
using data_storage_array = std::array<T, 8>;

// Map enum to char*
inline const char* DatasetStatusToString(DatasetStatuses status) {
  switch (status) {
    case DatasetStatuses::StandBy: return "A";
    case DatasetStatuses::BoosterDelay: return "B";
    case DatasetStatuses::Startup: return "C";
    case DatasetStatuses::Running: return "D";
    case DatasetStatuses::Producing: return "E";
    case DatasetStatuses::Alarm: return "F";
    case DatasetStatuses::TankFull: return "G";
    case DatasetStatuses::AutoAdjusting: return "H";
    default: return "?";
  }
}

// Map char* to enum
inline DatasetStatuses StringToDatasetStatus(const char* str) {
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


struct DatasetDTO {
  std::string device_number;
  std::string moto_hours;
  DatasetStatuses status;
  uint8_t channels;
  data_storage_array<float> oxygen_levels;
  data_storage_array<float> oxygen_speed;
  data_storage_array<float> tanks_pressure;
  uint8_t inputs;
  uint8_t outputs;
  uint32_t errors;
  uint8_t crc;
};