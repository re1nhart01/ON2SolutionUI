#include "components/application.hh"
#include "components/view.hh"

#include "components/button.hh"
#include "components/stack_navigator.hh"
#include "components/state.hh"
#include "ui/screens/main_screen.cc"
#include "ui/screens/pincode_screen.cc"
#include "ui/screens/settings_screen.cc"
#include "ui/screens/preloder_screen.cc"

extern "C" {
  #include "../../main/core/lvgl_port.h"
}

using namespace foundation;

auto label_ref_1 = std::make_shared<Ref>("LABEL_0");
auto state_1 = std::make_shared<State<int>>(0);

auto screen = lv_scr_act();

auto stack_navigator = std::make_shared<
  StackNavigator>(StackNavigatorConfig{.initial_route = "/main"}, screen);


auto main_screen = std::make_shared<MainScreen>(
    stack_navigator, main_screen_props{.ref = nullptr});
auto settings_screen = std::make_shared<SettingsScreen>(
    stack_navigator, settings_screen_props{.ref = nullptr});
auto pincode_screen = std::make_shared<PinCodeScreen>(
    stack_navigator, pincode_screen_props{.ref = nullptr});
auto preloader_screen = std::make_shared<PreloaderScreen>(
    stack_navigator, preloader_screen_props{.ref = nullptr});

class WaveApplication : public Application {
public:
  explicit WaveApplication(lv_obj_t *screen)
    : Application(screen) {
  }

  Component *root_component() override {
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