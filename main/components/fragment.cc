#include "fragment.hh"

namespace foundation {

  Fragment::Fragment()
      : Component(nullptr, nullptr) {
    this->parent = nullptr;
  }

  lv_obj_t* Fragment::render() {
    Component::render();

    lv_obj_t* parent_obj = this->get_parent();
    if (!parent_obj) return nullptr;

    set_component(lv_obj_create(parent_obj));
    auto* obj = get_component();

    lv_obj_set_size(obj, 1, 1);

    lv_obj_set_style_bg_opa(obj, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(obj, 0, 0);
    lv_obj_set_style_outline_width(obj, 0, 0);
    lv_obj_set_style_shadow_width(obj, 0, 0);
    lv_obj_set_style_radius(obj, 0, 0);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE);

    return obj;
  }

  std::shared_ptr<Styling> Fragment::styling() {
    return nullptr;
  }


  Fragment* Fragment::append(lv_obj_t* obj) {
    lv_obj_set_parent(obj, get_component());
    return this;
  }

} // namespace foundation
