#pragma once

#include "all.h"

using VNodePtr = std::shared_ptr<foundation::VNode>;

  inline std::shared_ptr<foundation::View> $View(auto&&... args) {
    return std::make_shared<foundation::View>(std::forward<decltype(args)>(args)...);
  }

  inline std::shared_ptr<foundation::Text> $Text(auto&&... args) {
    return std::make_shared<foundation::Text>(std::forward<decltype(args)>(args)...);
  }

  inline std::shared_ptr<foundation::Button> $Button(auto&&... args) {
    return std::make_shared<foundation::Button>(std::forward<decltype(args)>(args)...);
  }

  inline std::shared_ptr<foundation::TextInput> $Input(auto&&... args) {
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

  inline std::shared_ptr<foundation::Fragment> $Fragment() {
    return std::make_shared<foundation::Fragment>();
  }


  using Children = std::vector<std::shared_ptr<foundation::VNode>>;

  #define $s(key) this->styles->get(key)

    namespace foundation {
      struct entrypoint {};
    }

  #define entry(func) \
  extern "C" void app_main() { \
    ESP_LOGI("APPLICATION ENTRYPOINT", "START"); \
    func(); \

  #define end_entry \
  }
