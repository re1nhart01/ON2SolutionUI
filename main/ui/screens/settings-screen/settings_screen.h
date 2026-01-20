#pragma once

#include "../../components/foundation/core/shortcuts.h"

#include <lg/dataset/paramspec.h>
#include <protocols/uart/uart_proto.h>
#include <ui/localization.h>
#include <ui/styles/common_styles.h>

namespace ON2Solutions {

  using namespace ON2Solutions::parser::paramspec;

  class SettingsScreen;

  using namespace foundation;

  struct SettingsScreenProps final
      : BaseProps<SettingsScreenProps, SettingsScreen> {};

  class SettingsScreen final : public NavigationScreen<SettingsScreenProps> {
   private:
    std::unique_ptr<UartHandler> uart_handler = nullptr;

   public:
    explicit SettingsScreen(StackNavigator* stack, SettingsScreenProps props)
        : NavigationScreen(stack, std::move(props)) {}

    void on_focus() override;
    void on_blur() override;
    template <typename T>
    void update_param(const ParamSpec& spec, T value, T min, T max) const;
    void update_param(const ParamSpec& spec, const char* option) const;
    void hour_run_reset() const;

    template <typename T>
    $$Stepper make_param(const ParamSpec& spec, T value, float dependable = 0,
                         short width = 165) const;
    $$View render_sensors_tab() const;
    $$View render_timers_tab() const;
    $$View render_limits_tab() const;
    $$View render_service_tab() const;
    $$View render_section_header(const std::string& heading) const;
    lv_obj_t* render() override;
    const Styling* styling() const override;
    SettingsScreen* append(lv_obj_t* obj) override;
  };
}  // namespace ON2Solutions