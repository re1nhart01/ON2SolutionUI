
#pragma once

#include "../../components/foundation/core/application.h"
#include "../../components/foundation/core/shortcuts.h"
#include <ui/components/common_header/common_header.h>
#include <ui/components/side_bar/side_bar.h>
#include <ui/styles/common_styles.h>
#include "constants/theme.h"

namespace ON2Solutions {
  class ErrorsScreen;
  using namespace foundation;

  struct ErrorsScreenProps final
      : BaseProps<ErrorsScreen, ErrorsScreen> {};

  class ErrorsScreen final : public NavigationScreen<ErrorsScreenProps> {
   public:
    explicit ErrorsScreen(StackNavigator* stack, ErrorsScreenProps props)
        : NavigationScreen(stack, std::move(props)) {}

    ~ErrorsScreen() override = default;

    void component_did_mount() override;
    $$CommonHeader render_header() const;
    $$View render_body();
    lv_obj_t* render() override;
    ErrorsScreen* append(lv_obj_t* obj) override;
  };
}  // namespace ON2Solutions