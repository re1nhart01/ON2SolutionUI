#pragma once
#include "image_props.h"

namespace foundation
{
  class Image final : public Component<ImageProps> {
  private:
    using Component::props;
    const std::string* base64_source = nullptr;

  public:
    explicit Image(const ImageProps& props)
      : Component(nullptr, nullptr, std::move(props)) {
      this->apply_reactive<Image>(this, props.reactive_delegates);
      if (this->props.ref != nullptr) {
          this->props.ref->set(this);
      }
    };

    ~Image() override
    {
      if (this->props.ref != nullptr) {
          this->props.ref->unlink();
      }
      if (!this->props.reactive_link.empty())
      {
        this->detach_reactives<Image>(this, this->props.reactive_link);
      }
    };

    lv_obj_t* render() override
    {
      Component::render();
      if (get_component() == nullptr || this->parent == nullptr) {
          this->set_component(this->create_initial(this->parent));
      }
      lv_obj_t* comp = get_component();

      if (this->props.img_src != nullptr)
      {
        lv_img_set_src(comp, this->props.img_src);
      } else
      {
        lv_img_set_src(comp, &this->props.img_dsc);
      }
      lv_obj_set_width(comp, this->props.real_width);
      lv_obj_set_height(comp, this->props.real_height);
      lv_obj_align(comp, LV_ALIGN_CENTER, 0, 0);

      auto style = this->styling();
      if (style != nullptr) {
          lv_obj_add_style(comp, style->getStyle(), LV_PART_MAIN);
      }

      return comp;
    };

    const Styling* styling() const override
    {
      style.reset();

      apply_base_style(style);

      if (props.style_override) {
          props.style_override(style);
      }

      return &style;
    }

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