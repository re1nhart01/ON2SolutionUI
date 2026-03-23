#pragma once

#include <core/style_store/style_store.h>
#include <lg/dataset/store/dataset.store.h>
#include <protocols/uart/uart_proto.h>
#include <ui/components/info_modal/info_modal.h>
#include "ui/components/errors_modal/error_modal.h"
#include <ui/components/dot_indicator/dot_indicator.h>
#include <ui/styles/common_styles.h>
#include "../../components/foundation/core/shortcuts.h"
#include "../../components/foundation/core/state/state.h"
#include "esp_random.h"
#include "lg/dataset/deserializer.h"
#include "internals/lvgl_port.h"
#include "ui/components/common_header/common_header.h"
#include "ui/components/side_bar/side_bar.h"

namespace ON2Solutions {
  class MainScreen;
  struct MainScreenProps final : BaseProps<MainScreenProps, MainScreen> {};

  class MainScreen final : public NavigationScreen<MainScreenProps> {
    std::unique_ptr<UartHandler> uart_handler = nullptr;
    $$InfoModal info_modal = nullptr;
    $$ErrorModal error_modal = nullptr;
    TaskHandle_t xHandle = nullptr;
    std::shared_ptr<AnimatedControl> alarm_control = nullptr;
   public:
    explicit MainScreen(StackNavigator* stack, MainScreenProps props)
        : NavigationScreen(stack, std::move(props)), alarm_control(std::make_shared<AnimatedControl>()) {
    }

    ~MainScreen() override {
      ESP_LOGI("main_screen", "Main screen destroyed");
    };

    void on_focus() override;
    void on_blur() override;
    void show_info_modal();
    void show_errors_modal();
    void start_random_updater();
    void add_uart_data_event() const;
    void execute_status_trigger() const;
    template<typename C>
    void update_styles(Component<C>* component, const Delegate<void(Styling&)>& style);

    $$CommonHeader render_header() const;
    $$View render_footer() const;
    $$View render_body() const;
    $$View render_body_left() const;
    $$Animated render_animated_alarm() const;
    lv_obj_t* render() override;
    const Styling* styling() const override;
    MainScreen* append(lv_obj_t* obj) override;
  };
}  // namespace ON2Solutions