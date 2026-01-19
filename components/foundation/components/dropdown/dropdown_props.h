
#pragma once

#include "components/props.h"

namespace foundation
{
  class Dropdown;
  struct DropdownProps final : BaseProps<DropdownProps, Dropdown>
  {
    std::vector<const char*> options;
    Delegate<void(const char*)> on_change{};


    DropdownProps&& set_options(std::vector<const char*> options) {
      this->options = std::move(options);
      return std::move(*this);
    }

    DropdownProps&& change(auto fn) {
      on_change = fn;
      return std::move(*this);
    }

  };
}