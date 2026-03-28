#pragma once

#include <core/shortcuts.h>
#include "core/styling/styling.h"
#include <constants/theme.h>
#include <utility>
#include <vector>

namespace ON2Solutions {
  class PaginatedList;

  struct PaginatedListProps final : foundation::BaseProps<PaginatedListProps, PaginatedList> {
    std::vector<std::unique_ptr<VNode>> children;
    short items_per_page = 7;
    int total_items = 0;
    int current_page = 1; // Стан поточної сторінки
    Delegate<std::unique_ptr<VNode>(int)> row_renderer;
    short width = 740;
    short height = 450;

    PaginatedListProps&& set_list_items(std::vector<std::unique_ptr<VNode>> value) {
      children = std::move(value);
      return std::move(*this);
    }

    PaginatedListProps&& set_renderer(int total, Delegate<std::unique_ptr<VNode>(int)> renderer) {
      total_items = total;
      row_renderer = std::move(renderer);
      return std::move(*this);
    }

    PaginatedListProps&& set_items_per_page(short v) {
      items_per_page = v;
      return std::move(*this);
    }

    PaginatedListProps&& w(short v) {
      width = v;
      return std::move(*this);
    }

    PaginatedListProps&& h(short v) {
      height = v;
      return std::move(*this);
    }

    // ... інші сеттери (w, h)
  };

  class PaginatedList final : public Component<PaginatedListProps> {
   public:
    using Component::props;
    std::shared_ptr<Ref<View>> items_container_ref = std::make_shared<Ref<View>>("items_container");
    explicit PaginatedList(PaginatedListProps&& props)
        : Component(nullptr, nullptr, std::move(props)) {
      this->apply_reactive<PaginatedList>(this, this->props.reactive_delegates);
    };

    std::vector<std::unique_ptr<VNode>> get_current_page_items() {
      std::vector<std::unique_ptr<VNode>> page_items;

      size_t start = (props.current_page - 1) * props.items_per_page;
      size_t end = std::min(start + (size_t)props.items_per_page, props.children.size());

      return page_items;
    }

    void do_rebuild() override {
        if (this->items_container_ref->get()) {
        int total_pages = (props.total_items + props.items_per_page - 1) / props.items_per_page;

        size_t start_idx = (props.current_page - 1) * props.items_per_page;
        size_t end_idx = std::min(start_idx + (size_t)props.items_per_page, (size_t)props.total_items);

        Children rows;
        for(size_t i = start_idx; i < end_idx; ++i) {
            rows.push_back(props.row_renderer(i));
        }

        this->items_container_ref->get()->props.children = std::move(rows);
        this->items_container_ref->get()->refresh_childrens();
    }
    };

    lv_obj_t* render() override {
      int total_pages = (props.total_items + props.items_per_page - 1) / props.items_per_page;
      size_t start_idx = (props.current_page - 1) * props.items_per_page;
      size_t end_idx = std::min(start_idx + (size_t)props.items_per_page, (size_t)props.total_items);

      Children rows;
      for(size_t i = start_idx; i < end_idx; ++i) {
        rows.push_back(props.row_renderer(i));
      }

      return this->delegate($View(
          ViewProps::up()
              .w(props.width)
              .h(props.height)
              .flow(FlexPreset::ColumnCenter)
              .set_style([](Styling& style) {
                style.setBackgroundColor(lv_color_hex(0xF8F9FA)); // Світлий фон як на фото
                style.setBorderRadius(16);
                style.setPadding(12);
                style.setGap(10, 0);
              })
              .set_children(children(
                  // Контейнер для списку
                  $View(ViewProps::up()
                            .w(static_cast<short>(props.width - 40))
                            .flow(FlexPreset::ColumnStart)
                            .set_ref(items_container_ref)
                            .set_style([](Styling& style) {
                              style.setPadding(0);
                              style.setGap(8, 0);
                              style.setFlexGrow(1);
                            })
                            .set_children(std::move(rows))),

                  $View(ViewProps::up()
                          .w(static_cast<short>(props.width - 40))
                          .h(60)
                          .flow(FlexPreset::Center)
                          .set_style([](Styling& style) {
                            style.setBorder(lv_color_hex(0xEEEEEE), 1, LV_OPA_COVER);
                          })
                          .set_children(children(
                              $Pagination(
                                PaginationProps()
                                  .pages(total_pages)
                                  .active_page(props.current_page)
                                  .change([this](int page) {
                                      this->props.current_page = page;
                                      this->do_rebuild();
                                  })
                                  .set_style([](Styling& style) {
                                      style.setBackgroundColor(lv_color_hex(0xFFFFFF));
                                      style.setHeight(40);
                                  })
                              )
                          )))
              ))));
    };

    const Styling* styling() const override {
      return &this->style;
    };

    PaginatedList* append(lv_obj_t* obj) {
      lv_obj_set_parent(obj, get_component());
      return this;
    }
  };

  using $$PaginatedList = std::unique_ptr<PaginatedList>;
  inline $$PaginatedList $PaginatedList(PaginatedListProps&& args) {
    return std::make_unique<PaginatedList>(std::forward<PaginatedListProps>(args));
  }
}