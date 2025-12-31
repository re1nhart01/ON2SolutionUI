//
// Created by evgeniy on 12/21/25.
//

#pragma once
#include "components/props.h";

namespace foundation
{
  class Modal;
  struct ModalProps final : BaseProps<ModalProps, Modal> {
    std::shared_ptr<VNode> content;
    bool close_on_overlay = true;

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