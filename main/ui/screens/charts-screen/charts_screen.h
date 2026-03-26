
#pragma once

#include "constants/theme.h"
#include "../../components/foundation/components/component.h"
#include "../../components/foundation/core/application.h"
#include "../../components/foundation/core/shortcuts.h"

#include <ui/components/common_header/common_header.h>

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
    void execute_status_trigger() const;
    $$CommonHeader render_header() const;
    $$View render_body() const;
    $$View render_card() const;
    $$View render_circular_wrapper(std::vector<std::unique_ptr<VNode>> childs,
                                   bool is_around) const;
    $$View render_circular_column() const;
    void navigate_after() const;
    $$View render_footer() const;
    lv_obj_t* render() override;
    ChartsScreen* append(lv_obj_t* obj) override;
  };
}  // namespace ON2Solutions