#include "charts_screen.h";

namespace ON2Solutions {
  void ChartsScreen::component_did_mount() {
    ESP_LOGI("preloader_screen", "Preloading screen");
    this->navigate_after();
  }

  void ChartsScreen::navigate_after() const {
    TimerHandle_t timeout_handle = xTimerCreate(
        "navigate_timer", pdMS_TO_TICKS(4000), pdFALSE, this->navigation_ref,
        [](TimerHandle_t timer) {
          lv_async_call(
              [](void* data) {
                auto* navigator = static_cast<StackNavigator*>(data);

                navigator->navigate("/main");
              },
              pvTimerGetTimerID(timer));
        });

    xTimerStart(timeout_handle, 0);
  }

  lv_obj_t* ChartsScreen::render() {
    NavigationScreen::render();

    return this->delegate($View(
        ViewProps::up()
            .set_style([](Styling& style) {
              style.setPadding(0, 0, 0, 0);
              style.setBorderRadius(0);
              style.setBorder(lv_color_make(255, 255, 255), 0, 0);
            })
            .set_children(children(
                $Image(ImageProps::up()
                           .source("S:on2_logo_300_192.bin")
                           .width(310)
                           .height(192)),
                $Activity(ActivityIndicatorProps::up().sz(86).arc(80).set_color(
                    PRIMARY_COLOR))))
            .w(LV_PCT(100))
            .h(LV_PCT(100))
            .justify(LV_FLEX_ALIGN_CENTER)
            .items(LV_FLEX_ALIGN_CENTER)
            .track_cross(LV_FLEX_ALIGN_CENTER)
            .direction(LV_FLEX_FLOW_COLUMN)));
  }

  ChartsScreen* ChartsScreen::append(lv_obj_t* obj) {
    lv_obj_set_parent(obj, get_component());
    return this;
  }
}  // namespace ON2Solutions
