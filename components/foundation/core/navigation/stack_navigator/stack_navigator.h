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

namespace foundation {

struct StackCurrentScreen {
    int id;
    std::string name;
    std::shared_ptr<VNode> instance;
};

struct StackNavigatorConfig {
    std::string initial_route;
};

using ScreenFactory = Delegate<std::shared_ptr<VNode>()>;

class StackNavigator : public std::enable_shared_from_this<StackNavigator> {
private:
    std::optional<StackCurrentScreen> current;
    lv_obj_t* parent;
    StackNavigatorConfig config;
    std::unordered_map<std::string, ScreenFactory> factories;
    std::stack<StackCurrentScreen> history;
    int id_counter = 0;

public:
    explicit StackNavigator(const StackNavigatorConfig& cfg, lv_obj_t* parent)
        : parent(parent), config(cfg) {}

    void register_screen(const std::string& name, ScreenFactory factory) {
        factories[name] = std::move(factory);
    }

    void start() {
      const std::string initial = this->config.initial_route;
      if (const auto it = factories.find(initial); it != factories.end())
        {
          const std::shared_ptr<VNode>& component =  it->second();
          current = StackCurrentScreen{
            .id = id_counter++,
            .name = initial,
            .instance = component
        };

          if (auto* s = dynamic_cast<NavigationScreenBase*>(component.get()))
            {
              s->on_focus();
            }
        }
    }

    void navigate(const std::string& name) {
        auto it = factories.find(name);
        if (it == factories.end()) return;

        if (current) {
            if (auto* ns = dynamic_cast<NavigationScreenBase*>(current->instance.get())) {
                ns->on_blur();
            }

            history.push(std::move(*current));
            current.reset();
        }

        lv_obj_t* active = parent ? parent : lv_scr_act();
        lv_obj_clean(active);

        auto screen = it->second();
        screen->set_parent(active);

        auto obj = screen->render();
        screen->set_component(obj);

        current = StackCurrentScreen {
            .id = id_counter++,
            .name = name,
            .instance = screen
        };

        if (auto* ns = dynamic_cast<NavigationScreenBase*>(screen.get())) {
            ns->on_focus();
        }
    }

    void goBack() {
        if (history.empty()) return;

        if (current) {
            if (auto* ns = dynamic_cast<NavigationScreenBase*>(current->instance.get())) {
                ns->on_blur();
            }
        }

        lv_obj_clean(parent);

        auto prev = std::move(history.top());
        history.pop();

        auto screen = prev.instance;
        auto obj = screen->render();
        screen->set_component(obj);

        current = std::move(prev);

        if (auto* ns = dynamic_cast<NavigationScreenBase*>(screen.get())) {
            ns->on_focus();
        }
    }

    const std::string& getCurrentRoute() const {
        static const std::string empty = "";
        return current ? current->name : empty;
    }

    bool hasScreen(const std::string& name) const {
        return factories.contains(name);
    }

    std::shared_ptr<VNode> getCurrentComponent() const {
        return current ? current->instance : nullptr;
    }
};

} // namespace foundation
