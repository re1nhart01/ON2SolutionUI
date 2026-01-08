#pragma once

#include <string>
#include <vector>
#include "components/props.h";


  namespace foundation
  {
    class TabView;

    struct TabViewProps final : BaseProps<TabViewProps, TabView>
    {
      lv_dir_t tab_pos = LV_DIR_TOP;
      lv_coord_t header_size = 40;
      std::vector<std::string> tabs;
      std::vector<std::unique_ptr<VNode>> childrens;

      Delegate<void(uint16_t), 64> on_change_cb{};

      TabViewProps&& set_tabs(const std::vector<std::string>& t) {
        this->tabs = t;
        return std::move(*this);
      }

      TabViewProps&& set_position(lv_dir_t pos) {
        this->tab_pos = pos;
        return std::move(*this);
      }

      TabViewProps&& set_header_size(lv_coord_t size) {
        this->header_size = size;
        return std::move(*this);
      }

      TabViewProps&& on_change(Delegate<void(uint16_t), 64> cb) {
        this->on_change_cb = std::move(cb);
        return std::move(*this);
      }

      TabViewProps&& set_children(std::vector<std::unique_ptr<VNode>>&& value) {
        this->childrens = std::move(value);
        return std::move(*this);
      }
    };
  }
