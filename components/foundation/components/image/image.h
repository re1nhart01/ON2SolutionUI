#pragma once
#include "image_props.h"

namespace foundation
{
  class Image final : public Component {
  private:
    ImageProps props;
    const std::string* base64_source = nullptr;
    lv_img_dsc_t img_dsc;

  public:
    explicit Image(const lv_img_dsc_t source, const ImageProps& props)
      : Component(nullptr, nullptr), props(props), img_dsc(source) {
      set_style(props.style);

      if (this->props.ref != nullptr) {
          this->props.ref->set(this);
      }
    };

    ~Image() override = default;

    lv_obj_t* render() override
    {
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
    };

    std::shared_ptr<Styling> styling() override
    {
      return this->props.style ? this->props.style : std::shared_ptr<Styling>{};
    };

    Image* append(lv_obj_t* obj) override
    {
      lv_obj_set_parent(obj, get_component());
      return this;
    };

  private:
    lv_obj_t* create_initial(lv_obj_t* parental) const
    {
      return lv_img_create(parental);
    };
  };
}