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

  inline $$Animated $TimerView(
      const std::shared_ptr<AnimatedControl>& alarm_control) {
    return $Animated(
        AnimatedProps::up()
            .from(0)
            .to(255)
            .stop(0)
            .time(200)
            .playback(0)
            .repeat(1)
            .wait(0)
            .easing(lv_anim_path_ease_in_out)
            .set_control("alarm_anim", alarm_control)
            .set_auto_start(false)
            .prop(AnimatedProps::Property::Opacity)
            .on($View(
                ViewProps::up()
                    .w(166)
                    .h(44)
                    .set_style([](Styling& style) {
                      style.setPadding(10, 10, 16, 16);
                      style.setBorderRadius(14);
                      style.setBackgroundColor(PRIMARY_COLOR_3);
                      style.setBorder(BORDER_PRIMARY, 1, LV_OPA_100);
                      style.setOpacity(LV_OPA_0);
                      style.setTextOpacity(LV_OPA_0);
                    })
                    .flow(FlexPreset::RowCenter)
                    .set_children(children(
                        $Text(TextProps::up().value(locales::en::timer)),
                        $Text(
                            TextProps::up()
                                .watch<parser::Dataset>(
                                    parser::DatasetStore::getInstance(),
                                    "alarm_text",
                                    [alarm_control](
                                        Text* self,
                                        const parser::Dataset& value) {
                                      if (!self) return;
                                      bool is_alarm_now =
                                          (value.operative_data.status[0] ==
                                           'F');

                                      uint8_t count =
                                          value.optional.reset_countdown_sec;

                                      self->set_state(
                                          [count](TextProps& props) {
                                            props.value(fmt_str("%d", count));
                                          });

                                      if (is_alarm_now) {
                                        alarm_control->play(
                                            "alarm_anim");
                                      } else {
                                        alarm_control->stop(
                                            "alarm_anim");
                                      }
                                    })
                                .value("0")))))));
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
