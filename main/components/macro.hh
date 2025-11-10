
#pragma once

#include "button.hh"
#include "label.hh"
#include "view.hh"
#include "text_input.hh"
#include "status_bar.hh"
#include "animated.hh"
#include "image.hh"
#include "scroll_view.hh"
#include "circular_progress.hh"
#include "fragment.hh"


#ifndef FOUNDATION_ENTRYPOINT_HH
#define FOUNDATION_ENTRYPOINT_HH


#define $view_props(...)        ([]() { view_props p{}; __VA_ARGS__; return p; })()
#define $label_props(...)       ([]() { label_props p{}; __VA_ARGS__; return p; })()
#define $button_props(...)      ([]() { button_props p{}; __VA_ARGS__; return p; })()
#define $textinput_props(...)   ([]() { textinput_props p{}; __VA_ARGS__; return p; })()
#define $status_bar_props(...)   ([]() { status_bar_props p{}; __VA_ARGS__; return p; })()
#define $animated_props(...)    ([]() { animated_props p{}; __VA_ARGS__; return p; })()
#define $image_props(...)       ([]() { image_props p{}; __VA_ARGS__; return p; })()
#define $scrollview_props(...)      ([]() { scrollview_props p{}; __VA_ARGS__; return p; })()
#define $circular_props(...)    ([]() { circular_props p{}; __VA_ARGS__; return p; })()


#define $view(...)        std::make_shared<View>(__VA_ARGS__)
#define $label(...)       std::make_shared<Label>(__VA_ARGS__)
#define $button(...)      std::make_shared<Button>(__VA_ARGS__)
#define $input(...)       std::make_shared<TextInput>(__VA_ARGS__)
#define $statusbar(...)   std::make_shared<StatusBar>(__VA_ARGS__)
#define $animated(...)    std::make_shared<Animated>(__VA_ARGS__)
#define $image(...)       std::make_shared<Image>(__VA_ARGS__)
#define $scroll_view(...) std::make_shared<ScrollView>(__VA_ARGS__)
#define $circular(...)    std::make_shared<CircularProgress>(__VA_ARGS__)
#define $fragment         std::make_shared<Fragment>()


#define $s(key) this->styles->get(key)
#define $rn(fn) this->fn()

namespace foundation {
  struct entrypoint {};
}

#define entry(func) \
extern "C" void app_main() { \
ESP_LOGI("APPLICATION ENTRYPOINT", "START"); \
func(); \

#define end_entry \
}

#endif