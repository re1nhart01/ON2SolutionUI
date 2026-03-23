#pragma once
#include "components/props.h"
#include "components/vnode.h"
#include "core/lv_event.h"
#include "core/structures/delegate.h"
#include "core/styling/styling.h"


namespace foundation
{
  class Matrix;
  struct MatrixProps final : BaseProps<MatrixProps, Matrix>
  {
    const char** btn_map = nullptr;
    char* submit_sym = nullptr;
    char* backspace = nullptr;
    uint8_t submit_pos = 0;

    Delegate<void(std::string value)> on_change{};
    Delegate<void(std::string value)> on_submit{};

    MatrixProps&& set_btn_map(const char** map, uint8_t pos, char* submit) {
      this->btn_map = map;
      this->submit_sym = submit;
      this->submit_pos = pos;
      return std::move(*this);
    }

    MatrixProps&& change(auto fn) {
      on_change = fn;
      return std::move(*this);
    }

    MatrixProps&& submit(auto fn) {
      on_submit = fn;
      return std::move(*this);
    }
  };
}