
#pragma once

#include "../../components/foundation/components/component.h"
#include "../../components/foundation/core/application.h"
#include "../../components/foundation/core/shortcuts.h"
#include "ui/styles/theme.h"


namespace ON2Solutions {
  class PreloaderScreen;
  using namespace foundation;

  struct PreloaderScreenProps final : BaseProps<PreloaderScreenProps, PreloaderScreen> {};

  class PreloaderScreen final : public NavigationScreen<PreloaderScreenProps> {
  public:
    explicit PreloaderScreen(StackNavigator* stack, PreloaderScreenProps props)
        : NavigationScreen(stack, std::move(props))
    {}

    ~PreloaderScreen() override = default;


    void component_did_mount() override;
    void navigate_after() const;
    lv_obj_t* render() override;
    PreloaderScreen* append(lv_obj_t* obj) override;
  };
}