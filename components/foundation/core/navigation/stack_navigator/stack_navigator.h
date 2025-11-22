#pragma once

#include "components/component.h"
#include "navigation_screen.h"

#include <stack>
#include <string>

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
    explicit StackNavigator(const StackNavigatorConfig &config,
                            lv_obj_t *parent)
        : current_screen(), parent(parent), config(config)
    {}

    void registerScreen(const std::string& name, VNode* component) {
      screens[name] = component;

      current_screen = StackCurrentScreen{
        .id = id_counter++,
        .name = name,
        .screen = component
    };
    }

    void navigate(const std::string& name) {
      auto* prev = current_screen.screen;

      if (auto s = dynamic_cast<NavigationScreen*>(prev)) {
          s->on_blur();
      }

      auto it = screens.find(name);
      if (it != screens.end()) {
          VNode* screen = it->second;

          lv_obj_t* active = parent ? parent : lv_scr_act();
          lv_obj_clean(active);

          screen->set_parent(active);
          screen->set_component(screen->render());

          current_screen = StackCurrentScreen{
            .id = id_counter++,
            .name = name,
            .screen = screen
        };

          if (auto s = dynamic_cast<NavigationScreen*>(screen)) {
              s->on_focus();
          }

          stack.push(current_screen);
      }
    }

    void goBack() {
      if (!stack.empty()) {
          StackCurrentScreen previous = stack.top();
          if (const auto s = dynamic_cast<NavigationScreen*>(previous.screen)) {
              s->on_blur();
          }
          this->pop();


          this->current_screen = previous;

          const auto screen = previous.screen;
          lv_obj_t* active_screen = this->parent;
          lv_obj_clean(active_screen);

          screen->set_parent(active_screen);

          lv_obj_t* obj = screen->render();
          screen->set_component(obj);

          if (const auto s = dynamic_cast<NavigationScreen*>(previous.screen)) {
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