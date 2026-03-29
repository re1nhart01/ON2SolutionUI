#pragma once
#include "components/props.h"
#include "core/structures/delegate.h"

namespace foundation
{
  class Pagination;

  struct PaginationProps final : BaseProps<PaginationProps, Pagination>
  {
    PaginationProps() = default;

    int total_pages = 5;
    int current_page = 1;
    short height = 30;
    short width = 30;
    Delegate<void(int)> on_page_changed{};
    Delegate<void(Styling&)> btn_style_override{};


    PaginationProps&& pages(const int count) {
      total_pages = count;
      return std::move(*this);
    }

    PaginationProps&& h(const short v) {
      height = v;
      return std::move(*this);
    }

    PaginationProps&& w(const short v) {
      width = v;
      return std::move(*this);
    }

    PaginationProps&& set_btn_style(Delegate<void(Styling&)> cb) {
      btn_style_override = std::move(cb);
      return std::move(*this);
    }

    PaginationProps&& active_page(int page) {
      current_page = page;
      return std::move(*this);
    }

    PaginationProps&& change(auto fn) {
      on_page_changed = fn;
      return std::move(*this);
    }
  };
}