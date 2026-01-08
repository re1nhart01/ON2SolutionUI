#pragma once

#include "all.h"
#include "components/activity_indicator/activity_indicator.h"

using VNodePtr = std::unique_ptr<foundation::VNode>;

using $$View = std::unique_ptr<foundation::View>;
using $$Text = std::unique_ptr<foundation::Text>;
using $$Button = std::unique_ptr<foundation::Button>;
using $$TextInput = std::unique_ptr<foundation::TextInput>;
using $$StatusBar = std::unique_ptr<foundation::StatusBar>;
using $$Animated = std::unique_ptr<foundation::Animated>;
using $$Image = std::unique_ptr<foundation::Image>;
using $$ScrollView = std::unique_ptr<foundation::ScrollView>;
using $$Circular = std::unique_ptr<foundation::CircularProgress>;
using $$Activity  = std::unique_ptr<foundation::ActivityIndicator>;
using $$Modal = std::unique_ptr<foundation::Modal>;
using $$Stepper = std::unique_ptr<foundation::Stepper>;
using $$Fragment = std::unique_ptr<foundation::Fragment>;
using $$TabView = std::unique_ptr<foundation::TabView>;
using $$FlatList = std::unique_ptr<foundation::FlatList>;


  inline std::unique_ptr<foundation::View> $View(foundation::ViewProps&& args) {
    return std::make_unique<foundation::View>(std::move(args));
  }

  inline std::unique_ptr<foundation::Text> $Text(foundation::TextProps args) {
    return std::make_unique<foundation::Text>(std::move(args));
  }

  inline std::unique_ptr<foundation::Button> $Button(foundation::ButtonProps args) {
    return std::make_unique<foundation::Button>(std::move(args));
  }

  inline std::unique_ptr<foundation::TextInput> $TextInput(foundation::TextInputProps args) {
    return std::make_unique<foundation::TextInput>(std::move(args));
  }

  inline std::unique_ptr<foundation::StatusBar> $StatusBar(foundation::StatusBarProps args) {
    return std::make_unique<foundation::StatusBar>(std::move(args));
  }

  inline std::unique_ptr<foundation::Animated> $Animated(foundation::AnimatedProps args) {
    return std::make_unique<foundation::Animated>(std::move(args));
  }

  inline std::unique_ptr<foundation::Image> $Image(foundation::ImageProps args) {
    return std::make_unique<foundation::Image>(std::move(args));
  }

  inline std::unique_ptr<foundation::ScrollView> $ScrollView(foundation::ScrollViewProps args) {
    return std::make_unique<foundation::ScrollView>(std::move(args));
  }

  inline std::unique_ptr<foundation::CircularProgress> $Circular(foundation::CircularProgressProps args) {
    return std::make_unique<foundation::CircularProgress>(std::move(args));
  }

  inline std::unique_ptr<foundation::ActivityIndicator> $Activity(foundation::ActivityIndicatorProps args) {
    return std::make_unique<foundation::ActivityIndicator>(std::move(args));
  }

  inline std::unique_ptr<foundation::Modal> $Modal(foundation::ModalProps args) {
    return std::make_unique<foundation::Modal>(std::move(args));
  }

  inline std::unique_ptr<foundation::Stepper> $Stepper(foundation::StepperProps args) {
    return std::make_unique<foundation::Stepper>(std::move(args));
  }

  inline std::unique_ptr<foundation::Fragment> $Fragment(foundation::FragmentProps args) {
    return std::make_unique<foundation::Fragment>(std::move(args));
  }

  inline std::unique_ptr<foundation::TabView> $TabView(foundation::TabViewProps args) {
    return std::make_unique<foundation::TabView>(std::move(args));
  }

  inline std::unique_ptr<foundation::FlatList> $FlatList(foundation::FlatListProps args) {
    return std::make_unique<foundation::FlatList>(std::move(args));
  }

  using Children = std::vector<std::unique_ptr<foundation::VNode>>;

  template <typename... Nodes>
  std::vector<std::unique_ptr<foundation::VNode>> children(Nodes&&... nodes)
  {
    std::vector<std::unique_ptr<foundation::VNode>> v;
    v.reserve(sizeof...(Nodes));
    (v.push_back(std::move(nodes)), ...);
    return v;
  }



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
