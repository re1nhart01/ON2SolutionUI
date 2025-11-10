
#include <string>
#include "dataset.dto.cc"
#include "fun_helpers.cc"

//$AABBCCDDEEFFRQ</CH=2/ST=F/O2=17.4;0.0/FL=10.1;0.0/II=0F/IO=F0/ER=8FFFFF00/PS=51;0/HR=9:42>D6

static data_storage_array<float> unpack_arrays_float(std::string data) {
  data_storage_array<float> result{};

  std::string temporary;
  uint8_t index = 0;

  for (int i = 3; i <= data.size(); i++) {
      if (data[i] == ';' || data[i] == '/') {
          const float d_v = std::stof(temporary);

          result[index] = d_v;
          index++;
          temporary = "";
      } else {
          temporary += data[i];
      }
  }

  return result;
}


DatasetDTO parse_into_dataset(const std::string& value_string) {
    DatasetDTO result{};

    std::string temporary;
    bool locked = false;
    int index_of_start = 0;

    if (value_string.empty() || value_string[0] != '$') {
        return result;
    }

    for (int i = 1; i < value_string.length(); i++) {
        if (value_string[i] != '<') {
            result.device_number += value_string[i];
        } else {
            index_of_start = i + 1;
            break;
        }
    }

    for (int i = index_of_start; i < value_string.length(); i++) {
        if (locked) {
            temporary += value_string[i];
        }

        if (value_string[i] == '/' || value_string[i] == '>') {
            if (!temporary.empty()) {
                if (temporary.find("CH=", 0) == 0) {
                    result.channels = std::stoi(temporary.substr(3));
                }
                else if (temporary.find("ST=", 0) == 0) {
                    result.status = StringToDatasetStatus(temporary.substr(3, 1).c_str());
                }
                else if (temporary.find("O2=", 0) == 0) {
                    result.oxygen_levels = unpack_arrays_float(temporary);
                }
                else if (temporary.find("FL=", 0) == 0) {
                    result.oxygen_speed = unpack_arrays_float(temporary);
                }
                else if (temporary.find("II=", 0) == 0) {
                    result.inputs = std::stoi(temporary.substr(3), nullptr, 16);
                }
                else if (temporary.find("IO=", 0) == 0) {
                    result.outputs = std::stoi(temporary.substr(3), nullptr, 16);
                }
                else if (temporary.find("ER=", 0) == 0) {
                    result.errors = std::stoul(temporary.substr(3), nullptr, 16);
                }
                else if (temporary.find("PS=", 0) == 0) {
                    result.tanks_pressure = unpack_arrays_float(temporary);

                }
                else if (temporary.find("HR=", 0) == 0) {
                    result.moto_hours = temporary.substr(3);
                }
            }
            temporary.clear();
            locked = false;
        }

        if (value_string[i] == '/') {
            locked = true;
        }
    }

    result.crc = std::stoi(value_string.substr(value_string.length() - 2), nullptr, 16);

    return result;
}