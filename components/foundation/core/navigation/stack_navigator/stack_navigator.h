#pragma once

#include "components/component.h"
#include "core/structures/delegate.h"
#include "navigation_screen_base.h"

#include <core/structures/ui_queue.h>
#include <esp_log.h>
#include <functional>
#include <memory>
#include <optional>
#include <stack>
#include <string>
#include <unordered_map>

namespace foundation {
  static const std::string empty = "";
  using NavigationParam = std::unordered_map<
      std::string,
      std::variant<float, int, short, const char*, bool, std::string>>;
  using ScreenFactory =
      Delegate<std::unique_ptr<VNode>(const NavigationParam& params)>;

  struct StackCurrentScreen {
    int id;
    std::string name;
    std::unique_ptr<VNode> instance;
    NavigationParam params;
  };

  struct StackNavigatorConfig {
    std::string initial_route;
  };

  struct StackHistoryRoute {
    int id;
    std::string name;
    NavigationParam params;
  };

  template <typename T>
  T get_param(const NavigationParam& params, const std::string& key,
              T default_val = T()) {
    auto it = params.find(key);
    if (it != params.end()) {
      if (auto val = std::get_if<T>(&it->second)) {
        return *val;
      }
    }
    return default_val;
  }

  class StackNavigator {
   public:
    std::optional<StackCurrentScreen> current;
    lv_obj_t* parent;
    uint32_t last_update = 0;
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
      _mount_screen(initial_route, false, {});
    }

    void _mount_screen(const std::string& name, const bool save_to_history,
                       const NavigationParam& param) {
      UIQueue::get_instance().clear();

      const auto it = factories.find(name);
      if (it == factories.end())
        return;

      if (current.has_value()) {
        if (auto* ns =
                dynamic_cast<NavigationScreenBase*>(current->instance.get())) {
          ns->on_blur();
        }

        if (save_to_history) {
          history.push_back(StackHistoryRoute{.id = history_counter++,
                                              .name = current->name,
                                              .params = current->params});
        }
      }

      current.reset();

      lv_obj_t* active_parent = parent ? parent : lv_scr_act();
      lv_obj_clean(active_parent);

      auto screen_instance = it->second(param);

      current = StackCurrentScreen{.id = id_counter++,
                                   .name = name,
                                   .instance = std::move(screen_instance),
                                   .params = param};

      const auto screen_ref = current->instance.get();

      screen_ref->set_parent(active_parent);
      const auto ui_obj = screen_ref->render();
      screen_ref->set_component(ui_obj);

      if (auto* ns = dynamic_cast<NavigationScreenBase*>(screen_ref)) {
        ns->on_focus();
      }
    }

    void navigate(const std::string& name, bool with_save = true) {
      const uint32_t now = xTaskGetTickCount();
      constexpr uint32_t interval = pdMS_TO_TICKS(250);

      if (now - last_update < interval) {
        return;
      }

      last_update = now;

      UIQueue::get_instance().push([this, name, with_save]() {
        this->_mount_screen(name, with_save, {});
      });
    }

    void navigate(const std::string& name, const NavigationParam& params,
                  bool with_save = true) {
      const uint32_t now = xTaskGetTickCount();
      constexpr uint32_t interval = pdMS_TO_TICKS(250);

      if (now - last_update < interval) {
        return;
      }

      last_update = now;

      UIQueue::get_instance().push([this, name, params, with_save]() {
        this->_mount_screen(name, with_save, params);
      });
    }

    void goBack() {
      if (history.empty())
        return;

      const auto prev = history.back();
      if (history.size() == 0)
        return;
      history.pop_back();

      UIQueue::get_instance().push([this, prev]() {
        this->_mount_screen(prev.name, false, prev.params);
      });
    }

    void reset_to(const std::string& name) {
      history.clear();
      _mount_screen(name, false, {});
    }

    void reset_to(const std::string& name, const NavigationParam& param) {
      history.clear();
      UIQueue::get_instance().push(
          [this, name, param]() { this->_mount_screen(name, false, param); });
    }

    std::string get_current_route() const {
      if (!current.has_value())
        return "none";
      return current ? current->name : empty;
    }

    bool has_screen(const std::string& name) const {
      return factories.contains(name);
    }

    VNode* get_current_component() const {
      return current ? current->instance.get() : nullptr;
    }
  };

}  // namespace foundation
