#pragma once

#include "all.h"
#include "components/activity_indicator/activity_indicator.h"

using VNodePtr = std::shared_ptr<foundation::VNode>;

using $$View = std::shared_ptr<foundation::View>;
using $$Text = std::shared_ptr<foundation::Text>;
using $$Button = std::shared_ptr<foundation::Button>;
using $$TextInput = std::shared_ptr<foundation::TextInput>;
using $$StatusBar = std::shared_ptr<foundation::StatusBar>;
using $$Animated = std::shared_ptr<foundation::Animated>;
using $$Image = std::shared_ptr<foundation::Image>;
using $$ScrollView = std::shared_ptr<foundation::ScrollView>;
using $$Circular = std::shared_ptr<foundation::CircularProgress>;
using $$Activity  = std::shared_ptr<foundation::ActivityIndicator>;
using $$Modal = std::shared_ptr<foundation::Modal>;
using $$Stepper = std::shared_ptr<foundation::Stepper>;
using $$Fragment = std::shared_ptr<foundation::Fragment>;
using $$TabView = std::shared_ptr<foundation::TabView>;
using $$FlatList = std::shared_ptr<foundation::FlatList>;


  inline std::shared_ptr<foundation::View> $View(auto&&... args) {
    return std::make_shared<foundation::View>(std::forward<decltype(args)>(args)...);
  }

  inline std::shared_ptr<foundation::Text> $Text(auto&&... args) {
    return std::make_shared<foundation::Text>(std::forward<decltype(args)>(args)...);
  }

  inline std::shared_ptr<foundation::Button> $Button(auto&&... args) {
    return std::make_shared<foundation::Button>(std::forward<decltype(args)>(args)...);
  }

  inline std::shared_ptr<foundation::TextInput> $TextInput(auto&&... args) {
    return std::make_shared<foundation::TextInput>(std::forward<decltype(args)>(args)...);
  }

  inline std::shared_ptr<foundation::StatusBar> $StatusBar(auto&&... args) {
    return std::make_shared<foundation::StatusBar>(std::forward<decltype(args)>(args)...);
  }

  inline std::shared_ptr<foundation::Animated> $Animated(auto&&... args) {
    return std::make_shared<foundation::Animated>(std::forward<decltype(args)>(args)...);
  }

  inline std::shared_ptr<foundation::Image> $Image(auto&&... args) {
    return std::make_shared<foundation::Image>(std::forward<decltype(args)>(args)...);
  }

  inline std::shared_ptr<foundation::ScrollView> $ScrollView(auto&&... args) {
    return std::make_shared<foundation::ScrollView>(std::forward<decltype(args)>(args)...);
  }

  inline std::shared_ptr<foundation::CircularProgress> $Circular(auto&&... args) {
    return std::make_shared<foundation::CircularProgress>(std::forward<decltype(args)>(args)...);
  }

  inline std::shared_ptr<foundation::ActivityIndicator> $Activity(auto&&... args) {
    return std::make_shared<foundation::ActivityIndicator>(std::forward<decltype(args)>(args)...);
  }

  inline std::shared_ptr<foundation::Modal> $Modal(auto&&... args) {
    return std::make_shared<foundation::Modal>(std::forward<decltype(args)>(args)...);
  }

  inline std::shared_ptr<foundation::Stepper> $Stepper(auto&&... args) {
    return std::make_shared<foundation::Stepper>(std::forward<decltype(args)>(args)...);
  }

  inline std::shared_ptr<foundation::Fragment> $Fragment(auto&&... args) {
    return std::make_shared<foundation::Fragment>(std::forward<decltype(args)>(args)...);
  }

  inline std::shared_ptr<foundation::TabView> $TabView(auto&&... args) {
    return std::make_shared<foundation::TabView>(std::forward<decltype(args)>(args)...);
  }

  inline std::shared_ptr<foundation::FlatList> $FlatList(auto&&... args) {
    return std::make_shared<foundation::FlatList>(std::forward<decltype(args)>(args)...);
  }

  using Children = std::vector<std::shared_ptr<foundation::VNode>>;

  #define $s(key) (this->styles->get(key))

    namespace foundation {
      struct entrypoint {};
    }

  #define entry(func) \
  extern "C" void app_main() { \
    ESP_LOGI("APPLICATION ENTRYPOINT", "START"); \
    func(); \

  #define end_entry \
  }
