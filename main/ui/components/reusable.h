#pragma once

#include <core/shortcuts.h>

#include "core/styling/styling.h"

#include <constants/assets.h>
#include <constants/theme.h>
#include <lg/helpers/utils.h>

using namespace foundation;
namespace ON2Solutions {



  inline $$Button $HighButton(const char* source, auto on_press) {
    return $Button(ButtonProps::up()
                       .click(on_press)
                       .set_style([](Styling& style) {
                         style.setBackgroundColor(PRIMARY_COLOR_3);
                         style.setWidth(44);
                         style.setHeight(220);
                         style.setPadding(0);
                       })
                       .set_child($Image(
                           ImageProps::up().source(source).height(14).width(9)))

    );
  }

  inline $$View $ConnectionStat() {
    return $View(
        ViewProps::up()
            .w(24)
            .h(24)
            .flow(FlexPreset::Center)
            .set_style([](Styling& style) {
              style.setBackgroundColor(PRIMARY_COLOR_3);
              style.setPadding(0);
            })
            .set_children(children($Image(
                ImageProps::up().source(assets::Plugs).height(16).width(16)))));
  }

  inline $$View $TimerView() {
    return $View(ViewProps::up()
                     .w(166)
                     .h(44)
                     .set_style([](Styling& style) {
                       style.setPadding(10, 10, 16, 16);
                       style.setBorderRadius(14);
                       style.setBackgroundColor(PRIMARY_COLOR_3);
                       style.setBorder(BORDER_PRIMARY, 1, LV_OPA_100);
                     })
                     .flow(FlexPreset::RowCenter)
                     .set_children(children(
                         $Text(TextProps::up().value(locales::en::timer)),
                         $Text(TextProps::up().value("0")))));
  }

  inline $$View $FooterFragment() {
    return $View(ViewProps::up()
                     .w(166)
                     .h(44)
                     .set_style([](Styling& style) {
                       style.setPadding(0);
                       style.setBorderRadius(0);
                       style.setBackgroundColor(PRIMARY_COLOR_3);
                       style.setBackgroundOpa(LV_OPA_0);
                       style.setBorder(BORDER_PRIMARY, 0, 0);

                     })
                     .flow(FlexPreset::RowCenter));
  }

}  // namespace ON2Solutions
