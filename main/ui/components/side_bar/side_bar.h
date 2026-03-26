#pragma once

#include <core/shortcuts.h>
#include <lg/dataset/store/dataset.store.h>

#include <memory>
#include "core/styling/styling.h"

#include <constants/assets.h>
#include "constants/theme.h"

using namespace foundation;
using namespace ON2Solutions::parser;
namespace ON2Solutions {
  class Sidebar;

  struct SidebarProps final : foundation::BaseProps<SidebarProps, Sidebar> {

    StackNavigator* stack = nullptr;

    SidebarProps&& set_stack(StackNavigator* stack) {
      this->stack = stack;
      return std::move(*this);
    }
  };

  class Sidebar final : public Component<SidebarProps> {
   public:
    using Component::props;

    explicit Sidebar(SidebarProps&& props)
        : Component(nullptr, nullptr, std::move(props)) {
      this->parent = nullptr;
    };

    ~Sidebar() override {
      if (this->props.ref != nullptr) {
        this->props.ref->unlink();
      }
    };

    void on_navigation_press(const std::string& path) const {
      if (this->props.stack == nullptr) return;
      if (this->props.stack->get_current_route() == path) return;

      this->props.stack->navigate(path, false);
    }

    $$Button render_nav_button(const std::string& path,
                               const char* icon) const {
      if (this->props.stack == nullptr)
        return $Button(ButtonProps::up());
      bool is_active = this->props.stack->get_current_route() == path;

      return $Button(
          ButtonProps::up()
              .click([this, path](lv_event_t*) {

                struct AsyncArg {
                  std::string path;
                };

                auto delegate = new Delegate<void()>(
                    [path, this]() {
                  this->on_navigation_press(path);
                });

                lv_async_call(
                    [](void* arg) {
                      auto* callback = static_cast<Delegate<void()>*>(arg);

                      if (callback) {
                        (*callback)();
                        delete callback;
                      }
                }, delegate);

              })
              .set_style([is_active](Styling& style) {
                style.set_clear_default();
                style.setWidth(32);
                style.setHeight(32);
                style.setPadding(0);
                style.setBorderRadius(8);
                style.setBackgroundOpacity(LV_OPA_100);
                style.setBackgroundColor(is_active ? PRIMARY_COLOR_3
                                                   : TERTIARY_BG);
              })
              .set_child(
                  $Image(ImageProps::up().source(icon).width(15).height(15)))

      );
    }

    lv_obj_t* render() override {
      Component::render();

      return this->delegate(
          $View(ViewProps::up()
                    .w(56)
                    .h(LV_PCT(100))
                    .direction(LV_FLEX_FLOW_COLUMN)
                    .items(LV_FLEX_ALIGN_CENTER)
                    .justify(LV_FLEX_ALIGN_START)
                    .track_cross(LV_FLEX_ALIGN_CENTER)
                    .set_style([](Styling& style) {
                      style.setBackgroundColor(PRIMARY_BG);
                      style.setGap(4, 0);
                      style.setBorder(BORDER_SECONDARY, 1, LV_OPA_100);
                      style.setBorderRadius(0);
                      style.setPadding(12, 12, 12, 12);
                    })
                    .set_children(children(
                        this->render_nav_button("/main", assets::Pie),
                        this->render_nav_button("/errors", assets::Warning),
                        this->render_nav_button("/info", assets::Info),
                        this->render_nav_button("/settings", assets::Settings)))));
    };

    const Styling* styling() const override {
      this->style.setTextColor(lv_color_make(255, 255, 255));
      this->style.setPadding(0, 0, 16, 16);  // Padding T R B L
      this->style.setBorder(lv_color_make(0, 0, 0), 0, 0);
      this->style.setFont(&lv_font_montserrat_12);
      this->style.setBorderRadius(0);

      return &this->style;
    };

    Sidebar* append(lv_obj_t* obj) override {
      lv_obj_set_parent(obj, get_component());
      return this;
    };
  };

  using $$Sidebar = std::unique_ptr<Sidebar>;

  inline $$Sidebar $Sidebar(SidebarProps&& args) {
    return std::make_unique<Sidebar>(std::forward<SidebarProps>(args));
  }
}  // namespace ON2Solutions