
#include <constants/localization.h>
#include <lg/dataset/store/dataset.store.h>

#include "preloader_screen.h";

namespace ON2Solutions {
  using namespace parser;
  void PreloaderScreen::component_did_mount() {
    ESP_LOGI("preloader_screen", "Preloading screen");
    const int8_t bootloader_mode =
        parser::DatasetStore::getInstance()->get()->optional.bootloader_mode;

    if (bootloader_mode != parser::BootStatus::BootloaderReady) {
      this->navigate_after();
    }
  }

  void PreloaderScreen::navigate_after() const {
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

  lv_obj_t* PreloaderScreen::render() {
    NavigationScreen::render();

    const bool is_active_bootloader_mode =
        parser::DatasetStore::getInstance()->get()->optional.bootloader_mode ==
        -1;

    return this->delegate($View(
        ViewProps::up()
            .set_style([](Styling& style) {
              style.setPadding(0, 0, 0, 0);
              style.setBorderRadius(0);
              style.setBorder(lv_color_make(255, 255, 255), 0, 0);
            })
            .set_children(children(
                $Image(ImageProps::up()
                           .source(assets::LogoBig)
                           .width(310)
                           .height(192)),

                $Activity(ActivityIndicatorProps::up().sz(86).arc(80).set_color(
                    PRIMARY_COLOR)),
                $Text(TextProps::up()
                          .watch<Dataset>(
                              DatasetStore::getInstance(), "bootloader_text",
                              [this](Text* self, const Dataset& value) {
                                if (!self)
                                  return;
                                const int8_t status =
                                    value.optional.bootloader_mode;
                                std::string status_str =
                                    GetBootloaderStatus(status);
                                self->set_state([status_str, status,
                                                 this](TextProps& props) {
                                  props.value(status_str);

                                  if (status ==
                                      BootStatus::UploadedSuccessfully) {
                                    this->navigate_after();
                                    DatasetStore::getInstance()->set_silent(
                                        [](const Dataset& prev) {
                                          Dataset next = prev;
                                          next.optional.bootloader_mode = -1;
                                          return next;
                                        });
                                  }

                                  props.set_style([status](Styling& style) {
                                    if (status != -1) {
                                      style.setOpacity(LV_OPA_100);
                                    } else {
                                      style.setOpacity(LV_OPA_0);
                                    }
                                  });
                                });
                              })
                          .value(locales::en::bootloader_status_1)
                          .set_style([](Styling& style) {
                            style.setOpacity(LV_OPA_0);
                            style.setFont(&lv_font_montserrat_18);
                          }))))
            .w(LV_PCT(100))
            .h(LV_PCT(100))
            .justify(LV_FLEX_ALIGN_CENTER)
            .items(LV_FLEX_ALIGN_CENTER)
            .track_cross(LV_FLEX_ALIGN_CENTER)
            .direction(LV_FLEX_FLOW_COLUMN)));
  }

  PreloaderScreen* PreloaderScreen::append(lv_obj_t* obj) {
    lv_obj_set_parent(obj, get_component());
    return this;
  }
}  // namespace ON2Solutions
