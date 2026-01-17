#pragma once

#include <core/style_store/style_store.h>
#include <protocols/uart/uart_proto.h>
#include <ui/components/info_modal/info_modal.h>
#include <ui/styles/common_styles.h>
#include "../../components/foundation/core/shortcuts.h"
#include "../../components/foundation/core/state/state.h"
#include "control_config.hh"

namespace ON2Solutions {
  class ChartsScreen;
  struct ChartsScreenProps final
      : BaseProps<ChartsScreenProps, ChartsScreen> {};

  class ChartsScreen : public NavigationScreen<ChartsScreenProps> {
    std::unique_ptr<StyleStorage> styles;
    std::unique_ptr<Modal> info_modal = nullptr;

  public:
    explicit ChartsScreen(StackNavigator* stack, ChartsScreenProps props)
        : NavigationScreen(stack, std::move(props)),
          styles(std::make_unique<StyleStorage>()) {
      style_screen_register(*this->styles);
    }

    ~ChartsScreen() override = default;

    void component_did_mount() override;
    lv_obj_t* render() override;
    const Styling* styling() const override;
    ChartsScreen* append(lv_obj_t* obj) override;
  };
}  // namespace ON2Solutions