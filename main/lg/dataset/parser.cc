
#include <string>
#include "dataset.dto.cc"
#include "fun_helpers.cc"

#include "esp_random.h"


static const std::string packets[10] = {
  "$AABBCCDDEEFFRQ</CH=3/ST=4/O2=18.2;0.1/FL=11.3;0.2/II=1A/IO=0F/ER=7ABCDE10/PS=49;1/HR=8:21>D5",
  "$AABBCCDDEEFFRQ</CH=1/ST=7/O2=16.9;0.3/FL=12.0;0.5/II=2B/IO=1E/ER=6F00AA33/PS=52;2/HR=7:58>C1",
  "$AABBCCDDEEFFRQ</CH=5/ST=2/O2=19.7;0.2/FL=14.4;0.4/II=3C/IO=2D/ER=55FF1020/PS=45;3/HR=9:07>AF",
  "$AABBCCDDEEFFRQ</CH=4/ST=1/O2=20.3;0.6/FL=13.1;0.1/II=4D/IO=3C/ER=442211AA/PS=56;4/HR=10:11>BB",
  "$AABBCCDDEEFFRQ</CH=6/ST=3/O2=21.8;0.8/FL=15.5;0.7/II=5E/IO=4B/ER=33C0F0A1/PS=43;5/HR=11:22>E3",
  "$AABBCCDDEEFFRQ</CH=2/ST=8/O2=22.5;0.4/FL=16.9;0.6/II=6F/IO=5A/ER=2288AABB/PS=47;6/HR=6:39>9A",
  "$AABBCCDDEEFFRQ</CH=7/ST=9/O2=15.3;0.2/FL=10.8;0.9/II=7A/IO=69/ER=11BBCC88/PS=54;7/HR=5:12>F4",
  "$AABBCCDDEEFFRQ</CH=8/ST=A/O2=23.4;1.1/FL=18.3;1.0/II=8B/IO=78/ER=0FAACC55/PS=42;8/HR=12:09>A7",
  "$AABBCCDDEEFFRQ</CH=9/ST=6/O2=17.8;0.9/FL=19.0;0.3/II=9C/IO=87/ER=AA55F0C3/PS=58;9/HR=4:49>C9",
  "$AABBCCDDEEFFRQ</CH=A/ST=5/O2=24.7;1.3/FL=17.6;1.2/II=AD/IO=96/ER=BC0D5512/PS=50;10/HR=3:37>D1"
};

uint32_t random_in_range(uint32_t min, uint32_t max) {
  uint32_t r = esp_random();
  return min + r % (max - min + 1);
}


static data_storage_array<float> unpack_arrays_float(const std::string& data) {
  data_storage_array<float> result{};
  std::string tmp;
  uint8_t idx = 0;

  for (size_t i = 3; i < data.size(); i++) {
      if (data[i] == ';' || data[i] == '/') {
          result[idx++] = stfl(tmp);
          tmp.clear();
      } else {
          tmp += data[i];
      }
      if (idx >= result.size()) break;
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
                    result.channels = stin(temporary.substr(3));
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
                    result.inputs = shtin(temporary.substr(3));
                }
                else if (temporary.find("IO=", 0) == 0) {
                    result.outputs = shtin(temporary.substr(3));
                }
                else if (temporary.find("ER=", 0) == 0) {
                    result.errors = shtul(temporary.substr(3));
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