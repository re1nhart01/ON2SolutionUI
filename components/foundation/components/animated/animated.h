#pragma once

#include "animated_props.h"
#include "components/component.h"

namespace foundation {

  class Animated final : public Component<AnimatedProps> {
   private:
    std::unique_ptr<lv_anim_t> anim;
    bool is_running = false;

   public:
    explicit Animated(AnimatedProps&& props)
        : Component(nullptr, nullptr, std::move(props)) {
      this->apply_reactive<Animated>(this, this->props.reactive_delegates);

      anim = std::make_unique<lv_anim_t>();
      lv_anim_init(anim.get());

      if (this->props.ref != nullptr) {
        this->props.ref->set(this);
      }

      if (this->props.control.second != nullptr) {
        this->props.control.second->append(this->props.control.first, this);
      }
    }

    ~Animated() override {
      if (this->component && lv_obj_is_valid(this->component)) {
        lv_anim_del(this->component, nullptr);
      }

      if (!this->props.reactive_link.empty()) {
        this->detach_reactives<Animated>(this, this->props.reactive_link);
      }

      if (this->props.ref != nullptr) {
        this->props.ref->unlink();
      }

      if (this->props.control.second != nullptr) {}
    }

    lv_obj_t* render() override {
      this->set_parent(this->parent);

      if (this->props.child == nullptr)
        return nullptr;

      auto delegate_view = this->delegate(std::move(this->props.child));

      lv_anim_set_ready_cb(anim.get(), [](lv_anim_t* a) {
        auto* self = static_cast<Animated*>(a->user_data);
        if (self)
          self->is_running = false;
      });
      lv_anim_set_user_data(anim.get(), this);

      if (this->props.auto_start) {
        play();
      }

      return delegate_view;
    }

    void play() {
      auto* obj = get_component();
      if (!obj || !lv_obj_is_valid(obj) || is_running)
        return;

      is_running = true;
      // lv_anim_del(obj, nullptr);

      auto* a = anim.get();
      lv_anim_init(a);

      lv_anim_set_var(a, obj);
      lv_anim_set_time(a, this->props.duration);
      lv_anim_set_values(a, this->props.start_value, this->props.end_value);
      lv_anim_set_playback_time(a, this->props.playback_duration);
      lv_anim_set_repeat_count(a, this->props.repeat_count);
      lv_anim_set_delay(a, this->props.delay);
      lv_anim_set_path_cb(a, this->props.path_cb);

      set_exec_callback();

      lv_anim_start(a);
    }

    void stop() {
      auto* obj = get_component();
      if (!obj || !lv_obj_is_valid(obj))
        return;

      this->is_running = false;


      apply_value(obj, this->props.stop_value);

      lv_anim_del(obj, nullptr);
    }

    void setTo(int32_t value) {
      auto* obj = get_component();
      if (!obj || !lv_obj_is_valid(obj))
        return;

      apply_value(obj, value);
    }

    void animateTo(int32_t toValue, uint32_t ms) {
      this->props.start_value = getCurrentValue();
      this->props.end_value = toValue;
      this->props.duration = ms;
      play();
    }

    int32_t getCurrentValue() const {
      auto* obj = this->component;
      if (!obj || !lv_obj_is_valid(obj))
        return 0;

      switch (this->props.property) {
        case AnimatedProps::Property::X:
          return lv_obj_get_x(obj);
        case AnimatedProps::Property::Y:
          return lv_obj_get_y(obj);
        default:
          return this->props.start_value;
      }
    }

    Animated* append(lv_obj_t* obj) override {
      lv_obj_set_parent(obj, get_component());
      return this;
    };

   private:
    void apply_value(lv_obj_t* obj, int32_t v) {
      switch (this->props.property) {
        case AnimatedProps::Property::X:
          lv_obj_set_x(obj, static_cast<int16_t>(v));
          break;
        case AnimatedProps::Property::Y:
          lv_obj_set_y(obj, static_cast<int16_t>(v));
          break;
        case AnimatedProps::Property::Opacity:
          lv_obj_set_style_text_opa(obj, v, LV_PART_MAIN);
          lv_obj_set_style_opa(obj, v, LV_PART_MAIN);
          break;
        case AnimatedProps::Property::Zoom:
          lv_obj_set_style_transform_zoom(obj, static_cast<int16_t>(v),
                                          LV_PART_MAIN);
          break;
        case AnimatedProps::Property::Rotation:
          lv_img_set_angle(obj, static_cast<int16_t>(v));
          break;
      }
    }

    void set_exec_callback() {
      switch (this->props.property) {
        case AnimatedProps::Property::X:
          lv_anim_set_exec_cb(anim.get(), [](void* obj, int32_t v) {
            lv_obj_set_x(static_cast<lv_obj_t*>(obj), static_cast<int16_t>(v));
          });
          break;

        case AnimatedProps::Property::Y:
          lv_anim_set_exec_cb(anim.get(), [](void* obj, int32_t v) {
            lv_obj_set_y(static_cast<lv_obj_t*>(obj), static_cast<int16_t>(v));
          });
          break;

        case AnimatedProps::Property::Opacity:
          lv_anim_set_exec_cb(anim.get(), [](void* obj, int32_t opa) {
            lv_obj_set_style_text_opa(static_cast<lv_obj_t*>(obj),
                                      static_cast<lv_opa_t>(opa), LV_PART_MAIN);
            lv_obj_set_style_opa(static_cast<lv_obj_t*>(obj), opa,
                                 LV_PART_MAIN);
          });
          break;

        case AnimatedProps::Property::Zoom:
          lv_anim_set_exec_cb(anim.get(), [](void* obj, int32_t zoom) {
            lv_obj_set_style_transform_zoom(static_cast<lv_obj_t*>(obj),
                                            static_cast<int16_t>(zoom),
                                            LV_PART_MAIN);
          });
          break;

        case AnimatedProps::Property::Rotation:
          lv_anim_set_exec_cb(anim.get(), [](void* obj, int32_t angle) {
            lv_img_set_angle(static_cast<lv_obj_t*>(obj),
                             static_cast<int16_t>(angle));
          });
          break;
      }
    }
  };

}  // namespace foundation
