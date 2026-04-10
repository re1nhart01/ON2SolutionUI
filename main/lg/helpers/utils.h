
#pragma once

#include <vector>

inline bool is_up_bit_pos(uint32_t value, short pos = 0)
{
  short bit_pos = (value >> pos) & 1;

  if (bit_pos == 1) {
    return true;
  }

  return false;
}

/*
 *
* 0000 1110  (это a >> 2)
  & 0000 0001  (это 1)
  -----------
  0000 0000  <-- Result 0
 *
 *
 *  pos count from zero
 */

template<typename... Args, size_t Capacity = 64>
std::string fmt_str(const char* fmt, Args... args) {
  char buf[Capacity];
  snprintf(buf, sizeof(buf), fmt, args...);
  return std::string(buf);
}

inline short find_index(const std::vector<const char*>& options, const std::string& value) {
  const auto it = std::ranges::find_if(options,
      [&value](const char* opt) {
        return opt != nullptr && value == opt;
      });

  return (it != options.end()) ? static_cast<short>(std::distance(options.begin(), it)) : 0;
}