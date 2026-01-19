#pragma once
#include "components/props.h"

namespace foundation
{
  class FlatList;
  struct FlatListProps final : BaseProps<FlatListProps, FlatList> {
    std::vector<std::unique_ptr<VNode>> children;
    short width = LV_PCT(100);
    short height = LV_PCT(100);

    FlatListProps&& add_child(std::unique_ptr<VNode> c) {
      children.push_back(std::move(c));
      return std::move(*this);
    }

    FlatListProps&& set_children(std::vector<std::unique_ptr<VNode>>&& ch) {
      children = std::move(ch);
      return std::move(*this);
    }

    FlatListProps&& w(short v) { width = v; return std::move(*this); }
    FlatListProps&& h(short v) { height = v; return std::move(*this); }
  };
}