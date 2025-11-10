#pragma once

#include "component.hh"
#include "styling.hh"
#include <memory>
#include <string>

namespace foundation {

  struct image_props {
    std::shared_ptr<Ref> ref = nullptr;
    std::shared_ptr<Styling> style;

    short real_width = 0;
    short real_height = 0;
  };

  class Image final : public Component {
  private:
    image_props props;
    const std::string* base64_source = nullptr;  // зарезервовано під майбутнє
    lv_img_dsc_t img_dsc;

  public:
    explicit Image(const lv_img_dsc_t source, const image_props& props);
    ~Image() override = default;

    lv_obj_t* render() override;
    std::shared_ptr<Styling> styling() override;
    Image* append(lv_obj_t* obj) override;

  private:
    lv_obj_t* create_initial(lv_obj_t* parental);
  };

} // namespace foundation
