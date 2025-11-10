//
// Created by evgeniy on 4/28/25.
//

#include <cstring>
bool contains_substring(const char* haystack, const char* needle) {
  if (!*needle) return true;

  const char* p1 = haystack;
  const char* p2 = needle;

  while (*p1) {
      const char* h = p1;
      const char* n = p2;

      while (*h && *n && (*h == *n)) {
          ++h;
          ++n;
      }

      if (!*n) {
          return true;
      }

      ++p1;
  }

  return false;
}

static bool starts_with(const char* str, const char* prefix) {
  if (!str || !prefix) return false;

  size_t len_str = strlen(str);
  size_t len_prefix = strlen(prefix);

  if (len_prefix > len_str) {
      return false;
  }

  for (size_t i = 0; i < len_prefix; ++i) {
      if (str[i] != prefix[i]) {
          return false;
      }
  }

  return true;
}

char* substring(const char* str, size_t start, size_t length) {
  if (!str) return nullptr;

  size_t str_len = strlen(str);

  if (start >= str_len) return nullptr;

  if (start + length > str_len) {
      length = str_len - start;
  }

  char* result = (char*)malloc(length + 1); // +1 для '\0'
  if (!result) return nullptr;

  strncpy(result, str + start, length);
  result[length] = '\0';

  return result;
}