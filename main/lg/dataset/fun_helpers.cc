#include <cstring>
#include <string>

inline int parse_hex_to_int(const char* start, size_t len, int def = 0) {
  if (len == 0) return def;

  char buffer[16];
  if (len >= sizeof(buffer)) len = sizeof(buffer) - 1;

  strncpy(buffer, start, len);
  buffer[len] = '\0';

  char* end;
  long v = strtol(buffer, &end, 16);
  if (end == buffer) return def;
  return (int)v;
}

inline unsigned long parse_hex_to_ul(const char* start, size_t len, unsigned long def = 0) {
  if (len == 0) return def;

  char buffer[16];
  if (len >= sizeof(buffer)) len = sizeof(buffer) - 1;

  strncpy(buffer, start, len);
  buffer[len] = '\0';

  char* end;
  unsigned long v = strtoul(buffer, &end, 16);
  if (end == buffer) return def;
  return v;
}

inline int parse_int(const char* start, size_t len, int def = 0) {
  if (len == 0) return def;

  char buffer[16];
  if (len >= sizeof(buffer)) len = sizeof(buffer) - 1;

  strncpy(buffer, start, len);
  buffer[len] = '\0';

  char* end;
  long v = strtol(buffer, &end, 10);
  if (end == buffer) return def;
  return (int)v;
}

inline float parse_float(const char* start, size_t len, float def = 0.f) {
  if (len == 0) return def;

  char buffer[16];
  if (len >= sizeof(buffer)) len = sizeof(buffer) - 1;

  strncpy(buffer, start, len);
  buffer[len] = '\0';

  char* end;
  float v = strtof(buffer, &end);
  if (end == buffer) return def;
  return v;
}

