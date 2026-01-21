#pragma once

#include "components/props.h";
#include "components/vnode.h"
#include <memory>

#include <core/animated/animated_control.h>

namespace foundation
{
  class Animated;

  struct AnimatedProps final : BaseProps<AnimatedProps, Animated> {
    std::unique_ptr<VNode> child = nullptr;
    std::pair<std::string, std::shared_ptr<AnimatedControl>> control;
    int32_t start_value = 0;
    int32_t end_value = 0;
    int32_t stop_value = 0;
    uint32_t duration = 500;
    uint32_t playback_duration = 0;
    uint32_t delay = 0;
    int32_t repeat_count = LV_ANIM_REPEAT_INFINITE;
    lv_anim_path_cb_t path_cb = lv_anim_path_linear;
    bool auto_start = true;

    enum class Property {
      X, Y, Opacity, Zoom, Rotation
    } property = Property::X;


    AnimatedProps&& set_auto_start(bool v) { auto_start = v; return std::move(*this); }

    AnimatedProps&& from(int32_t value) {
      start_value = value;
      return std::move(*this);
    }

    AnimatedProps&& to(int32_t value) {
      end_value = value;
      return std::move(*this);
    }

    AnimatedProps&& stop(int32_t value) {
      stop_value = value;
      return std::move(*this);
    }


    AnimatedProps&& on(std::unique_ptr<VNode> value) {
      child = std::move(value);
      return std::move(*this);
    }

    AnimatedProps&& set_control(std::string name, std::shared_ptr<AnimatedControl> value) {
      control = { name, value };
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