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
    std::unique_ptr<VNode> instance;
};

struct StackNavigatorConfig {
    std::string initial_route;
};



using NavigationParam = std::unordered_map<std::string, std::variant<float, int, short, const char*, bool, std::string>>;
using ScreenFactory = Delegate<std::unique_ptr<VNode>(const NavigationParam& params)>;

struct StackHistoryRoute
{
    int id;
    std::string name;
    NavigationParam params;
};

template <typename T>
T get_param(const NavigationParam& params, const std::string& key, T default_val = T()) {
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

    void _mount_screen(const std::string& name, bool save_to_history, const NavigationParam& param) {
        auto it = factories.find(name);
        if (it == factories.end()) return;

        if (current.has_value()) {
                if (auto* ns = dynamic_cast<NavigationScreenBase*>(current->instance.get())) {
                        ns->on_blur();
                }

                if (save_to_history) {
                        history.push_back(StackHistoryRoute{.id = history_counter++, .name = current->name, .params = param});
                }
        }

        lv_obj_t* active_parent = parent ? parent : lv_scr_act();
        lv_obj_clean(active_parent);

        auto screen_instance = it->second(param);

        current.reset();

        current = StackCurrentScreen {
            .id = id_counter++,
            .name = name,
            .instance = std::move(screen_instance)
        };

        auto screen_ref = current->instance.get();

        screen_ref->set_parent(active_parent);
        auto ui_obj = screen_ref->render();
        screen_ref->set_component(ui_obj);

        if (auto* ns = dynamic_cast<NavigationScreenBase*>(screen_ref)) {
                ns->on_focus();
        }
    }

  void navigate(const std::string& name, bool with_save = true) {
        _mount_screen(name, with_save, {});
  }

  void navigate(const std::string& name, const NavigationParam& params, bool with_save = true) {
        _mount_screen(name, with_save, params);
  }

  void goBack() {
      if(history.empty())
      return;

      auto prev = history.back();
      if (history.size() == 0) return;
      history.pop_back();

      _mount_screen(prev.name, false, prev.params);
  }

  void reset_to(const std::string& name) {
      history.clear();
      _mount_screen(name, false, {});
  }

    void reset_to(const std::string& name, const NavigationParam& param) {
        history.clear();
        _mount_screen(name, false, param);
    }

    const std::string& get_current_route() const {
        static const std::string empty = "";
        return current ? current->name : empty;
    }

    bool has_screen(const std::string& name) const {
        return factories.contains(name);
    }

    VNode* get_current_component() const {
        return current ? current->instance.get() : nullptr;
    }
};

} // namespace foundation
