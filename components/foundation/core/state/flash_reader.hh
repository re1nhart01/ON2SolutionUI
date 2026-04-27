//
// Created by evgeniy on 4/22/26.
//

#pragma once

namespace foundation {
  using AnyValue = std::variant<int, double, std::string, bool, float>;
  class FlashReader {
   private:
    std::string namespace_name;
    std::unordered_map<std::string, AnyValue> nvs_map;

    template <typename T>
    esp_err_t _nvs_write(nvs_handle_t h, const char* key, T value) {
      if constexpr (std::is_same_v<T, int32_t>)
        return nvs_set_i32(h, key, value);
      if constexpr (std::is_same_v<T, bool>)
        return nvs_set_u8(h, key, value ? 1 : 0);
      if constexpr (std::is_same_v<T, float>)
        return nvs_set_blob(h, key, &value, sizeof(float));
      if constexpr (std::is_same_v<T, std::string>)
        return nvs_set_str(h, key, value.c_str());
      return ESP_ERR_NOT_SUPPORTED;
    }

   public:
    explicit FlashReader(const std::string& n) : namespace_name(n) {
      esp_err_t err = nvs_flash_init();

      if (err == ESP_ERR_NVS_NO_FREE_PAGES ||
          err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        nvs_flash_init();
      }
    }

    ~FlashReader() = default;

    template <typename T>
    void set_value(const char* key, T value) {
      nvs_handle_t h;
      if (nvs_open(namespace_name.c_str(), NVS_READWRITE, &h) == ESP_OK) {
        if (this->_nvs_write(h, key, value) == ESP_OK) {
          this->nvs_map[key] = value;
          nvs_commit(h);
        }
        nvs_close(h);
      }
    }

    template <typename T>
    T get_value(const char* key, T default_value) {
      if (nvs_map.contains(key)) {
        return std::get<T>(nvs_map[key]);;
      }

      nvs_handle_t h;
      T result = default_value;
      if (nvs_open(namespace_name.c_str(), NVS_READONLY, &h) == ESP_OK) {
        esp_err_t err = ESP_FAIL;
        if constexpr (std::is_same_v<T, int32_t>) {
          err = nvs_get_i32(h, key, &result);
        } else if constexpr (std::is_same_v<T, bool>) {
          uint8_t val;
          err = nvs_get_u8(h, key, &val);
          if (err == ESP_OK)
            result = (val == 1);
        } else if constexpr (std::is_same_v<T, float>) {
          size_t size = sizeof(float);
          err = nvs_get_blob(h, key, &result, &size);
        }else if constexpr (std::is_same_v<T, const char*>) {
          if (nvs_map.contains(key)) {
            return std::get<std::string>(nvs_map[key]).c_str();
          }

          size_t required_size;
          err = nvs_get_str(h, key, nullptr, &required_size);
          if (err == ESP_OK) {
            std::string str_result(required_size, '\0');
            nvs_get_str(h, key, &str_result[0], &required_size);

            if (!str_result.empty() && str_result.back() == '\0') str_result.pop_back();

            nvs_map[key] = str_result; // Зберігаємо в кеш як string
            result = std::get<std::string>(nvs_map[key]).c_str();
          }
        }
        else if constexpr (std::is_same_v<T, std::string>) {
          size_t required_size;
          err = nvs_get_str(h, key, nullptr, &required_size);
          if (err == ESP_OK) {
            const auto buf = new char[required_size];
            nvs_get_str(h, key, buf, &required_size);
            result = std::string(buf);
            delete[] buf;
          }
        }

        if (err == ESP_OK)
          nvs_map[key] = result;
        nvs_close(h);
      }
      return result;
    }
  };
}  // namespace foundation