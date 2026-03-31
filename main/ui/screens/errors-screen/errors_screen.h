
#pragma once

#include "../../components/foundation/core/application.h"
#include "../../components/foundation/core/shortcuts.h"
#include <ui/components/common_header/common_header.h>
#include <ui/components/side_bar/side_bar.h>
#include <ui/styles/common_styles.h>
#include "constants/theme.h"
#include "ui/components/paginated_list/paginated_list.h"

#include <protocols/uart/uart_proto.h>

namespace ON2Solutions {
  struct ErrorModel;
  class ErrorsScreen;
  using namespace foundation;

  struct ErrorsScreenProps final
      : BaseProps<ErrorsScreen, ErrorsScreen> {
    NavigationParam params;
    std::shared_ptr<UartHandler> uart;
  };

  class ErrorsScreen final : public NavigationScreen<ErrorsScreenProps> {
  private:

   public:
    explicit ErrorsScreen(StackNavigator* stack, ErrorsScreenProps props)
        : NavigationScreen(stack, std::move(props)) {}

    ~ErrorsScreen() override = default;

    void component_did_mount() override;
    $$CommonHeader render_header() const;
    $$View create_status_row(const std::string& label, bool is_error,
                             int index);

    std::vector<ErrorModel> generate_data_models(const Dataset& data) const;
    $$View render_body();
    lv_obj_t* render() override;
    ErrorsScreen* append(lv_obj_t* obj) override;
  };
}  // namespace ON2Solutions