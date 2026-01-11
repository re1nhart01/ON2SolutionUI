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
      : BaseProps<PinCodeScreenProps, PinCodeScreen> {};

  class PinCodeScreen : public NavigationScreen<PinCodeScreenProps> {
    std::shared_ptr<KeyboardManager> keyboard;
    State<std::string> password_state = State<std::string>("");
    State<std::string> login_state = State<std::string>("");
    std::unique_ptr<StyleStorage> styles;
    std::unique_ptr<Modal> info_modal = nullptr;

   public:
    explicit PinCodeScreen(StackNavigator* stack, PinCodeScreenProps props)
        : NavigationScreen(stack, std::move(props)),
          keyboard(std::make_shared<KeyboardManager>()),
          styles(std::make_unique<StyleStorage>()) {
      style_screen_register(*this->styles);
    }

    ~PinCodeScreen() override = default;

    void component_did_mount() override;
    void show_info_modal();
    void validate_and_login();
    lv_obj_t* render() override;
    const Styling* styling() const override;
    PinCodeScreen* append(lv_obj_t* obj) override;
  };
}  // namespace ON2Solutions