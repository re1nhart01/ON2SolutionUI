#pragma once

#include "components/component.h"
#include "core/structures/delegate.h"
#include "navigation_screen_base.h"

#include <functional>
#include <memory>
#include <optional>
#include <stack>
#include <string>
#include <unordered_map>
#include <esp_log.h>

namespace foundation {

struct StackCurrentScreen {
    int id;
    std::string name;
    std::shared_ptr<VNode> instance;
};

struct StackNavigatorConfig {
    std::string initial_route;
};

struct StackHistoryRoute
{
  int id;
  std::string name;
};

using ScreenFactory = Delegate<std::shared_ptr<VNode>()>;

class StackNavigator : public std::enable_shared_from_this<StackNavigator> {
public:
    std::optional<StackCurrentScreen> current;
    lv_obj_t* parent;
    StackNavigatorConfig config;
    std::unordered_map<std::string, ScreenFactory> factories;
    std::vector<StackHistoryRoute> history{};
    int id_counter = 0;
    int history_counter = 0;

    explicit StackNavigator(const StackNavigatorConfig& cfg, lv_obj_t* parent)
        : parent(parent), config(cfg) {}

    void register_screen(const std::string& name, ScreenFactory factory) {
        factories[name] = std::move(factory);
    }

    void start() {
      const auto initial_route = config.initial_route;
      history.clear();
      _mount_screen(initial_route, false);
    }

    void _mount_screen(const std::string& name, bool save_to_history) {
      auto self_lock = shared_from_this();

      std::string target_name = name;

      auto it = factories.find(target_name);
      if (it == factories.end()) return;

      if (current.has_value()) {
          if (auto* ns = dynamic_cast<NavigationScreenBase*>(current->instance.get())) {
              ns->on_blur();
          }

          if (save_to_history) {
              history.push_back(StackHistoryRoute{.id = history_counter++, .name = current->name});
          }
      }

      lv_obj_t* active_parent = parent ? parent : lv_scr_act();
      lv_obj_clean(active_parent);

      auto screen_instance = it->second();

      current.reset();

      current = StackCurrentScreen {
        .id = id_counter++,
        .name = target_name,
        .instance = screen_instance
    };

      screen_instance->set_parent(active_parent);
      auto ui_obj = screen_instance->render();
      screen_instance->set_component(ui_obj);

      if (auto* ns = dynamic_cast<NavigationScreenBase*>(screen_instance.get())) {
          ns->on_focus();
      }
    }

  void navigate(const std::string& name) {
      _mount_screen(name, true);
  }

  void goBack() {
      if(history.empty())
      return;

      auto prev = history.back();
      if (history.size() == 0) return;
      history.pop_back();

      _mount_screen(prev.name, false);
  }

  void reset_to(const std::string& name) {
      history.clear();
      _mount_screen(name, false);
  }

    const std::string& get_current_route() const {
        static const std::string empty = "";
        return current ? current->name : empty;
    }

    bool has_screen(const std::string& name) const {
        return factories.contains(name);
    }

    std::shared_ptr<VNode> get_current_component() const {
        return current ? current->instance : nullptr;
    }
};

} // namespace foundation
