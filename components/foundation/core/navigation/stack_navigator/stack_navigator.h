#pragma once

#include "components/component.h"
#include "navigation_screen_base.h"

#include <stack>
#include <string>
#include <unordered_map>

namespace foundation
{
  struct StackCurrentScreen {
    int id;
    std::string name;
    VNode* screen;
  };

  struct StackNavigatorConfig {
    std::string initial_route;
  };

  class StackNavigator {
  private:
    StackCurrentScreen current_screen;
    lv_obj_t* parent = nullptr;
    StackNavigatorConfig config;

    std::unordered_map<std::string, VNode*> screens;
    std::stack<StackCurrentScreen> stack;

    int id_counter = 0;

  public:
    explicit StackNavigator(const StackNavigatorConfig &config, lv_obj_t *parent) : current_screen(), parent(parent), config(config) {}

    void registerScreen(const std::string& name, VNode* component) {
      screens[name] = component;
    }

    void navigate(const std::string& name) {
      VNode* prev = current_screen.screen;

      if (auto* s = dynamic_cast<NavigationScreenBase*>(prev)) {
          s->on_blur();
      }

      auto it = screens.find(name);
      if (it == screens.end()) return;

      VNode* screen = it->second;

      lv_obj_t* active = parent ? parent : lv_scr_act();
      lv_obj_clean(active);

      screen->set_parent(active);
      screen->set_component(screen->render());

      if (prev != nullptr) {
        stack.push(current_screen);
      }

      current_screen = {
        .id = id_counter++,
        .name = name,
        .screen = screen
      };

      if (auto* s = dynamic_cast<NavigationScreenBase*>(screen)) {
          s->on_focus();
      }
    }

    void goBack() {
      if (stack.empty()) {
        ESP_LOGI("navigation", "Stack empty");
        return;
      }

      if (auto* s = dynamic_cast<NavigationScreenBase*>(current_screen.screen)) {
        s->on_blur();
      }

      StackCurrentScreen previous = stack.top();
      stack.pop();
      current_screen = previous;

      VNode* screen = previous.screen;

      lv_obj_clean(parent);
      screen->set_parent(parent);
      screen->set_component(screen->render());

      // focus previous
      if (auto* s = dynamic_cast<NavigationScreenBase*>(screen)) {
        s->on_focus();
      }
    }

    VNode* getCurrentComponent() const {
      return current_screen.screen;
    }

    bool hasScreen(const std::string& name) const {
      return screens.contains(name);
    }

    std::string getCurrentRoute() const {
      return current_screen.name;
    }
  };
}
