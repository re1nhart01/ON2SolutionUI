#pragma once

#include "lvgl.h"
#include "components/props.h";

namespace foundation
{
  class Animated;

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

    AnimatedProps&& from(int32_t value) {
      start_value = value;
      return std::move(*this);
    }

    AnimatedProps&& to(int32_t value) {
      end_value = value;
      return std::move(*this);
    }

    AnimatedProps&& time(uint32_t ms) {
      duration = ms;
      return std::move(*this);
    }

    AnimatedProps&& playback(uint32_t ms) {
      playback_duration = ms;
      return std::move(*this);
    }

    AnimatedProps&& wait(uint32_t ms) {
      delay = ms;
      return std::move(*this);
    }

    AnimatedProps&& repeat(int32_t count) {
      repeat_count = count;
      return std::move(*this);
    }

    AnimatedProps&& easing(lv_anim_path_cb_t cb) {
      path_cb = cb;
      return std::move(*this);
    }

    AnimatedProps&& prop(Property p) {
      property = p;
      return std::move(*this);
    }
  };
}