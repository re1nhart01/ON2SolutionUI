#pragma once
#include <charconv>
#include <cstring>
#include <string>
#include <limits>
#include <type_traits>
#include "esp_log.h";

namespace ON2Solutions::parser::helpers {

  template <typename T>
  inline bool parse_hex(const char* p, int len, T& out)
  {
    out = 0;
    for (int i = 0; i < len; i++) {
      char c = p[i];
      out <<= 4;
      if (c >= '0' && c <= '9') out |= c - '0';
      else if (c >= 'A' && c <= 'F') out |= c - 'A' + 10;
      else if (c >= 'a' && c <= 'f') out |= c - 'a' + 10;
      else return false;
    }
    return true;
  }

  template <typename T>
  inline bool parse_value(const char* p, int len, T& out)
  {
    static_assert(std::is_unsigned<T>::value, "T must be unsigned integer");

    if (!p || len <= 0) return false;

    long long tmp = 0;

    auto res = std::from_chars(p, p + len, tmp);
    if (res.ec != std::errc()) return false;

    if (tmp > std::numeric_limits<T>::max()) return false;

    out = static_cast<T>(tmp);
    return true;
  }

  inline bool parse_value(const char* p, int len, float& out)
  {

    if (!p || len <= 0) return false;

    int i = 0;
    bool neg = false;

    if (p[i] == '-') {
      neg = true;
      i++;
    } else if (p[i] == '+') {
      i++;
    }

    int ip = 0;
    bool has_digits = false;

    while (i < len && p[i] >= '0' && p[i] <= '9') {
      ip = ip * 10 + (p[i] - '0');
      i++;
      has_digits = true;
    }

    float fp = 0.0f;
    float div = 1.0f;

    if (i < len && p[i] == '.') {
      i++;
      while (i < len && p[i] >= '0' && p[i] <= '9') {
        fp = fp * 10.0f + (p[i] - '0');
        div *= 10.0f;
        i++;
        has_digits = true;
      }
    }

    if (!has_digits) return false;

    float v = ip + fp / div;
    out = neg ? -v : v;
    return true;
  }

  template <typename T, size_t N>
  inline int parse_list(const char* p, int len, std::array<T, N>& out)
  {

    int count = 0;
    int start = 0;

    for (int i = 0; i < len && count < N; i++) {
      if (p[i] == ';') {
        if (!parse_value(p + start, i - start, out[count])) {
          return count;
        }
        count++;
        start = i + 1;
      }
    }

    if (count < N && start < len) {
      parse_value(p + start, len - start, out[count]);
      count++;
    }

    return count;
  }

  inline void copy_string(void* dest, size_t dest_len, const void* src, size_t src_len)
  {
    if (!dest || !src || dest_len == 0) return;

    size_t n = (src_len < dest_len - 1) ? src_len : dest_len - 1;

    memset(dest, 0, dest_len);
    memcpy(dest, src, n);
  }
}  // namespace ON2Solutions::parser::helpers
