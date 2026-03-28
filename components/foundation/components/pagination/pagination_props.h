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
    Delegate<void(int)> on_page_changed{};

    // Fluent API
    PaginationProps&& pages(int count) {
      total_pages = count;
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