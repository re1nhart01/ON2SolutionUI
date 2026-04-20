
#pragma once

#include "../../components/foundation/core/application.h"
#include "../../components/foundation/core/shortcuts.h"

#include <protocols/uart/uart_proto.h>
#include <ui/components/common_header/common_header.h>
#include <ui/components/on_circular/on_circular.h>


namespace ON2Solutions {
  class ChartsScreen;
  using namespace foundation;

  struct ChartsScreenProps final
      : BaseProps<ChartsScreenProps, ChartsScreen> {
    NavigationParam params;
    std::shared_ptr<UartHandler> uart;
  };

  class ChartsScreen final : public NavigationScreen<ChartsScreenProps> {
  private:
    std::shared_ptr<AnimatedControl> alarm_control = nullptr;

  public:
    explicit ChartsScreen(StackNavigator* stack, ChartsScreenProps props)
        : NavigationScreen(stack, std::move(props)), alarm_control(std::make_shared<AnimatedControl>()) {}

    ~ChartsScreen() override = default;

    void component_did_mount() override;
    void execute_status_trigger() const;
    $$CommonHeader render_header() const;
    $$View render_body() const;
    $$View render_card(const CircularSelectorType& type, const char* title,
                       uint8_t index, bool is_small) const;
    $$View render_circular_wrapper(std::vector<std::unique_ptr<VNode>> childs,
                                   bool is_around) const;
    $$View render_circular_column() const;
    void navigate_after() const;
    $$View render_footer() const;
    lv_obj_t* render() override;
    ChartsScreen* append(lv_obj_t* obj) override;
  };
}  // namespace ON2Solutions