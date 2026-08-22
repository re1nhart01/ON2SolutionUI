//
// Created by evgeniy on 1/19/26.
//

#pragma once
#include <cstdint>
#include "serializer.h"
#include "types.h"

namespace ON2Solutions::parser::paramspec {

  struct ParamSpec final {
    const char* label = nullptr;

    float min = 0.0f;
    float max = 0.0f;
    float step = 1.0f;

    short precision = 0;
    int8_t num_sensor = 0;

    float default_value = 0.0f;
    SendableCommands command;

    bool with_num_sensor{false};
  };

#pragma region CALIBRATION_OXYGEN_SHIFT

  inline constexpr ParamSpec OxygenShiftASpec{
      .label = "Oxy-A",
      .min = -5.0f,
      .max = 5.0f,
      .step = 0.1f,
      .precision = 1,
      .num_sensor = 1,
      .default_value = 0.0f,
      .command = SendableCommands::OxygenShift,
      .with_num_sensor = true,
  };  // OS-1

  inline constexpr ParamSpec OxygenShiftBSpec{
      .label = "Oxy-B",
      .min = -5.0f,
      .max = 5.0f,
      .step = 0.1f,
      .precision = 1,
      .num_sensor = 2,
      .default_value = 0.0f,
      .command = SendableCommands::OxygenShift,
      .with_num_sensor = true,
  };  // OS-2

  inline constexpr ParamSpec OxygenShiftCSpec{
      .label = "Oxy-C",
      .min = -5.0f,
      .max = 5.0f,
      .step = 0.1f,
      .precision = 1,
      .num_sensor = 3,
      .default_value = 0.0f,
      .command = SendableCommands::OxygenShift,
      .with_num_sensor = true,
  };  // OS-3

  inline constexpr ParamSpec OxygenShiftDSpec{
      .label = "Oxy-D",
      .min = -5.0f,
      .max = 5.0f,
      .step = 0.1f,
      .precision = 1,
      .num_sensor = 4,
      .default_value = 0.0f,
      .command = SendableCommands::OxygenShift,
      .with_num_sensor = true,
  };  // OS-4

#pragma endregion CALIBRATION_OXYGEN_SHIFT

#pragma region CALIBRATION_FLOW_SHIFT

  inline constexpr ParamSpec FlowShiftASpec{
      .label = "Flow-A",
      .min = -5.0f,
      .max = 5.0f,
      .step = 0.1f,
      .precision = 1,
      .num_sensor = 1,
      .default_value = 0.0f,
      .command = SendableCommands::FlowShift,
      .with_num_sensor = true,
  };  // FS-1

  inline constexpr ParamSpec FlowShiftBSpec{
      .label = "Flow-B",
      .min = -5.0f,
      .max = 5.0f,
      .step = 0.1f,
      .precision = 1,
      .num_sensor = 2,
      .default_value = 0.0f,
      .command = SendableCommands::FlowShift,
      .with_num_sensor = true,
  };  // FS-2

  inline constexpr ParamSpec FlowShiftCSpec{
      .label = "Flow-C",
      .min = -5.0f,
      .max = 5.0f,
      .step = 0.1f,
      .precision = 1,
      .num_sensor = 3,
      .default_value = 0.0f,
      .command = SendableCommands::FlowShift,
      .with_num_sensor = true,
  };  // FS-3

  inline constexpr ParamSpec FlowShiftDSpec{
      .label = "Flow-D",
      .min = -5.0f,
      .max = 5.0f,
      .step = 0.1f,
      .precision = 1,
      .num_sensor = 4,
      .default_value = 0.0f,
      .command = SendableCommands::FlowShift,
      .with_num_sensor = true,
  };  // FS-4

#pragma endregion CALIBRATION_FLOW_SHIFT

#pragma region TIMERS

  inline constexpr ParamSpec CompressorDelaySpec{
      .label = "Compressor Delay",
      .min = 1.0f,
      .max = 9.0f,
      .step = 1.0f,
      .precision = 0,
      .default_value = 9.0f,
      .command = SendableCommands::CompressorDelay};  // CD

  inline constexpr ParamSpec RunUpTimeSpec{
      .label = "RunUp Time",
      .min = 120.0f,
      .max = 999.0f,
      .step = 1.0f,
      .precision = 0,
      .default_value = 120.0f,
      .command = SendableCommands::RunUpTime};  // RU

  // NOTE: max depends on RU!
  inline constexpr ParamSpec PreStartTimeSpec{
      .label = "PreStart Time",
      .min = 30.0f,
      .max = 999.0f,
      .step = 1.0f,
      .precision = 0,
      .default_value = 30.0f,
      .command = SendableCommands::PreStartTime};  // PS

#pragma endregion TIMERS

#pragma region LIMITS_OXYGEN

  inline constexpr ParamSpec WorkConcentrationSpec{
      .label = "Work Concentration",
      .min = 90.0f,
      .max = 99.9f,
      .step = 0.1f,
      .precision = 1,
      .default_value = 95.0f,
      .command = SendableCommands::WorkOxygenConcentration};  // WC

  // NOTE: max depends on WC!
  inline constexpr ParamSpec LowLimitConcentrationSpec{
      .label = "Low Concentration",
      .min = 80.0f,
      .max = 99.9f,  // base max, clamp by WC
      .step = 0.1f,
      .precision = 1,
      .default_value = 80.0f,
      .command = SendableCommands::LowOxygenLimit};  // LL

  inline constexpr ParamSpec LowLimitTimeSpec{
      .label = "Low Limit Time",
      .min = 10.0f,
      .max = 99.0f,
      .step = 1.0f,
      .precision = 0,
      .default_value = 10.0f,
      .command = SendableCommands::LowOxygenLimitTime};  // LT

#pragma endregion LIMITS_OXYGEN

#pragma region LIMITS_PRESSURE

  // Tank pressure
  // NOTE: must be >= TL, autocorrect TL+2
  inline constexpr ParamSpec TankHighLimitSpec{
      .label = "Tank High Limit PSI",
      .min = 0.0f,
      .max = 180.0f,
      .step = 1.0f,
      .precision = 0,
      .default_value = 50.0f,
      .command = SendableCommands::TankPressureHighLimit};  // TH

  // NOTE: must be <= TH, autocorrect TH-2
  inline constexpr ParamSpec TankLowLimitSpec{
      .label = "Tank Low Limit PSI",
      .min = 0.0f,
      .max = 180.0f,
      .step = 1.0f,
      .precision = 0,
      .default_value = 40.0f,
      .command = SendableCommands::TankPressureLowLimit};  // TL

#pragma endregion LIMITS_PRESSURE

#pragma region LIMITS_SYSTEM

  inline constexpr ParamSpec ErrorCountSpec{
      .label = "Error Count",
      .min = 1.0f,
      .max = 9.0f,
      .step = 1.0f,
      .precision = 0,
      .default_value = 1.0f,
      .command = SendableCommands::ErrorCountToAlarm};  // EC

  inline constexpr ParamSpec TempOverheatSpec{
      .label = "Temp Overheat",
      .min = 30.0f,
      .max = 200.0f,
      .step = 1.0f,
      .precision = 0,
      .default_value = 55.0f,
      .command = SendableCommands::OxygenOverheatTempLimit};  // TO

  inline constexpr ParamSpec FlowErrorSpec{
      .label = "Flow Error",
      .min = 0.0f,
      .max = 9.9f,
      .step = 0.1f,
      .precision = 1,
      .default_value = 1.2f,
      .command = SendableCommands::FlowError};  // FE

#pragma endregion LIMITS_SYSTEM

#pragma region SERVICE

  // SPV valves
  // NOTE: must be > VL, autocorrect VL+0.2
  inline constexpr ParamSpec ValveHighTimeSpec{
      .label = "Valve High Time",
      .min = 1.0f,
      .max = 9.9f,
      .step = 0.1f,
      .precision = 1,
      .default_value = 6.9f,
      .command = SendableCommands::SpvValveOpenTime};  // VH

  // NOTE: must be < VH, autocorrect VH-0.2
  inline constexpr ParamSpec ValveLowTimeSpec{
      .label = "Valve Low Time",
      .min = 0.5f,
      .max = 9.0f,
      .step = 0.1f,
      .precision = 1,
      .default_value = 4.9f,
      .command = SendableCommands::SpvValveCloseTime};  // VL

  inline constexpr ParamSpec CalibrateValveSpec{
      .label = "Calibrate Valve",
      .min = 0.0f,
      .max = 9.0f,
      .step = 1.0f,
      .precision = 0,
      .default_value = 0.0f,
      .command = SendableCommands::SpvValveAutoCalibration};  // CV

  // PT is enum, not a Stepper
  inline constexpr ParamSpec PressureTypeSensorSpec{
    .label = "Pressure Type Sensor",
    .min = 0.0f,
    .max = 9.0f,
    .step = 1.0f,
    .precision = 0,
    .default_value = 0.0f,
    .command = SendableCommands::TankPressureSensorType};

  inline constexpr ParamSpec PrimaryScreenOxygenSensorSpec{
    .label = "Pressure Type Sensor",
    .min = 0.0f,
    .max = 9.0f,
    .step = 1.0f,
    .precision = 0,
    .default_value = 0.0f,
    .command = SendableCommands::PrimaryScreenOxygenSensor};

  inline constexpr ParamSpec PrimaryScreenPressureSensorSpec{
    .label = "Pressure Type Sensor",
    .min = 0.0f,
    .max = 9.0f,
    .step = 1.0f,
    .precision = 0,
    .default_value = 0.0f,
    .command = SendableCommands::PrimaryScreenPressureSensor};

#pragma endregion SERVICE

#pragma region HELPERS

  inline std::pair<float, float> calculate_dynamic_range(const ParamSpec& spec,
                                                         float dependable, bool enabled_dyn = false) {
    float mn = spec.min;
    float mx = spec.max;

    if (!enabled_dyn) {
      return { mn, mx };
    }

    switch (spec.command) {

      // PS <= RU - 60
      case SendableCommands::PreStartTime: {
        mx = dependable - 60.0f;
        if (mx < mn)
          mx = mn;
        break;
      }

        // LL <= WC
      case SendableCommands::LowOxygenLimit: {
        mx = dependable;  // dependable = WC
        if (mx < mn)
          mx = mn;
        break;
      }

        // TH >= TL
      case SendableCommands::TankPressureHighLimit: {
        mn = dependable;  // dependable = TL
        if (mn > mx)
          mn = mx;
        break;
      }

        // TL <= TH
      case SendableCommands::TankPressureLowLimit: {
        mx = dependable;  // dependable = TH
        if (mx < mn)
          mx = mn;
        break;
      }

        // VH > VL
      case SendableCommands::SpvValveOpenTime: {
        mn = dependable + 0.2f;  // dependable = VL
        if (mn > mx)
          mn = mx;
        break;
      }

        // VL < VH
      case SendableCommands::SpvValveCloseTime: {
        mx = dependable - 0.2f;  // dependable = VH
        if (mx < mn)
          mx = mn;
        break;
      }

      default:
        break;
    }

    return {mn, mx};
  }

  inline std::pair<float, float> calculate_dynamic_range(
      const ParamSpec& spec, const Dataset& dataset, bool enabled_dyn = false) {
    float mn = spec.min;
    float mx = spec.max;

    if (!enabled_dyn) {
      return { mn, mx };
    }

    switch (spec.command) {

      // PS <= RU - 60
      case SendableCommands::PreStartTime: {
        const float ru = static_cast<float>(dataset.settings.run_up_delay_sec);
        mx = ru - 60.0f;
        if (mx < mn)
          mx = mn;
        break;
      }

        // LL <= WC
      case SendableCommands::LowOxygenLimit: {
        const float wc =
            static_cast<float>(dataset.settings.work_oxygen_concentration);
        mx = wc;
        if (mx < mn)
          mx = mn;
        break;
      }

        // TH >= TL
      case SendableCommands::TankPressureHighLimit: {
        const float tl = static_cast<float>(dataset.settings.tank_low_pressure);
        mn = tl;
        if (mn > mx)
          mn = mx;
        break;
      }

        // TL <= TH
      case SendableCommands::TankPressureLowLimit: {
        const float th =
            static_cast<float>(dataset.settings.tank_high_pressure);
        mx = th;
        if (mx < mn)
          mx = mn;
        break;
      }

        // VH > VL  (OpenTime >= CloseTime + 0.2)
      case SendableCommands::SpvValveOpenTime: {
        const float vl = static_cast<float>(dataset.settings.spv_off_time_sec);
        mn = vl + 0.2f;
        if (mn > mx)
          mn = mx;
        break;
      }

        // VL < VH  (CloseTime <= OpenTime - 0.2)
      case SendableCommands::SpvValveCloseTime: {
        const float vh = static_cast<float>(dataset.settings.spv_on_time_sec);
        mx = vh - 0.2f;
        if (mx < mn)
          mx = mn;
        break;
      }

      default:
        break;
    }

    return {mn, mx};
  }

#pragma endregion HELPERS

}  // namespace ON2Solutions::parser::paramspec
