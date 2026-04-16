#pragma once

#include <core/shortcuts.h>
#include <lg/dataset/store/dataset.store.h>

#include <memory>
#include "core/styling/styling.h"

#include <constants/assets.h>
#include "constants/theme.h"

#include <lg/dataset/serializer.h>

#include "protocols/uart/uart_proto.h"

using namespace foundation;
using namespace ON2Solutions::parser;
namespace ON2Solutions {
  class Sidebar;

  struct SidebarProps final : foundation::BaseProps<SidebarProps, Sidebar> {

    StackNavigator* stack = nullptr;
    UartHandler* uart = nullptr;

    SidebarProps&& set_stack(StackNavigator* stack_nav) {
      this->stack = stack_nav;
      return std::move(*this);
    }

    SidebarProps&& set_uart_handler(UartHandler* handler) {
      this->uart = handler;
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
      if (this->props.stack == nullptr)
        return;
      if (this->props.stack->get_current_route() == path)
        return;

      this->props.stack->navigate(path, false);
    }

    $$Button render_nav_button(const std::string& path, const char* icon,
                               bool is_active_additional = false) const {
      if (this->props.stack == nullptr)
        return $Button(ButtonProps::up());
      bool is_active = this->props.stack->get_current_route() == path ||
                       is_active_additional;


      return $Button(
          ButtonProps::up()
              .click([this, path](lv_event_t*) {
                this->on_navigation_press(path);
              })
              .set_style([is_active](Styling& style) {
                style.set_clear_default();
                style.setWidth(42);
                style.setHeight(42);
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

    $$Button render_action_button() const {
      auto* uart_handle = this->props.uart;

      return $Button(ButtonProps::up()
                         .click([uart_handle](lv_event_t*) {
                           constexpr auto command =
                               SerializableCommand<const char*>{
                                   .command = SendableCommands::CabinetLight,
                               };

                           const std::string serialized = serialize(command);
                           auto _ = uart_handle->send(serialized);
                         })
                         .set_style([](Styling& style) {
                           style.set_clear_default();
                           style.setWidth(42);
                           style.setHeight(42);
                           style.setPadding(0);
                           style.setBorderRadius(8);
                           style.setBackgroundOpacity(LV_OPA_100);
                           style.setBackgroundColor(TERTIARY_BG);
                         })
                         .set_child($Image(ImageProps::up()
                                               .source(assets::Lightbulb)
                                               .width(15)
                                               .height(15))));
    }

    lv_obj_t* render() override {
      Component::render();

      return this->delegate($View(
          ViewProps::up()
              .w(56)
              .h(LV_PCT(88))
              .direction(LV_FLEX_FLOW_COLUMN)
              .items(LV_FLEX_ALIGN_CENTER)
              .track_cross(LV_FLEX_ALIGN_CENTER)
              .justify(LV_FLEX_ALIGN_SPACE_BETWEEN)
              .set_style([](Styling& style) {
                style.setBackgroundColor(PRIMARY_BG);
                style.setBorder(BORDER_SECONDARY, 1, LV_OPA_100);
                style.setBorderRadius(0);
                style.setPadding(12, 12, 0, 0);
                style.setGap(
                    0, 0);
              })
              .set_children(children(
                  $View(ViewProps::up()
                            .direction(LV_FLEX_FLOW_COLUMN)
                            .items(LV_FLEX_ALIGN_CENTER)
                            .track_cross(LV_FLEX_ALIGN_CENTER)
                            .w(LV_PCT(100))
                            .h(LV_SIZE_CONTENT)
                            .set_style([](Styling& style) {
                              style.setBackgroundOpacity(0);
                              style.setGap(4, 0);
                              NoPaddingApply(style);
                            })
                            .set_children(children(
                                this->render_nav_button(
                                    "/main", assets::Pie,
                                    this->props.stack->get_current_route() ==
                                        "/charts"),
                                this->render_nav_button("/errors",
                                                        assets::Warning),
                                this->render_nav_button("/info", assets::Info),
                                this->render_nav_button(
                                    "/pin_code", assets::Settings,
                                    this->props.stack->get_current_route() ==
                                        "/settings")))),

                  $View(ViewProps::up()
                            .direction(LV_FLEX_FLOW_COLUMN)
                            .items(LV_FLEX_ALIGN_CENTER)
                            .track_cross(LV_FLEX_ALIGN_CENTER)
                            .w(LV_PCT(100))
                            .h(LV_SIZE_CONTENT)
                            .set_style([](Styling& style) {
                              style.setBackgroundOpacity(0);
                              NoPaddingApply(style);
                            })
                            .set_children(
                                children(this->render_action_button())))))));
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