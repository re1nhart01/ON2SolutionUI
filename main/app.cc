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

auto stack_navigator = std::make_shared<
  StackNavigator>(StackNavigatorConfig{.initial_route = "/main"}, screen);

auto main_screen = std::make_shared<MainScreen>(
    stack_navigator, MainScreenProps{});
auto settings_screen = std::make_shared<SettingsScreen>(
    stack_navigator, SettingsScreenProps{});
auto pincode_screen = std::make_shared<PinCodeScreen>(
    stack_navigator, PinCodeScreenProps{});
auto preloader_screen = std::make_shared<PreloaderScreen>(
    stack_navigator, PreloaderScreenProps{});

class WaveApplication : public Application {
public:
  explicit WaveApplication(lv_obj_t *screen)
    : Application(screen) {
  }

  VNode *root_component() override {
    const auto initial = stack_navigator->getCurrentComponent();

    return initial.get();
  }

  void before_load_application() override {
    stack_navigator->registerScreen("/main", main_screen);
    stack_navigator->registerScreen("/pin_code", pincode_screen);
    stack_navigator->registerScreen("/settings", preloader_screen);
  }

  void after_load_application() override {
    ESP_LOGI("MyApp", "after_load_application called");
  }
};