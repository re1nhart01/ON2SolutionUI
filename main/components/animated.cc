#include "animated.hh"

namespace foundation::animated {

Animated::Animated(Component* component, animated_config* config, animated_sub_config* sub_config) {
    this->config = config;
    this->sub_config = sub_config;
    this->component = component;
    this->reference = std::make_unique<lv_anim_t>();

    lv_anim_init(this->reference.get());
}

lv_obj_t* Animated::render() {
    const auto obj = this->create_initial(this->parent);
    auto* ref = this->reference.get();

    lv_anim_set_var(ref, obj);
    lv_anim_set_time(ref, this->config->duration);
    lv_anim_set_values(ref, this->config->start_value, this->config->end_value);
    lv_anim_set_playback_time(ref, this->config->playback_duration);
    lv_anim_set_repeat_count(ref, this->config->repeat_count);

    switch (this->config->property) {
        case animated_config::Property::X:
            make_move_x_animation();
            break;
        case animated_config::Property::Y:
            make_move_y_animation();
            break;
        case animated_config::Property::Opacity:
            make_opacity_animation();
            break;
        case animated_config::Property::Zoom:
            make_zoom_animation();
            break;
        case animated_config::Property::Rotation:
            make_rotation_animation();
            break;
    }

    lv_anim_start(ref);
    return obj;
}

void Animated::make_move_y_animation() const {
    lv_anim_set_exec_cb(this->reference.get(), [](void* obj, int32_t v) {
        lv_obj_set_y(static_cast<lv_obj_t*>(obj), static_cast<int16_t>(v));
    });
}

void Animated::make_move_x_animation() const {
    lv_anim_set_exec_cb(this->reference.get(), [](void* obj, int32_t v) {
        lv_obj_set_x(static_cast<lv_obj_t*>(obj), static_cast<int16_t>(v));
    });
}

void Animated::make_opacity_animation() const {
    lv_anim_set_exec_cb(this->reference.get(), [](void* obj, int32_t opa) {
        lv_obj_set_style_opa(static_cast<lv_obj_t*>(obj), opa, LV_PART_MAIN);
    });
}

void Animated::make_zoom_animation() const {
    lv_anim_set_exec_cb(this->reference.get(), [](void* obj, int32_t zoom) {
        lv_obj_set_style_transform_zoom(static_cast<lv_obj_t*>(obj), static_cast<int16_t>(zoom), LV_PART_MAIN);
    });
}

void Animated::make_rotation_animation() const {
    lv_anim_set_exec_cb(this->reference.get(), [](void* obj, int32_t angle) {
        lv_img_set_angle(static_cast<lv_obj_t*>(obj), static_cast<int16_t>(angle));
    });
}

std::shared_ptr<Styling> Animated::styling() {
    return {};
}

Animated* Animated::append(lv_obj_t* obj) {
    lv_obj_set_parent(obj, get_component());
    return this;
}

lv_obj_t* Animated::create_initial(lv_obj_t* parental) {
    this->set_parent(parental);
    return this->component->render();
}

} // namespace foundation::animated
