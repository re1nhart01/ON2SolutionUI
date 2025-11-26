#pragma once

#include "components/component.h"
#include "navigation_screen_base.h"

#include <memory>
#include <stack>
#include <string>

namespace foundation
{
  struct StackCurrentScreen {
    int id;
    std::string name;
    std::shared_ptr<VNode> screen;
  };

  struct StackNavigatorConfig {
    std::string initial_route;
  };

  class StackNavigator {
  private:
    std::shared_ptr<StackCurrentScreen> current_screen = nullptr;
    lv_obj_t* parent = nullptr;
    StackNavigatorConfig config;
    std::unordered_map<std::string, std::shared_ptr<VNode>> screens;
    std::stack<StackCurrentScreen> stack;
    int id_counter = 0;

  public:
    explicit StackNavigator(const StackNavigatorConfig& config, lv_obj_t* parent){
      this->config = config;
      this->parent = parent;
  };

  void register_screen(const std::string& name, const std::shared_ptr<VNode>& component) {
    screens[name] = component;

    current_screen = std::make_shared<StackCurrentScreen>(StackCurrentScreen{
        .id = id_counter++,
        .name = name,
        .screen = component
    });
  }

  void start() {
    const std::string initial = this->config.initial_route;
    if (const auto it = screens.find(initial); it != screens.end())
    {
      const std::shared_ptr<VNode>& component =  it->second;
      current_screen = std::make_shared<StackCurrentScreen>(StackCurrentScreen{
        .id = id_counter++,
        .name = initial,
        .screen = component
    });

    if (auto* s = dynamic_cast<NavigationScreenBase*>(component.get()))
    {
      s->on_focus();
    }
  }
  }

  void navigate(const std::string& name) {
    if (const auto it = screens.find(name); it != screens.end()) {
        const auto screen = it->second;
          lv_obj_t* active_screen = this->parent != nullptr ? this->parent : lv_scr_act();

        auto prev = this->current_screen->screen;

        if (auto* s = dynamic_cast<NavigationScreenBase*>(prev.get()))
        {
          s->on_blur();
        }

          lv_obj_clean(active_screen);

          screen->set_parent(active_screen);

          current_screen = std::make_shared<StackCurrentScreen>(StackCurrentScreen{
             .id = ++id_counter,
             .name = name,
             .screen = screen
          });

          lv_obj_t* obj = screen->render();
          screen->set_component(obj);

          if (auto* s = dynamic_cast<NavigationScreenBase*>(screen.get()))
          {
            s->on_focus();
          }

          this->push(*current_screen);
      }
  }

  void goBack() {
      if (!stack.empty()) {
          if (auto* s = dynamic_cast<NavigationScreenBase*>(current_screen->screen.get()))
          {
            s->on_blur();
          }

          StackCurrentScreen previous = stack.top();
          this->pop();


          auto screen = previous.screen;
          lv_obj_t* active_screen = this->parent;
          lv_obj_clean(active_screen);

          screen->set_parent(active_screen);

          lv_obj_t* obj = screen->render();
          screen->set_component(obj);

          this->current_screen = std::make_shared<StackCurrentScreen>(previous);
          if (auto* s = dynamic_cast<NavigationScreenBase*>(current_screen->screen.get()))
          {
            s->on_focus();
          }

          // screen->on_mount();
      } else {
          ESP_LOGI("NO SCREENS IN STACK NAVIGATION", "");
      }
  }

  StackCurrentScreen pop() {
      auto current = this->stack.top();
      this->stack.pop();
      return current;
  }

  StackCurrentScreen push(const StackCurrentScreen& current) {
      this->stack.push(current);
      return current;
  }

  std::shared_ptr<VNode> getCurrentComponent() const {
      return current_screen ? current_screen->screen : nullptr;
  }

  std::string getCurrentRoute() const {
      return current_screen ? current_screen->name : "";
  }

  bool hasScreen(const std::string& name) const {
      return screens.contains(name);
  }
  };

}