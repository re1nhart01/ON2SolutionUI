//
// Created by evgeniy on 12/21/25.
//

#pragma once
#include "components/props.h";

namespace foundation
{
  class Modal;
  struct ModalProps final : BaseProps<ModalProps, Modal> {
    std::unique_ptr<VNode> content;
    bool close_on_overlay = true;

    ModalProps&& set_content(std::unique_ptr<VNode> v) {
      content = std::move(v);
      return std::move(*this);
    }

    ModalProps&& set_close_on_overlay(bool v) {
      close_on_overlay = v;
      return std::move(*this);
    }

  };
}