#pragma once

#include "component.hh"
#include "lvgl.h"
#include <vector>
#include <memory>

namespace foundation::animated {

  enum animation_type {
    single_animation = 0xff,
    parallel_animation = 0xf1,
    chained_animation = 0xf2,
};

  struct animated_config {
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
  };

  struct animated_sub_config {
    animation_type type_anim = animation_type::single_animation;
    std::vector<animated_config> configs = {};
  };

  class Animated : public Component {
  private:
    std::unique_ptr<lv_anim_t> reference;
    Component* component;
    animated_config* config;
    animated_sub_config* sub_config;

  public:
    explicit Animated(Component* component, animated_config* config, animated_sub_config* sub_config);
    ~Animated() override = default;

    lv_obj_t* render() override;
    std::shared_ptr<Styling> styling() override;
    Animated* append(lv_obj_t* obj) override;

  private:
    lv_obj_t* create_initial(lv_obj_t* parental);
    void make_move_x_animation() const;
    void make_move_y_animation() const;
    void make_opacity_animation() const;
    void make_zoom_animation() const;
    void make_rotation_animation() const;
  };

} // namespace foundation::animated
