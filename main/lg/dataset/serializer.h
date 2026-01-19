//
// Created by evgeniy on 1/12/26.
//

#pragma once

#include <string_view>


namespace ON2Solutions::parser {

#define STATUS_COMMAND "<ST>"
#define RESET_MOTO_HOURS "<RS>"

  enum class SendableCommands {
    StatusCommand,
    ResetMotoCommand,
    OxygenShift,              // OS
    FlowShift,                // FS
    CompressorDelay,          // CD
    RunUpTime,                // RU
    PreStartTime,             // PS
    WorkOxygenConcentration,  // WC
    LowOxygenLimit,           // LL
    LowOxygenLimitTime,       // LT
    ErrorCountToAlarm,        // EC
    TankPressureHighLimit,    // TH
    TankPressureLowLimit,     // TL
    SpvValveOpenTime,         // VH
    SpvValveCloseTime,        // VL
    TankPressureSensorType,   // PT
    OxygenOverheatTempLimit,  // TO
    FlowError,                // FE
    SpvValveAutoCalibration   // CV
  };

  constexpr std::string_view command_to_string(SendableCommands cmd) {
    switch (cmd) {
      case SendableCommands::OxygenShift:
        return "OS";
      case SendableCommands::FlowShift:
        return "FS";
      case SendableCommands::CompressorDelay:
        return "CD";
      case SendableCommands::RunUpTime:
        return "RU";
      case SendableCommands::PreStartTime:
        return "PS";
      case SendableCommands::WorkOxygenConcentration:
        return "WC";
      case SendableCommands::LowOxygenLimit:
        return "LL";
      case SendableCommands::LowOxygenLimitTime:
        return "LT";
      case SendableCommands::ErrorCountToAlarm:
        return "EC";
      case SendableCommands::TankPressureHighLimit:
        return "TH";
      case SendableCommands::TankPressureLowLimit:
        return "TL";
      case SendableCommands::SpvValveOpenTime:
        return "VH";
      case SendableCommands::SpvValveCloseTime:
        return "VL";
      case SendableCommands::TankPressureSensorType:
        return "PT";
      case SendableCommands::OxygenOverheatTempLimit:
        return "TO";
      case SendableCommands::FlowError:
        return "FE";
      case SendableCommands::SpvValveAutoCalibration:
        return "CV";
      default:
        return "UNKNOWN";
    }
  }

  template<typename T>
  struct SerializableCommand {
    SendableCommands command;
    T data{};
    int8_t num_sensor{0};
    T min{};
    T max{};
    bool with_num_sensor{true};
  };

  template<typename T>
  std::string serialize(const SerializableCommand<T>& command);


  template<typename T>
  T normalize_value(T value, T min, T max) {
    if constexpr (std::is_floating_point_v<T>) {
      if (value < min) return min;
      if (value > max) return max;
      return value;
    } else if constexpr (std::is_integral_v<T>) {
      if (value < min) return min;
      if (value > max) return max;
      return value;
    } else {
      return value;
    }
  }

}  // namespace ON2Solutions::parser