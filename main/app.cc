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

  static const char* current_packet = CH_PACKETS[0];

  class WaveApplication final : public foundation::Application {
    std::shared_ptr<foundation::StackNavigator> stack_navigator;
    std::shared_ptr<UartHandler> uart_handler;
    TaskHandle_t xHandle = nullptr;

   public:
    explicit WaveApplication(lv_obj_t* screen) : Application(screen) {
      this->stack_navigator = std::make_shared<foundation::StackNavigator>(
          foundation::StackNavigatorConfig{.initial_route = "/preloader"},
          screen);

      this->uart_handler = std::make_unique<UartHandler>(
          UART_NUM_2, GPIO_NUM_43, GPIO_NUM_44, 9600, 16384);
      // start_random_updater();
      ESP_LOGI("main_screen", "on_FOCUS");

      // this->uart_handler->stop();
      // if (xHandle != nullptr) {
      // vTaskDelete(xHandle);
      // xHandle = nullptr;
      // }
    }

    void start_random_updater() {
      xTaskCreate(
          [](void* pvParameters) {
            auto* const self = static_cast<MainScreen*>(pvParameters);
            while (true) {
              if (self) {
                current_packet = CH_PACKETS[esp_random() % 15];
                const char* p = current_packet;

                if (lvgl_port_lock(-1)) {
                  DatasetStore::getInstance()->set([&](const Dataset& current) {
                    Dataset next = current;
                    parse(&next, p, strlen(p));
                    return next;
                  });

                  lvgl_port_unlock();
                }
              }
              vTaskDelay(pdMS_TO_TICKS(1500));
            }
          },
          "rand_task", 16384, this, 5, &xHandle);
    }

    void add_uart_data_event() const {
      this->uart_handler->add_event_listener(UartTypes::UartHandlerEvent{
          .key_v = const_cast<char*>("read_data_dto"),
          .event = UART_DATA,
          .delegate = [](const UartTypes::UartCallbackResponse& uart_data) {
            if (strlen(uart_data.response.packet) <= 0)
              return;

            ESP_LOGI("main_screen", "Received data from UART %s",
                     uart_data.response.packet);

            if (uart_data.response.packet &&
                strlen(uart_data.response.packet) > 0) {
              const char* p = uart_data.response.packet;

              DatasetStore::getInstance()->set([&](const Dataset& current) {
                Dataset next = current;
                parse(&next, p, strlen(p));
                return next;
              });
            }
          }});
    }

    void on_event_bootloader_mode() const {
      auto navigation_ref = this->stack_navigator;

      DatasetStore::getInstance()->add_event_listener("event_bootloader_mode", [navigation_ref](const std::shared_ptr<const Dataset>& dataset) {
          const uint8_t current_state = dataset.get()->optional.bootloader_mode;

          if (current_state == BootStatus::BootloaderReady && navigation_ref) {
             navigation_ref->navigate("/preloader");
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
      // Setting pull-up hardware uart to erase gibberish into uart from start of main controller
      gpio_set_direction(GPIO_NUM_43, GPIO_MODE_OUTPUT);
      gpio_set_level(GPIO_NUM_43, 1);  // idle state UART
      gpio_set_pull_mode(GPIO_NUM_43, GPIO_PULLUP_ONLY);

      gpio_set_direction(GPIO_NUM_44, GPIO_MODE_INPUT);
      gpio_set_pull_mode(GPIO_NUM_44, GPIO_PULLUP_ONLY);

      this->uart_handler->init();
      this->uart_handler->enable_rx(true);
      this->add_uart_data_event();
      this->on_event_bootloader_mode();
    }

    void after_load_application() override {
      ESP_LOGI("MyApp", "after_load_application called");
    }
  };
}  // namespace ON2Solutions