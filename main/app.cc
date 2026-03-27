#include "core/navigation/stack_navigator/stack_navigator.h"

#include "ui/screens/main-screen/main_screen.h"
#include "ui/screens/pincode-screen/pincode_screen.h"
#include "ui/screens/preloader-screen/preloader_screen.h"
#include "ui/screens/settings-screen/settings_screen.h"
#include "ui/screens/charts-screen/charts_screen.h"
#include "ui/screens/errors-screen/errors_screen.h"
#include "ui/screens/info-screen/info_screen.h"


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

      navigator->register_screen("/main", [navigator](const NavigationParam& params) {
        return std::make_unique<MainScreen>(navigator.get(), MainScreenProps{.params = params});
      });

      navigator->register_screen("/pin_code", [navigator](const NavigationParam& params) {
        return std::make_unique<PinCodeScreen>(navigator.get(), PinCodeScreenProps{.params = params});
      });

      navigator->register_screen("/preloader", [navigator](const NavigationParam& _) {
        return std::make_unique<PreloaderScreen>(navigator.get(), PreloaderScreenProps{});
      });

      navigator->register_screen("/settings", [navigator](const NavigationParam& params) {
        return std::make_unique<SettingsScreen>(navigator.get(), SettingsScreenProps{.params = params});
      });

      navigator->register_screen("/errors", [navigator](const NavigationParam& params) {
        return std::make_unique<ErrorsScreen>(navigator.get(), ErrorsScreenProps{.params = params});
      });

      navigator->register_screen("/info", [navigator](const NavigationParam& params) {
        return std::make_unique<InfoScreen>(navigator.get(), InfoScreenProps{.params = params});
      });

      navigator->register_screen("/charts", [navigator](const NavigationParam& params) {
        return std::make_unique<ChartsScreen>(navigator.get(), ChartsScreenProps{.params = params});
      });

      navigator->start();
    };

    void before_load_application() override
    {
      ESP_LOGI("MyApp", "before_load_application called");
      // Setting pull-up hardware uart to erase gibberish into uart from start of main controller
      gpio_set_direction(GPIO_NUM_43, GPIO_MODE_OUTPUT);
      gpio_set_level(GPIO_NUM_43, 1);   // idle state UART
      gpio_set_pull_mode(GPIO_NUM_43, GPIO_PULLUP_ONLY);

      gpio_set_direction(GPIO_NUM_44, GPIO_MODE_INPUT);
      gpio_set_pull_mode(GPIO_NUM_44, GPIO_PULLUP_ONLY);
    }

    void after_load_application() override
    {
      ESP_LOGI("MyApp", "after_load_application called");
    }
  };
}