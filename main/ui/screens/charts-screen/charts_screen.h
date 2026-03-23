
#pragma once

#include "constants/theme.h"
#include "../../components/foundation/components/component.h"
#include "../../components/foundation/core/application.h"
#include "../../components/foundation/core/shortcuts.h"

namespace ON2Solutions {
  class ChartsScreen;
  using namespace foundation;

  struct ChartsScreenProps final
      : BaseProps<ChartsScreenProps, ChartsScreen> {};

  class ChartsScreen final : public NavigationScreen<ChartsScreenProps> {
   public:
    explicit ChartsScreen(StackNavigator* stack, ChartsScreenProps props)
        : NavigationScreen(stack, std::move(props)) {}

    ~ChartsScreen() override = default;

    void component_did_mount() override;
    void navigate_after() const;
    lv_obj_t* render() override;
    ChartsScreen* append(lv_obj_t* obj) override;
  };
}  // namespace ON2Solutions