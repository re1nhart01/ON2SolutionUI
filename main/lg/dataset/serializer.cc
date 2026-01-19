//
// Created by evgeniy on 1/3/26.
//

#include <string>
#include "serializer.h"


namespace ON2Solutions::parser {

  template<typename T>
  std::string serialize(const SerializableCommand<T>& command) {
    if (command.command == SendableCommands::StatusCommand) {
      return "<ST>";
    }
    if (command.command == SendableCommands::ResetMotoCommand) {
      return "<RS>";
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
      std::snprintf(buf, sizeof(buf), command.with_num_sensor ? "%+.1f" : "%.1f", static_cast<float>(v));
      result += buf;
    } else if constexpr (std::is_integral_v<T>) {
      auto v = normalize_value<T>(command.data, command.min, command.max);
      result += std::to_string(static_cast<int>(v));
    } else if constexpr (std::is_same_v<T, std::string_view> || std::is_same_v<T, char>) {
      result += command.data;
    }


    result += ">";
    return result;
  }

  template std::string serialize<float>(const SerializableCommand<float>&);
  template std::string serialize<int16_t>(const SerializableCommand<int16_t>&);
  template std::string serialize<std::string_view>(const SerializableCommand<std::string_view>&);
  template std::string serialize<char>(const SerializableCommand<char>&);

}  // namespace ON2Solutions::parser