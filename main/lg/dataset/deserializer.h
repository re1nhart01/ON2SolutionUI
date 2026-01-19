#pragma once

#include "esp_log.h"
#include "lg/dataset/types.h"
#include "lg/dataset/helpers.h"
#include "lg/dataset/serializer.h"
#include "lg/dataset/paramspec.h"

namespace ON2Solutions::parser {

  enum class PacketType : int8_t { UNKNOWN = -1, OPERATIVE = 0, SETTINGS = 1, SYSTEM_INFO = 2 };

  PacketType validate_type(const char* packet_string, size_t len);
  void parse_selected_value(Dataset* dataset, const char* key, int key_len, const char* val, int val_len, PacketType type);
  void parse_data(char packet_start, Dataset* dataset, const char* packet_string, PacketType type);
  void parse(Dataset* dataset, const char* packet_string, size_t len);
}