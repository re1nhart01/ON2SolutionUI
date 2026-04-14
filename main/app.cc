#include "core/navigation/stack_navigator/stack_navigator.h"

#include "ui/screens/charts-screen/charts_screen.h"
#include "ui/screens/errors-screen/errors_screen.h"
#include "ui/screens/info-screen/info_screen.h"
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
    std::shared_ptr<UartHandler> uart_handler;

   public:
    explicit WaveApplication(lv_obj_t* screen) : Application(screen) {
      this->stack_navigator = std::make_shared<foundation::StackNavigator>(
          foundation::StackNavigatorConfig{.initial_route = "/preloader"},
          screen);

      this->uart_handler = std::make_unique<UartHandler>(
          UART_NUM_2, GPIO_NUM_43, GPIO_NUM_44, 9600, 16384);
    }

    void add_uart_data_event() const {
      this->uart_handler->add_event_listener(UartTypes::UartHandlerEvent{
          .key_v = const_cast<char*>("read_data_dto"),
          .event = UART_DATA,
          .delegate = [](const UartTypes::UartCallbackResponse& uart_data) {
            if (strlen(uart_data.response.packet) <= 0)
              return;

            // ESP_LOGI("main_screen", "Received data from UART %s",
            //          uart_data.response.packet);

            heap_caps_print_heap_info(MALLOC_CAP_8BIT);

            if (uart_data.response.packet &&
                strlen(uart_data.response.packet) > 0) {
              const char* p = uart_data.response.packet;

              DatasetStore::getInstance()->set([p](const Dataset& current) {
                Dataset next = current;
                parse(&next, p, strlen(p));
                return next;
              });
            }
          }});
    }

    void on_event_bootloader_mode() const {
      auto navigation_ref = this->stack_navigator;

      DatasetStore::getInstance()->add_event_listener(
          "event_bootloader_mode",
          [navigation_ref](const std::shared_ptr<const Dataset>& dataset) {
            if (dataset->optional.bootloader_mode !=
                BootStatus::BootloaderReady)
              return;
            if (!navigation_ref)
              return;

            auto* nav =
                new std::shared_ptr<foundation::StackNavigator>(navigation_ref);

            if (lvgl_port_lock(-1)) {
              lv_async_call(
                  [](void* user_data) {
                    auto* nav = static_cast<
                        std::shared_ptr<foundation::StackNavigator>*>(
                        user_data);
                    (*nav)->navigate("/preloader");
                    delete nav;
                  },
                  nav);
              lvgl_port_unlock();
            } else {
              delete nav;
            }
          });
    }

    void on_init() override {
      auto navigator = this->stack_navigator;

      navigator->register_screen("/main", [navigator,
                                           uart = this->uart_handler](
                                              const NavigationParam& params) {
        return std::make_unique<MainScreen>(
            navigator.get(), MainScreenProps{.params = params, .uart = uart});
      });

      navigator->register_screen(
          "/pin_code", [navigator](const NavigationParam& params) {
            return std::make_unique<PinCodeScreen>(
                navigator.get(), PinCodeScreenProps{.params = params});
          });

      navigator->register_screen("/preloader",
                                 [navigator](const NavigationParam& _) {
                                   return std::make_unique<PreloaderScreen>(
                                       navigator.get(), PreloaderScreenProps{});
                                 });

      navigator->register_screen(
          "/settings", [navigator, uart = this->uart_handler](
                           const NavigationParam& params) {
            return std::make_unique<SettingsScreen>(
                navigator.get(),
                SettingsScreenProps{.params = params, .uart = uart});
          });

      navigator->register_screen("/errors", [navigator,
                                             uart = this->uart_handler](
                                                const NavigationParam& params) {
        return std::make_unique<ErrorsScreen>(
            navigator.get(), ErrorsScreenProps{.params = params, .uart = uart});
      });

      navigator->register_screen("/info", [navigator,
                                           uart = this->uart_handler](
                                              const NavigationParam& params) {
        return std::make_unique<InfoScreen>(
            navigator.get(), InfoScreenProps{.params = params, .uart = uart});
      });

      navigator->register_screen("/charts", [navigator,
                                             uart = this->uart_handler](
                                                const NavigationParam& params) {
        return std::make_unique<ChartsScreen>(
            navigator.get(), ChartsScreenProps{.params = params, .uart = uart});
      });

      navigator->start();
    };

    void before_load_application() override {
      ESP_LOGI("MyApp", "before_load_application called");
      uart_driver_delete(UART_NUM_2);
      // 1. Сначала настраиваем как выход
      gpio_config_t io_conf = {};
      io_conf.intr_type = GPIO_INTR_DISABLE;
      io_conf.mode = GPIO_MODE_OUTPUT;
      io_conf.pin_bit_mask = (1ULL << GPIO_NUM_43);
      io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
      io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
      gpio_config(&io_conf);

      gpio_set_level(GPIO_NUM_43, 0);
      vTaskDelay(pdMS_TO_TICKS(50));

      gpio_set_level(GPIO_NUM_43, 1);
      gpio_set_pull_mode(GPIO_NUM_43, GPIO_PULLUP_ONLY);
      vTaskDelay(pdMS_TO_TICKS(50));

      gpio_set_direction(GPIO_NUM_44, GPIO_MODE_INPUT);
      gpio_set_pull_mode(GPIO_NUM_44, GPIO_PULLUP_ONLY);

      this->uart_handler->init();

      uart_flush_input(UART_NUM_2);

      this->uart_handler->enable_rx(true);
      this->add_uart_data_event();
      this->on_event_bootloader_mode();
    }

    void after_load_application() override {
      ESP_LOGI("MyApp", "after_load_application called");
    }
  };
}  // namespace ON2Solutions