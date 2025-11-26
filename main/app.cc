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
  std::shared_ptr<StackNavigator> stack_navigator;

public:
  explicit WaveApplication(lv_obj_t *screen)
      : Application(screen)
  {
    this->stack_navigator = std::make_shared<StackNavigator>(StackNavigatorConfig{.initial_route = "/main"}, screen);
  }

  VNode *root_component() override {
    auto root = stack_navigator->getCurrentComponent().get();
    return root;
  }

  void before_load_application() override {
    auto navigator = this->stack_navigator;

    navigator->register_screen("/main", [navigator]() {
      return std::make_shared<MainScreen>(navigator, MainScreenProps{});
    });

    navigator->register_screen("/pin_code", [navigator]() {
        return std::make_shared<PinCodeScreen>(navigator, PinCodeScreenProps{});
    });

    navigator->register_screen("/preloader", [navigator]() {
      return std::make_shared<PreloaderScreen>(navigator, PreloaderScreenProps{});
    });

    navigator->register_screen("/settings", [navigator]() {
      return std::make_shared<SettingsScreen>(navigator, SettingsScreenProps{});
    });

    navigator->start();
  }

  void after_load_application() override {
    ESP_LOGI("MyApp", "after_load_application called");
  }
};