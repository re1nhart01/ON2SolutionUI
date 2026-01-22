#include <cstdint>
#include <string>
#include <unordered_map>

#include "animated_control.h"

#include <components/animated/animated.h>

namespace foundation {
  void AnimatedControl::append(const std::string& name, Animated* anim) {
    if (anim == nullptr)
      return;
    elements[name] = anim;
  }

  void AnimatedControl::unlink(const std::string& name) {
    elements.erase(name);
  }

  void AnimatedControl::play(const std::string& key) {
    auto it = elements.find(key);
    if (it == elements.end()) return;

    Animated* anim = it->second;
    if (anim == nullptr || !lv_obj_is_valid(anim->get_component())) return;

    lv_async_call([](void* arg) {
      auto* a = static_cast<Animated*>(arg);
      if (a) a->play();
    }, anim);
  }

  void AnimatedControl::stop(const std::string& key) {
    auto it = elements.find(key);
    if (it == elements.end()) return;

    Animated* anim = it->second;

    if (anim == nullptr || !lv_obj_is_valid(anim->get_component())) return;

    lv_async_call([](void* arg) {
      auto* a = static_cast<Animated*>(arg);
      if (a) a->stop();
    }, anim);
  }

  void AnimatedControl::animate_to(const std::string& key, int32_t toValue,
                                   uint32_t ms) {
    auto anim = elements.find(key);
    if (anim == elements.end())
      return;

    anim->second->animateTo(toValue, ms);
  }

  int32_t AnimatedControl::get_current_value(const std::string& key) const {
    auto anim = elements.find(key);
    if (anim == elements.end())
      return 0;

    return anim->second->getCurrentValue();
  }
}  // namespace foundation
