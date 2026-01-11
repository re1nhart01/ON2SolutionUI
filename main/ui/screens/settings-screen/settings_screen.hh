#pragma once

#include "../../components/foundation/components/component.h"
#include "../../components/foundation/core/shortcuts.h"

#include "esp_log.h"
#include "ui/localization.hh"
#include "ui/styles/common_styles.h"

namespace ON2Solutions {
  class SettingsScreen;

  using namespace foundation;

  struct SettingsScreenProps final : BaseProps<SettingsScreenProps, SettingsScreen> {};

  class SettingsScreen final : public NavigationScreen<SettingsScreenProps> {
  private:
    std::unique_ptr<StyleStorage> styles;

  public:
    explicit SettingsScreen(StackNavigator* stack, SettingsScreenProps props)
        : NavigationScreen(stack, std::move(props)), styles(std::make_unique<StyleStorage>())
    {
      style_screen_register(*this->styles);
    }

    void component_did_mount() override;
    $$Stepper make_param(const char* label, float val, float step = 1.0f, short precis = 0) const;
    $$View render_sensors_tab() const;
    $$View render_timers_tab() const;
    $$View render_limits_tab() const;
    $$View render_service_tab() const;
    lv_obj_t* render() override;
    const Styling* styling() const override;
    SettingsScreen* append(lv_obj_t* obj) override;
  };
}