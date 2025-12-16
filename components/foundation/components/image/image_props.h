#pragma once
#include "image.h"

namespace foundation
{
  class Image;
  struct ImageProps final : BaseProps<ImageProps, Image> {
    std::shared_ptr<Styling> style;

    lv_img_dsc_t img_dsc;
    const char* img_src;
    short real_width = 0;
    short real_height = 0;

    static ImageProps up() { return ImageProps{}; }

    ImageProps& set_style(std::shared_ptr<Styling> s) {
      style = std::move(s);
      return *this;
    }

    ImageProps& source(const char* src)
    {
      this->img_src = src;
      return *this;
    }

    ImageProps& source(const lv_img_dsc_t& src)
    {
      this->img_dsc = src;
      return *this;
    }

    ImageProps& width(short w) {
      real_width = w;
      return *this;
    }

    ImageProps& height(short h) {
      real_height = h;
      return *this;
    }
  };
}