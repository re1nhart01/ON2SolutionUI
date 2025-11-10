#pragma once

#include "component.hh"
#include <unordered_map>
#include <string>
#include <memory>
#include <stack>

namespace foundation {

  struct StackCurrentScreen {
    int id;
    std::string name;
    std::shared_ptr<Component> screen;
  };

  struct StackNavigatorConfig {
    std::string initial_route;
  };

  class StackNavigator {
  private:
    std::shared_ptr<StackCurrentScreen> current_screen = nullptr;
    lv_obj_t* parent = nullptr;
    StackNavigatorConfig config;
    std::unordered_map<std::string, std::shared_ptr<Component>> screens;
    std::stack<StackCurrentScreen> stack;
    int id_counter = 0;

  public:
    explicit StackNavigator(const StackNavigatorConfig& config, lv_obj_t* parent);

    void registerScreen(const std::string& name, const std::shared_ptr<Component>& component);
    void start();
    void navigate(const std::string& name);
    void goBack();

    StackCurrentScreen pop();
    StackCurrentScreen push(const StackCurrentScreen& current);

    std::shared_ptr<Component> getCurrentComponent() const;
    std::string getCurrentRoute() const;
    bool hasScreen(const std::string& name) const;
  };

} // namespace foundation
