#pragma once

#include <protocols/uart/uart_proto.h>
#include "core/shortcuts.h"
#include "lg/helpers/utils.h"
#include "constants/theme.h"

namespace ON2Solutions {
  class PinCodeScreen;
  struct PinCodeScreenProps final
      : foundation::BaseProps<PinCodeScreenProps, PinCodeScreen> {
    foundation::NavigationParam params;
    std::shared_ptr<UartHandler> uart;
  };
  inline const char* btnm_map[] = {
    "1", "2", "3", "\n",
    "4", "5", "6", "\n",
    "7", "8", "9", "\n",
    "*", "0", "#", "\n",
    LV_SYMBOL_LEFT, "", "",
  };

  class PinCodeScreen : public foundation::NavigationScreen<PinCodeScreenProps> {
    foundation::Reactive<std::string> login_state = foundation::Reactive<std::string>("");
    std::unique_ptr<foundation::Modal> info_modal = nullptr;

   public:
    explicit PinCodeScreen(foundation::StackNavigator* stack, PinCodeScreenProps props)
        : NavigationScreen(stack, std::move(props)) {
    }

    ~PinCodeScreen() override = default;

    void component_did_mount() override;
    void show_info_modal();
    $$Button render_nav_button() const;
    void validate_and_login();
    lv_obj_t* render() override;
    const Styling* styling() const override;
    PinCodeScreen* append(lv_obj_t* obj) override;
  };
}  // namespace ON2Solutions