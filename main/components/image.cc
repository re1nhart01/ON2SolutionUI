#include "image.hh"

namespace foundation {

  Image::Image(const lv_img_dsc_t source, const image_props& props)
      : Component(nullptr, nullptr), props(props), img_dsc(source) {
    set_style(props.style);

    if (this->props.ref != nullptr) {
        this->props.ref->set(this);
    }
  }

  lv_obj_t* Image::render() {
    Component::render();

    if (get_component() == nullptr || this->parent == nullptr) {
        this->set_component(this->create_initial(this->parent));
    }

    lv_obj_t* comp = get_component();
    lv_img_set_src(comp, &this->img_dsc);
    lv_obj_align(comp, LV_ALIGN_CENTER, 0, 0);

    std::shared_ptr<Styling> style = this->styling();
    if (style != nullptr) {
        lv_obj_add_style(comp, style->getStyle(), LV_PART_MAIN);
    }

    return comp;
  }

  std::shared_ptr<Styling> Image::styling() {
    return this->props.style ? this->props.style : std::shared_ptr<Styling>{};
  }

  Image* Image::append(lv_obj_t* obj) {
    lv_obj_set_parent(obj, get_component());
    return this;
  }

  lv_obj_t* Image::create_initial(lv_obj_t* parental) {
    return lv_img_create(parental);
  }

} // namespace foundation
