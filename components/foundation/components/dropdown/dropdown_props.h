
#pragma once

#include "components/props.h"

namespace foundation {
  class Dropdown;
  struct DropdownProps final : BaseProps<DropdownProps, Dropdown> {
    std::vector<const char*> options;
    Delegate<void(const std::string)> on_change{};
    short selected = 0;

    template <typename T>
    DropdownProps&& set_options(const T& container) {
      this->options.clear();
      for (const auto& item : container) {
        if constexpr (std::is_same_v<std::decay_t<decltype(item)>,
                                     std::string_view>) {
          this->options.push_back(item.data());
        } else {
          this->options.push_back(item);
        }
      }
      return std::move(*this);
    }

    // 2. Оставляем initializer_list для удобства {"A", "B"}
    DropdownProps&& set_options(std::initializer_list<const char*> opts) {
      this->options.assign(opts.begin(), opts.end());
      return std::move(*this);
    }

    DropdownProps&& change(auto fn) {
      on_change = fn;
      return std::move(*this);
    }

    DropdownProps&& set_selected(short v) {
      selected = v;
      return std::move(*this);
    }
  };
}  // namespace foundation