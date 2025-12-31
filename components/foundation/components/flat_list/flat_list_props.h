#pragma once
#include "components/props.h"

namespace foundation
{
  class FlatList;
  struct FlatListProps final : BaseProps<FlatListProps, FlatList> {
    std::vector<std::shared_ptr<VNode>> children;
    short width = LV_PCT(100);
    short height = LV_PCT(100);

    FlatListProps& add_child(const std::shared_ptr<VNode>& c) {
      children.push_back(c);
      return *this;
    }

    FlatListProps& set_children(const std::vector<std::shared_ptr<VNode>>& ch) {
      children = ch;
      return *this;
    }

    FlatListProps& w(short v) { width = v; return *this; }
    FlatListProps& h(short v) { height = v; return *this; }
  };
}