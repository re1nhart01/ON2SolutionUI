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

  std::shared_ptr<MainScreen> main_screen;
  std::shared_ptr<SettingsScreen> settings_screen;
  std::shared_ptr<PinCodeScreen> pincode_screen;
 std::shared_ptr<PreloaderScreen> preloader_screen;

public:
  explicit WaveApplication(lv_obj_t *screen)
      : Application(screen)
  {

    this->stack_navigator = std::make_shared<
  StackNavigator>(StackNavigatorConfig{.initial_route = "/main"}, screen);

    this->main_screen = std::make_shared<MainScreen>(
        stack_navigator, MainScreenProps{});
    this->settings_screen = std::make_shared<SettingsScreen>(
        stack_navigator, SettingsScreenProps{});
    this->pincode_screen = std::make_shared<PinCodeScreen>(
        stack_navigator, PinCodeScreenProps{});
    this->preloader_screen = std::make_shared<PreloaderScreen>(
        stack_navigator, PreloaderScreenProps{});

  }

  VNode *root_component() override {
    auto root = stack_navigator->getCurrentComponent().get();
    return root;
  }

  void before_load_application() override {
    stack_navigator->register_screen("/pin_code", pincode_screen);
    stack_navigator->register_screen("/settings", settings_screen);
    stack_navigator->register_screen("/preloader", preloader_screen);
    stack_navigator->register_screen("/main", main_screen);
    // stack_navigator->start();
  }

  void after_load_application() override {
    ESP_LOGI("MyApp", "after_load_application called");
  }
};