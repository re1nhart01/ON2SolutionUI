#pragma once


namespace foundation
{
  class StatusBar;
  struct StatusBarProps final : BaseProps<StatusBarProps, StatusBar>
  {
    static StatusBarProps up() { return StatusBarProps{}; }
  };
}