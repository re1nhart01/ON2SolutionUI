#include "stack_navigator.hh"
#include "esp_log.h"

namespace foundation {

StackNavigator::StackNavigator(const StackNavigatorConfig& config, lv_obj_t* parent){
  this->config = config;
  this->parent = parent;
}

void StackNavigator::registerScreen(const std::string& name, const std::shared_ptr<Component>& component) {
    screens[name] = component;

    current_screen = std::make_shared<StackCurrentScreen>(StackCurrentScreen{
        .id = id_counter++,
        .name = name,
        .screen = component
    });
}

void StackNavigator::start() {
    if (screens.count(config.initial_route)) {
        navigate(config.initial_route);
    } else {
        // TODO: handle error case (optional)
    }
}

void StackNavigator::navigate(const std::string& name) {
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

        this->push(*current_screen);
    }
}

void StackNavigator::goBack() {
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

StackCurrentScreen StackNavigator::pop() {
    auto current = this->stack.top();
    this->stack.pop();
    return current;
}

StackCurrentScreen StackNavigator::push(const StackCurrentScreen& current) {
    this->stack.push(current);
    return current;
}

std::shared_ptr<Component> StackNavigator::getCurrentComponent() const {
    return current_screen ? current_screen->screen : nullptr;
}

std::string StackNavigator::getCurrentRoute() const {
    return current_screen ? current_screen->name : "";
}

bool StackNavigator::hasScreen(const std::string& name) const {
    return screens.find(name) != screens.end();
}

} // namespace foundation
