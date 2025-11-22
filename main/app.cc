#include "core/navigation/stack_navigator/stack_navigator.h"
#include "ui/screens/main_screen.cc"
#include "ui/screens/pincode_screen.cc"
#include "ui/screens/preloder_screen.cc"
#include "ui/screens/settings_screen.cc"

extern "C" {
  #include "../components/foundation/internals/lvgl_port.h"
}

using namespace foundation;

auto screen = lv_scr_act();

class WaveApplication final : public Application {
  StackNavigator stack_navigator;

  MainScreen main_screen;
  SettingsScreen settings_screen;
  PinCodeScreen pincode_screen;
  PreloaderScreen preloader_screen;

public:
  explicit WaveApplication(lv_obj_t *screen)
      : Application(screen),
        stack_navigator(StackNavigatorConfig{.initial_route = "/main"}, screen),
        main_screen(&stack_navigator, MainScreenProps{}),
        settings_screen(&stack_navigator, SettingsScreenProps{}),
        pincode_screen(&stack_navigator, PinCodeScreenProps{}),
        preloader_screen(&stack_navigator, PreloaderScreenProps{})
  {}

  VNode *root_component() override {
    return stack_navigator.getCurrentComponent();
  }

  void before_load_application() override {
    stack_navigator.registerScreen("/preloader", &preloader_screen);
    stack_navigator.registerScreen("/main", &main_screen);
    stack_navigator.registerScreen("/pin_code", &pincode_screen);
    stack_navigator.registerScreen("/settings", &settings_screen);
  }

  void after_load_application() override {
    ESP_LOGI("MyApp", "after_load_application called");
  }
};