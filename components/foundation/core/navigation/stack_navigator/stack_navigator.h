#pragma once

#include "components/component.h"
#include "navigation_screen.h"

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

  void registerScreen(const std::string& name, const std::shared_ptr<VNode>& component) {
    screens[name] = component;

    current_screen = std::make_shared<StackCurrentScreen>(StackCurrentScreen{
        .id = id_counter++,
        .name = name,
        .screen = component
    });
  }

  void start() {
      if (screens.contains(config.initial_route)) {
          navigate(config.initial_route);
      } else {
          // TODO: handle error case (optional)
      }
  }

  void navigate(const std::string& name) {
      auto screen_before = this->current_screen->screen;

      if (const auto s = dynamic_cast<NavigationScreen*>(screen_before.get())) {
          s->on_blur();
      }

      auto it = screens.find(name);
      if (it != screens.end()) {
          auto screen = it->second;
          lv_obj_t* active_screen = this->parent != nullptr ? this->parent : lv_scr_act();

          lv_obj_clean(active_screen);

          screen->set_parent(active_screen);

          lv_obj_t* obj = screen->render();
          screen->set_component(obj);

          // screen->component_did_mount();

          current_screen = std::make_shared<StackCurrentScreen>(StackCurrentScreen{
              .id = ++id_counter,
              .name = name,
              .screen = screen
          });

          if (const auto s = dynamic_cast<NavigationScreen*>(screen.get())) {
              s->on_focus();
          }

          this->push(*current_screen);
      }
  }

  void goBack() {
      if (!stack.empty()) {
          StackCurrentScreen previous = stack.top();
          this->pop();

          this->current_screen = std::make_shared<StackCurrentScreen>(previous);

          auto screen = previous.screen;
          lv_obj_t* active_screen = this->parent;
          lv_obj_clean(active_screen);

          screen->set_parent(active_screen);

          lv_obj_t* obj = screen->render();
          screen->set_component(obj);

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