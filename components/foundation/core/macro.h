#pragma once

#ifndef FOUNDATION_ENTRYPOINT_HH
#define FOUNDATION_ENTRYPOINT_HH

#include "all.h"

#define $View(...)        std::make_shared<View>(__VA_ARGS__)
#define $Text(...)       std::make_shared<Text>(__VA_ARGS__)
#define $Button(...)      std::make_shared<Button>(__VA_ARGS__)
#define $Input(...)       std::make_shared<TextInput>(__VA_ARGS__)
#define $StatusBar(...)   std::make_shared<StatusBar>(__VA_ARGS__)
#define $Animated(...)    std::make_shared<Animated>(__VA_ARGS__)
#define $Image(...)       std::make_shared<Image>(__VA_ARGS__)
#define $ScrollView(...) std::make_shared<ScrollView>(__VA_ARGS__)
#define $Circular(...)    std::make_shared<CircularProgress>(__VA_ARGS__)
#define $Fragment         std::make_shared<Fragment>()


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

#endif