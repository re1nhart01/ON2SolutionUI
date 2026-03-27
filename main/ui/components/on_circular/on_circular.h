
#pragma once

#include <core/shortcuts.h>

#include "core/styling/styling.h"

#include <constants/assets.h>
#include <constants/theme.h>
#include <lg/helpers/utils.h>

using namespace foundation;
namespace ON2Solutions {

  enum CircularSelectorType {
    O2,
    Ps,
    Fl,
    Tr,
    Tn,
  };

  inline std::string symbol_by_type(const CircularSelectorType& type) {
    switch (type) {
      case O2:
        return "%";
      case Ps:
        return "psi";
      case Tr:
        return "psi";
      case Tn:
        return "C";
      case Fl:
        return "l/m";
      default:
        return "%";
    }
  }

  inline std::array<short, 2> min_max_by_type(
      const CircularSelectorType& type) {
    switch (type) {
      case O2:
        return {0, 100};
      case Ps:
        return {0, 150};
      case Tr:
        return {0, 60};
      case Tn:
        return {0, 100};
      case Fl:
        return {0, 20};
      default:
        return {0, 100};
    }
  }

  struct SelectorConfig {
    std::string symbol;
    short min;
    short max;
    // Указатель на член данных (массив) в структуре OperativeData
    // Предполагаем, что operative_data — это структура с массивами типа float или int
    using DataPtr = const float (parser::Dataset::*);
    // Если типы массивов разные, лучше использовать std::function
    std::function<float(const parser::Dataset&, uint8_t)> getValue;
};

// Вспомогательная функция для получения настроек
inline SelectorConfig get_config(CircularSelectorType type) {
    using D = parser::Dataset;
    switch (type) {
        case O2: return {"%",   0, 100, [](const D& v, uint8_t i) { return v.operative_data.oxygen_levels[i]; }};
        case Ps: return {"psi", 0, 150, [](const D& v, uint8_t i) { return v.operative_data.primary_tank_pressure[i]; }};
        case Tr: return {"psi", 0, 60,  [](const D& v, uint8_t i) { return v.operative_data.secondary_tank_pressure[i]; }};
        case Tn: return {"C",   0, 100, [](const D& v, uint8_t i) { return v.operative_data.tank_temperatures[i]; }};
        case Fl: return {"l/m", 0, 20,  [](const D& v, uint8_t i) { return v.operative_data.oxygen_speed[i]; }};
        default: return {"%",   0, 100, [](const D& v, uint8_t i) { return 0; }};
    }
}

inline $$Circular $SpecificCircular(const CircularSelectorType& type, uint8_t index, bool is_small = false) {
    auto config = get_config(type);

    return $Circular(CircularProgressProps::up()
        .watch<parser::Dataset>(
            parser::DatasetStore::getInstance(), "circular",
            [config, index](CircularProgress* self, const parser::Dataset& value) {
                self->update_label_text();
                float val = config.getValue(value, index);
                self->set_state([val](CircularProgressProps& props) { props.value(val); });
            })
        .label(config.symbol)
        .show_label(true)
        .min(config.min)
        .max(config.max)
        .value(config.min)
        .set_text_style([is_small](Styling& style) {
            style.setFont(is_small ? &lv_font_montserrat_16 : &lv_font_montserrat_34);
        })
        .w(is_small ? 98 : 232)
        .h(is_small ? 98 : 232));
}

};  // namespace ON2Solutions
