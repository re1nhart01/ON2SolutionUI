//
// Created by evgeniy on 12/21/25.
//

#pragma once

namespace foundation
{
  class Modal;
  struct ModalProps final : BaseProps<ModalProps, Modal> {
    std::shared_ptr<VNode> content;
    bool close_on_overlay = true;

    std::shared_ptr<Styling> style;

    static ModalProps up() { return ModalProps{}; }

    ModalProps& set_style(std::shared_ptr<Styling> s) {
      style = std::move(s);
      return *this;
    }

    ModalProps& set_content(std::shared_ptr<VNode> v) {
      content = v;
      return *this;
    }

    ModalProps& set_close_on_overlay(bool v) {
      close_on_overlay = v;
      return *this;
    }

  };
}