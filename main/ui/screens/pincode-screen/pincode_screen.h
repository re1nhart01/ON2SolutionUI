#pragma once

#include <core/style_store/style_store.h>
#include <protocols/uart/uart_proto.h>
#include <ui/components/info_modal/info_modal.h>
#include <ui/styles/common_styles.h>
#include "../../components/foundation/core/shortcuts.h"
#include "../../components/foundation/core/state/state.h"
#include "control_config.hh"

namespace ON2Solutions {
  class PinCodeScreen;
  struct PinCodeScreenProps final
      : BaseProps<PinCodeScreenProps, PinCodeScreen> {
    NavigationParam params;
  };
  inline const char* btnm_map[] = {
    "1", "2", "3", "\n",
    "4", "5", "6", "\n",
    "7", "8", "9", "\n",
    "*", "0", "#", "\n",
    LV_SYMBOL_LEFT, "", "",
  };

  class PinCodeScreen : public NavigationScreen<PinCodeScreenProps> {
    std::shared_ptr<KeyboardManager> keyboard;
    Reactive<std::string> login_state = Reactive<std::string>("");
    std::unique_ptr<Modal> info_modal = nullptr;

   public:
    explicit PinCodeScreen(StackNavigator* stack, PinCodeScreenProps props)
        : NavigationScreen(stack, std::move(props)),
          keyboard(std::make_shared<KeyboardManager>()) {}

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