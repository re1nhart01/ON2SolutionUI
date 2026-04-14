
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

template <size_t N = 64, typename... Args>
std::array<char, N> fmt(const char* format, Args... args) {
  std::array<char, N> buf{};
  snprintf(buf.data(), buf.size(), format, args...);
  return buf;
}

template <size_t N = 64, typename Callback, typename... Args>
void fmt_cb(Callback&& cb, const char* fmt, Args... args) {
  char buf[N];
  snprintf(buf, N, fmt, args...);
  cb(buf);
}

inline short find_index(const std::vector<const char*>& options, const std::string& value) {
  const auto it = std::ranges::find_if(options,
      [&value](const char* opt) {
        return opt != nullptr && value == opt;
      });

  return (it != options.end()) ? static_cast<short>(std::distance(options.begin(), it)) : 0;
}

template <typename T, size_t N>
inline int find_index(const std::array<T, N>& arr, const std::string_view value) {
  auto it = std::ranges::find_if(arr.begin(), arr.end(), [value](const T& item) {
      return std::string_view(item) == value;
  });
  return (it != arr.end()) ? std::distance(arr.begin(), it) : 0;
}