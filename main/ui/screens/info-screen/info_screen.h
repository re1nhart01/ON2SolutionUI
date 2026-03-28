
#pragma once

#include "constants/theme.h"
#include "../../components/foundation/components/component.h"
#include "../../components/foundation/core/application.h"
#include "../../components/foundation/core/shortcuts.h"
#include <ui/components/common_header/common_header.h>
#include <ui/components/side_bar/side_bar.h>
#include <ui/screens/errors-screen/errors_screen.h>
#include <ui/styles/common_styles.h>
#include "constants/theme.h"


#include <ui/components/common_header/common_header.h>

namespace ON2Solutions {
  class InfoScreen;
  using namespace foundation;

  struct InfoScreenProps final
      : BaseProps<InfoScreenProps, InfoScreen> {
    NavigationParam params;
    std::shared_ptr<UartHandler> uart;
  };

  class InfoScreen final : public NavigationScreen<InfoScreenProps> {
   public:
    explicit InfoScreen(StackNavigator* stack, InfoScreenProps props)
        : NavigationScreen(stack, std::move(props)) {}

    ~InfoScreen() override = default;

    void component_did_mount() override;
    $$CommonHeader render_header() const;
    std::vector<ErrorModel> generate_test_data_models();
    $$View create_status_row(const std::string& label, bool is_error);
    $$View render_body();
    lv_obj_t* render() override;
    InfoScreen* append(lv_obj_t* obj) override;
  };
}  // namespace ON2Solutions