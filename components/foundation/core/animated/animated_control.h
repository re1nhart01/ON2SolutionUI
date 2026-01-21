

#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>

namespace foundation {

  class Animated;  // forward declaration

  class AnimatedControl {
   private:
    std::unordered_map<std::string, Animated*> elements;

   public:
    AnimatedControl() = default;
    ~AnimatedControl() = default;

    void append(const std::string& name, Animated* anim);
    void unlink(const std::string& name);
    void play(const std::string& key);
    void stop(const std::string& key);
    void animate_to(const std::string& key, int32_t toValue, uint32_t ms);
    int32_t get_current_value(const std::string& key) const;
  };

}  // namespace foundation
