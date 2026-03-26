
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
        return "";
      case Ps:
        return "";
      case Tr:
        return "";
      case Tn:
        return "";
      case Fl:
        return "";
      default:
        return "%";
    }
  }

  inline std::array<short, 2> min_max_by_type(const CircularSelectorType& type) {
    switch (type) {
      case O2:
        return {0, 100};
      case Ps:
        return {0, 100};
      case Tr:
        return {0, 100};
      case Tn:
        return {0, 100};
      case Fl:
        return {0, 100};
      default:
        return {0, 100};
    }
  }

  inline $$Circular $SpecificCircular(const CircularSelectorType& type, uint8_t index, bool is_small = false) {
    auto [min_val, max_val] = min_max_by_type(type);

    return $Circular(
        CircularProgressProps::up()
            .watch<parser::Dataset>(
                parser::DatasetStore::getInstance(), "circular",
                [type, index](CircularProgress* self, const parser::Dataset& value) {
                                  // if (type == O2) {
                                  //
                                  //   // self->set_state([val](MeterProps& props) { props.value(val); });
                                  // }
                              })
            .label(symbol_by_type(type))
            .show_label(true)
            .min(min_val)
            .max(max_val)
            .value(min_val)
            .set_text_style([is_small](Styling& style) {
                style.setFont(is_small ? &lv_font_montserrat_16 : &lv_font_montserrat_34);
            })
            .w(is_small ? 98 : 232)
            .h(is_small ? 98 : 232));
  }

};


