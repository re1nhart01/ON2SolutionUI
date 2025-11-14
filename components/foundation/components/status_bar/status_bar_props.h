#pragma once


namespace foundation
{
  struct StatusBarProps final : BaseProps<StatusBarProps>
  {
    static StatusBarProps up() { return StatusBarProps{}; }
  };
}