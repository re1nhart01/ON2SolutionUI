#pragma once

#include <core/style_store/style_store.h>
#include <lg/dataset/store/dataset.store.h>
#include <protocols/uart/uart_proto.h>
#include <ui/components/info_modal/info_modal.h>
#include <ui/styles/common_styles.h>
#include "../../components/foundation/core/shortcuts.h"
#include "../../components/foundation/core/state/state.h"
#include "esp_random.h"
#include "lg/dataset/deserializer.hh"

namespace ON2Solutions {
  class MainScreen;
  struct MainScreenProps final : BaseProps<MainScreenProps, MainScreen> {};

  class MainScreen final : public NavigationScreen<MainScreenProps> {
    std::unique_ptr<foundation::StyleStorage> styles;
    std::unique_ptr<UartHandler> uart_handler = nullptr;
    $$InfoModal info_modal = nullptr;
    TaskHandle_t xHandle = nullptr;
    Reactive<int> reactive_moto_lvgl;

   public:
    explicit MainScreen(StackNavigator* stack, MainScreenProps props)
        : NavigationScreen(stack, std::move(props)),
          styles(std::make_unique<StyleStorage>()),
          reactive_moto_lvgl(0) {
      style_screen_register(*this->styles);
    }

    ~MainScreen() override {
      ESP_LOGI("main_screen", "Main screen destroyed");
    };

    void on_focus() override;
    void on_blur() override;
    void show_info_modal();
    void show_errors_modal();
    void start_random_updater();
    void add_uart_data_event();

    $$View render_header();
    $$View render_footer() const;
    $$View render_body() const;
    lv_obj_t* render() override;
    const Styling* styling() const override;
    MainScreen* append(lv_obj_t* obj) override;
  };
}  // namespace ON2Solutions