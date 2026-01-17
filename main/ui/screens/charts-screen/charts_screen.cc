#include "charts_screen.h"

using namespace foundation;

namespace ON2Solutions {
  void ChartsScreen::component_did_mount() {};

  lv_obj_t* ChartsScreen::render() {
    VNode::render();
    auto navigation_ref = this->navigation_ref;

    return this->delegate($View(
        ViewProps::up()
            .set_style([](Styling& style) {
              style.setTextColor(lv_color_make(255, 255, 255));
              style.setPadding(0, 0, 0, 0);
              style.setBorderRadius(0);
              style.setBorder(lv_color_make(255, 255, 255), 0, 0);
            })
            .set_children(children(
                $StatusBar(StatusBarProps::up()),
                $View(
                    ViewProps::up()
                        .w(LV_PCT(98))
                        .h(60)
                        .direction(LV_FLEX_FLOW_ROW)
                        .items(LV_FLEX_ALIGN_CENTER)
                        .track_cross(LV_FLEX_ALIGN_CENTER)
                        .justify(LV_FLEX_ALIGN_SPACE_BETWEEN)
                        .set_children(children(
                            $View(
                                ViewProps::up()
                                    .w(LV_PCT(20))
                                    .h(60)
                                    .direction(LV_FLEX_FLOW_ROW)
                                    .items(LV_FLEX_ALIGN_CENTER)
                                    .justify(LV_FLEX_ALIGN_START)
                                    .track_cross(LV_FLEX_ALIGN_CENTER)
                                    .set_style($s("common.no_padding"))
                                    .set_children(children($Button(
                                        ButtonProps::up()
                                            .set_child(
                                                $Text(TextProps::up().value(
                                                    locales::en::button_back)))
                                            .click([navigation_ref](
                                                       lv_event_t* e) {
                                              navigation_ref->goBack();
                                            }))))),
                            $View(ViewProps::up()
                                      .w(LV_PCT(78))
                                      .h(60)
                                      .set_style($s("common.no_padding"))
                                      .direction(LV_FLEX_FLOW_ROW)
                                      .items(LV_FLEX_ALIGN_CENTER)
                                      .justify(LV_FLEX_ALIGN_CENTER)
                                      .track_cross(LV_FLEX_ALIGN_CENTER)
                                      .set_children(
                                          children($Text(TextProps::up().value(
                                              locales::en::
                                                  system_auth_header))))),
                            $View(ViewProps::up()
                                      .w(LV_PCT(20))
                                      .set_style($s("common.no_padding"))
                                      .set_children(children(
                                          $Fragment(FragmentProps::up())))))))))
            .merge(screen_container_props)));
  }

  const Styling* ChartsScreen::styling() const {
    this->style.setTextColor(lv_color_make(255, 255, 255));
    this->style.setPadding(0, 0, 0, 0);
    this->style.setBorderRadius(0);
    this->style.setBorder(lv_color_make(255, 255, 255), 0, 0);

    return &this->style;
  }

  ChartsScreen* ChartsScreen::append(lv_obj_t* obj) {
    lv_obj_set_parent(obj, get_component());
    return this;
  }
}  // namespace ON2Solutions