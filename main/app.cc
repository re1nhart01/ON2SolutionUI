#include "core/navigation/stack_navigator/stack_navigator.h"
#include "ui/screens/main-screen/main_screen.h"
#include "ui/screens/pincode-screen/pincode_screen.h"
#include "ui/screens/preloader-screen/preloader_screen.h"
#include "ui/screens/settings-screen/settings_screen.h"

#include <core/application.h>

extern "C" {
#include "../components/foundation/internals/lvgl_port.h"
}

namespace ON2Solutions {
  class WaveApplication final : public foundation::Application {
    std::shared_ptr<foundation::StackNavigator> stack_navigator;

  public:
    explicit WaveApplication(lv_obj_t* screen) : Application(screen)
    {
      this->stack_navigator = std::make_shared<foundation::StackNavigator>(
          foundation::StackNavigatorConfig{.initial_route = "/preloader"},
          screen);
    }

    void on_init() override
    {
      auto navigator = this->stack_navigator;

      navigator->register_screen("/main", [navigator]() {
        return std::make_unique<MainScreen>(navigator.get(), MainScreenProps{});
      });

      navigator->register_screen("/pin_code", [navigator]() {
        return std::make_unique<PinCodeScreen>(navigator.get(), PinCodeScreenProps{});
      });

      navigator->register_screen("/preloader", [navigator]() {
        return std::make_unique<PreloaderScreen>(navigator.get(), PreloaderScreenProps{});
      });

      navigator->register_screen("/settings", [navigator]() {
        return std::make_unique<SettingsScreen>(navigator.get(), SettingsScreenProps{});
      });

      navigator->start();
    };

    void before_load_application() override
    {
      ESP_LOGI("MyApp", "before_load_application called");
    }

    void after_load_application() override
    {
      ESP_LOGI("MyApp", "after_load_application called");
    }
  };
}