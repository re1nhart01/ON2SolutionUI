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

    lv_color_t btn_bg_color = lv_color_hex(0xFFFFFF);
    lv_color_t active_bg_color = lv_color_hex(0xFFFFFF);

    Delegate<void(Styling&)> btn_style_override{};
    Delegate<void(std::string value)> on_change{};
    Delegate<void(std::string value)> on_submit{};

    MatrixProps&& set_btn_map(const char** map, uint8_t pos, char* submit) {
      this->btn_map = map;
      this->submit_sym = submit;
      this->submit_pos = pos;
      return std::move(*this);
    }

    MatrixProps&& set_btn_style(Delegate<void(Styling&)> style) {
      this->btn_style_override = std::move(style);
      return std::move(*this);
    }

    MatrixProps&& set_btn_bg_color(const lv_color_t& v) {
      btn_bg_color = v;
      return std::move(*this);
    }

    MatrixProps&& set_active_btn_bg_color(const lv_color_t& v) {
      btn_bg_color = v;
      return std::move(*this);
    }

    MatrixProps&& set_btn_map(const char** map, char* backspace_sym) {
      this->btn_map = map;
      this->backspace = backspace_sym;
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