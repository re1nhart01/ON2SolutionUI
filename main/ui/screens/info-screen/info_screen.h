
#pragma once

#include "../../components/foundation/core/application.h"
#include "../../components/foundation/core/shortcuts.h"
#include <ui/components/common_header/common_header.h>
#include <ui/screens/errors-screen/errors_screen.h>
#include <protocols/uart/uart_proto.h>

namespace ON2Solutions {
  struct InfoModel;
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


    void on_focus() override;
    void component_did_mount() override;
    $$CommonHeader render_header() const;
    $$View create_status_row(int index);
    $$View render_body();
    lv_obj_t* render() override;
    InfoScreen* append(lv_obj_t* obj) override;
  };
}  // namespace ON2Solutions