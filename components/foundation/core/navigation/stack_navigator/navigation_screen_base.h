#pragma once

#include <esp_log.h>

namespace foundation
{
  class StackNavigator;

  class NavigationScreenBase
  {
  protected:
    StackNavigator* navigation_ref = nullptr;

  public:
    virtual ~NavigationScreenBase() = default;

    bool is_focused = false;

    virtual void on_focus()
    {
      is_focused = true;
      ESP_LOGI("navigation", "on_focus");
    }

    virtual void on_blur()
    {
      is_focused = false;
      ESP_LOGI("navigation", "on_blur");
    }

    bool get_is_focused() const { return is_focused; }

    void set_focused(bool focused) { is_focused = focused; }

    StackNavigator* get_navigation_ref() const { return navigation_ref; }
  };
}
