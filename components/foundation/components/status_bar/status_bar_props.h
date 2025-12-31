#pragma once
#include "components/props.h";


namespace foundation
{
  class StatusBar;

  struct StatusBarProps final : BaseProps<StatusBarProps, StatusBar>
  {
    std::vector<std::shared_ptr<VNode>> children;
    std::optional<lv_color_t> background_color;
    std::optional<lv_coord_t> height;

    StatusBarProps& set_children(const std::vector<std::shared_ptr<VNode>>& value)
    {
      this->children = std::move(value);
      return *this;
    }

    StatusBarProps& set_background_color(lv_color_t color)
    {
      this->background_color = color;
      return *this;
    }

    StatusBarProps& set_height(lv_coord_t h)
    {
      this->height = h;
      return *this;
    }
  };
}