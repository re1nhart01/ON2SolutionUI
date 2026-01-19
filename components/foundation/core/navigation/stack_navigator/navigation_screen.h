#pragma once

#include "navigation_screen_base.h"
#include "components/component.h"

namespace foundation
{
  class StackNavigator;

  template<typename Props>
  class NavigationScreen
      : public virtual VNode,
        public Component<Props>,
        public NavigationScreenBase
  {
  public:
    ~NavigationScreen() override = default;

    explicit NavigationScreen(StackNavigator* navigator, Props props)
      : Component<Props>(std::move(props))
    {
      this->navigation_ref = navigator;
    }
  };
}
