#pragma once
#include "animated_props.h"

namespace foundation
{
  class Animated final : public Component<AnimatedProps> {
  private:
    std::unique_ptr<lv_anim_t> reference;
    std::shared_ptr<VNode> component;

  public:
    explicit Animated(const std::shared_ptr<VNode> &component, AnimatedProps props)
      : Component(std::move(props))
    {
      this->component = component;
      this->reference = std::make_unique<lv_anim_t>();

      lv_anim_init(this->reference.get());

      if (this->props.ref != nullptr) {
          this->props.ref->set(this);
      }
    };

    ~Animated() override
    {
      if (this->props.ref != nullptr) {
          this->props.ref->unlink();
      }
    };

    lv_obj_t* render() override
    {
      const auto obj = this->create_initial(this->parent);
      this->set_component(obj);

      auto* ref = this->reference.get();

      lv_anim_set_var(ref, obj);
      lv_anim_set_time(ref, this->props.duration);
      lv_anim_set_values(ref, this->props.start_value, this->props.end_value);
      lv_anim_set_playback_time(ref, this->props.playback_duration);
      lv_anim_set_repeat_count(ref, this->props.repeat_count);
      lv_anim_set_delay(ref, this->props.delay);
      lv_anim_set_path_cb(ref, this->props.path_cb);

      this->set_exec_callback();

      lv_anim_start(ref);

      return obj;
    };

    std::shared_ptr<Styling> styling() override
    {
      return {};
    };
    Animated* append(lv_obj_t* obj) override
    {
      lv_obj_set_parent(obj, get_component());
      return this;
    };

  private:
    lv_obj_t* create_initial(lv_obj_t* parental)
    {
      this->set_parent(parental);
      return this->component->render();
    };

    void set_exec_callback() {
        switch (this->props.property) {
            case AnimatedProps::Property::X:
                lv_anim_set_exec_cb(this->reference.get(), [](void* obj, int32_t v) {
                    lv_obj_set_x(static_cast<lv_obj_t*>(obj), static_cast<int16_t>(v));
                });
                break;
            case AnimatedProps::Property::Y:
                lv_anim_set_exec_cb(this->reference.get(), [](void* obj, int32_t v) {
                    lv_obj_set_y(static_cast<lv_obj_t*>(obj), static_cast<int16_t>(v));
                });
                break;
            case AnimatedProps::Property::Opacity:
                lv_anim_set_exec_cb(this->reference.get(), [](void* obj, int32_t opa) {
                    lv_obj_set_style_opa(static_cast<lv_obj_t*>(obj), opa, LV_PART_MAIN);
                });
                break;
            case AnimatedProps::Property::Zoom:
                lv_anim_set_exec_cb(this->reference.get(), [](void* obj, int32_t zoom) {
                    lv_obj_set_style_transform_zoom(static_cast<lv_obj_t*>(obj), static_cast<int16_t>(zoom), LV_PART_MAIN);
                });
                break;
            case AnimatedProps::Property::Rotation:
                lv_anim_set_exec_cb(this->reference.get(), [](void* obj, int32_t angle) {
                    lv_img_set_angle(static_cast<lv_obj_t*>(obj), static_cast<int16_t>(angle));
                });
                break;
        }
    }
  };
}