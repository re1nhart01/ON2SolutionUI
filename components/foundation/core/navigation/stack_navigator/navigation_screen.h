//
// Created by evgeniy on 11/19/25.
//
#pragma once

namespace foundation
{
  class StackNavigator;
  class NavigationScreen
  {
  public:
    virtual ~NavigationScreen() = default;
    StackNavigator* navigation_ref = nullptr;

    explicit NavigationScreen(StackNavigator* navigation_ref)
    {
      this->navigation_ref = navigation_ref;
    }

    bool is_focused = false;

    virtual void on_focus()
    {
      is_focused = true;
      ESP_LOGI("navigation", "Focused");
    }

    virtual void on_blur()
    {
      is_focused = false;
      ESP_LOGI("navigation", "Focused");
    }

    bool get_is_focused() const {
      return is_focused;
    }

    void set_focused(const bool focused) {
      is_focused = focused;
    }

    StackNavigator* get_navigation_ref() const {
      return navigation_ref;
    }
  };
}