#pragma once

namespace foundation
{
  class Animated;
  enum AnimatedType {
    single_animation = 0xff,
    parallel_animation = 0xf1,
    chained_animation = 0xf2,
  };

  struct AnimatedProps final : BaseProps<AnimatedProps, Animated> {
    int32_t start_value = 0;
    int32_t end_value = 0;
    uint32_t duration = 500;
    uint32_t playback_duration = 0;
    uint32_t delay = 0;
    int32_t repeat_count = LV_ANIM_REPEAT_INFINITE;
    lv_anim_path_cb_t path_cb = lv_anim_path_linear;

    enum class Property {
      X, Y, Opacity, Zoom, Rotation
    } property = Property::X;

    static AnimatedProps up() { return AnimatedProps{}; }

    AnimatedProps& from(int32_t value) {
      start_value = value;
      return *this;
    }

    AnimatedProps& to(int32_t value) {
      end_value = value;
      return *this;
    }

    AnimatedProps& time(uint32_t ms) {
      duration = ms;
      return *this;
    }

    AnimatedProps& playback(uint32_t ms) {
      playback_duration = ms;
      return *this;
    }

    AnimatedProps& wait(uint32_t ms) {
      delay = ms;
      return *this;
    }

    AnimatedProps& repeat(int32_t count) {
      repeat_count = count;
      return *this;
    }

    AnimatedProps& easing(lv_anim_path_cb_t cb) {
      path_cb = cb;
      return *this;
    }

    AnimatedProps& prop(Property p) {
      property = p;
      return *this;
    }
  };

  struct AnimatedSubConfig {
    AnimatedType type_anim = AnimatedType::single_animation;
    std::vector<AnimatedProps> configs = {};

    static AnimatedSubConfig up() { return AnimatedSubConfig{}; }

    AnimatedSubConfig& type(AnimatedType t) {
      type_anim = t;
      return *this;
    }

    AnimatedSubConfig& add(const AnimatedProps& p) {
      configs.push_back(p);
      return *this;
    }

    AnimatedSubConfig& set(const std::vector<AnimatedProps>& v) {
      configs = v;
      return *this;
    }
  };
}