//
// Created by evgeniy on 1/3/26.
//

#include "serializer.h"
#include <string>

#include <lg/helpers/utils.h>

namespace ON2Solutions::parser {

  template <typename T>
  std::string serialize(const SerializableCommand<T>& command) {
    if (command.command == SendableCommands::StatusCommand) {
      return "<ST>";
    }
    if (command.command == SendableCommands::ResetMotoCommand) {
      return "<RS>";
    }
    if (command.command == SendableCommands::CabinetLight) {
      return "<CL>";
    }
    if (command.command == SendableCommands::SetDefault) {
      return "<RD>";
    }
    if constexpr (std::is_same_v<T, const char*>) {
      if (command.command == SendableCommands::TankPressureSensorType) {
        return fmt_str("<PT%s>", command.data);
      }

      if (command.command == SendableCommands::TankPressureSensorType) {}
    }

    auto command_str = command_to_string(command.command);

    std::string result;

    result.reserve(16);
    result += "<";
    result += command_str;
    if (command.with_num_sensor) {
      result += static_cast<char>('0' + command.num_sensor);
    }

    if constexpr (std::is_floating_point_v<T>) {
      auto v = normalize_value<T>(command.data, command.min, command.max);

      char buf[16];
      std::snprintf(buf, sizeof(buf),
                    command.with_num_sensor ? "%+.1f" : "%.1f",
                    static_cast<float>(v));
      result += buf;
    } else if constexpr (std::is_integral_v<T>) {
      auto v = normalize_value<T>(command.data, command.min, command.max);
      result += std::to_string(static_cast<int>(v));
    } else if constexpr (std::is_same_v<T, std::string_view> ||
                         std::is_same_v<T, char>) {
      result += command.data;
    }

    result += ">";
    return result;
  }

  template std::string serialize<float>(const SerializableCommand<float>&);
  template std::string serialize<uint8_t>(const SerializableCommand<uint8_t>&);
  template std::string serialize<uint16_t>(
      const SerializableCommand<uint16_t>&);
  template std::string serialize<int16_t>(const SerializableCommand<int16_t>&);
  template std::string serialize<const char*>(
      const SerializableCommand<const char*>&);
  template std::string serialize<char>(const SerializableCommand<char>&);

}  // namespace ON2Solutions::parser