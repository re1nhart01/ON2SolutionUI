
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
        return "lpm";
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
    using DataPtr = const float(parser::Dataset::*);
    Delegate<float(const parser::Dataset&, uint8_t)> getValue;
    short precision = 0;
  };

  inline lv_color_t get_color_by_value(const float value, const int min,
                                       const int max) {
    if (max <= min)
      return BORDER_PRIMARY;

    float ratio = (value - min) / (max - min);

    if (ratio < 0.25f) {
      return ERROR_COLOR;
    }
    if (ratio < 0.75f) {
      return TERTIARY_COLOR;
    }
    return NO_ERROR_COLOR;
  }

  // Вспомогательная функция для получения настроек
  inline SelectorConfig get_config(CircularSelectorType type) {
    using D = parser::Dataset;
    switch (type) {
      case O2:
        return {" %", 0, 100, [](const D& v, uint8_t i) {
                  return v.operative_data.oxygen_levels[i];
                }, 1};
      case Ps:
        return {" psi", 0, 150, [](const D& v, uint8_t i) {
                  return v.operative_data.primary_tank_pressure[i];
                }};
      case Tr:
        return {" psi", 0, 60, [](const D& v, uint8_t i) {
                  return v.operative_data.secondary_tank_pressure[i];
                }};
      case Tn:
        return {" c", 0, 100, [](const D& v, uint8_t i) {
                  return v.operative_data.tank_temperatures[i];
                }, 1};
      case Fl:
        return {" lpm", 0, 20, [](const D& v, uint8_t i) {
                  return v.operative_data.oxygen_speed[i];
                }};
      default:
        return {"", 0, 100, [](const D& v, uint8_t i) { return 0; }};
    }
  }

  inline $$Circular $SpecificCircular(const CircularSelectorType& type,
                                      uint8_t index, bool is_small = false) {
    auto config = get_config(type);

    const auto start_value = parser::DatasetStore::getInstance()->get();
    const auto value = config.getValue(*start_value, index);

    return $Circular(
        CircularProgressProps::up()
            .watch<parser::Dataset>(
                parser::DatasetStore::getInstance(), "circular",
                [config, index](CircularProgress* self,
                                const parser::Dataset& dataset_value) {
                  if (!self)
                    return;
                  self->update_label_text();
                  float val = config.getValue(dataset_value, index);
                  self->set_state([val, config](CircularProgressProps& props) {
                    props.value(val);
                    props.set_arc_style([val, config](Styling& style) {
                      style.setArcColor(
                          get_color_by_value(val, config.min, config.max));
                    });
                  });
                })
            .label(config.symbol)
            .show_label(true)
            .min(config.min)
            .max(config.max)
            .decimals(config.precision)
            .value(value)
            .set_arc_style([value, config](Styling& style) {
              style.setArcColor(
                  get_color_by_value(value, config.min, config.max));
            })
            .set_text_style([is_small](Styling& style) {
              style.setFont(is_small ? &lv_font_montserrat_16
                                     : &lv_font_montserrat_34);
            })
            .w(is_small ? 98 : 232)
            .h(is_small ? 98 : 232));
  }

};  // namespace ON2Solutions
