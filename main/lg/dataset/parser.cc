
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
static data_storage_array<float> unpack_arrays_float(const char* start, size_t len) {
    data_storage_array<float> result{};

    const char* current = start + 3;
    const char* data_start = current;
    uint8_t idx = 0;

    while (current < start + len) {
        if (*current == ';' || *current == '/') {
            size_t val_len = current - data_start;

            result[idx++] = parse_float(data_start, val_len);
            data_start = current + 1;
        }

        if (idx >= result.size()) break;
        current++;
    }

    if (idx < result.size() && data_start < start + len) {
        size_t val_len = (start + len) - data_start;
        result[idx] = parse_float(data_start, val_len);
    }

    return result;
}

DatasetDTO parse_into_dataset(const std::string& value_string) {
    DatasetDTO result{};

    if (value_string.empty() || value_string[0] != '$') {
        return result;
    }

    const char* raw_packet = value_string.c_str();
    const char* current = raw_packet + 1;
    const char* data_start;
    size_t data_len;

    const char* tag_start = strchr(current, '<');
    if (!tag_start) return result;

    data_len = tag_start - current;
    if (data_len < sizeof(result.device_number)) {
        strncpy(result.device_number, current, data_len);
        result.device_number[data_len] = '\0';
    }

    current = tag_start + 1;
    const char* end_packet = strchr(current, '>');
    if (!end_packet) return result;

    while (current < end_packet) {
        data_start = current;
        current = strchr(current, '/');

        if (!current || current > end_packet) {
            current = end_packet;
        }

        data_len = current - data_start;

        if (data_len > 3) {
            const char* tag_key = data_start;

            const char* tag_value = data_start + 3;
            size_t value_len = data_len - 3;

            if (strncmp(tag_key, "CH=", 3) == 0) {
                result.channels = static_cast<uint8_t>(parse_int(tag_value, value_len));
            }
            else if (strncmp(tag_key, "ST=", 3) == 0) {
                char status_char = tag_value[0];
                char status_str[2] = {status_char, '\0'};
                result.status = StringToDatasetStatus(status_str);
            }
            else if (strncmp(tag_key, "O2=", 3) == 0) {
                result.oxygen_levels = unpack_arrays_float(data_start, data_len);
            }
            else if (strncmp(tag_key, "FL=", 3) == 0) {
                result.oxygen_speed = unpack_arrays_float(data_start, data_len);
            }
            else if (strncmp(tag_key, "II=", 3) == 0) {
                result.inputs = (uint8_t)parse_hex_to_int(tag_value, value_len);
            }
            else if (strncmp(tag_key, "IO=", 3) == 0) {
                result.outputs = (uint8_t)parse_hex_to_int(tag_value, value_len);
            }
            else if (strncmp(tag_key, "ER=", 3) == 0) {
                result.errors = parse_hex_to_ul(tag_value, value_len);
            }
            else if (strncmp(tag_key, "PS=", 3) == 0) {
                result.tanks_pressure = unpack_arrays_float(data_start, data_len);
            }
            else if (strncmp(tag_key, "HR=", 3) == 0) {
                if (value_len < sizeof(result.moto_hours)) {
                    strncpy(result.moto_hours, tag_value, value_len);
                    result.moto_hours[value_len] = '\0';
                }
            }
        }

        if (*current == '/') {
            current++;
        }
    }

    const char* crc_start = value_string.c_str() + value_string.length() - 2;
    result.crc = static_cast<uint8_t>(parse_hex_to_int(crc_start, 2));

    return result;
}