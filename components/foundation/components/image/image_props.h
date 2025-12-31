#pragma once
#include "image.h"
#include "components/props.h";

namespace foundation
{
  class Image;
  struct ImageProps final : BaseProps<ImageProps, Image> {
    lv_img_dsc_t img_dsc = {};
    const char* img_src = nullptr;
    short real_width = 0;
    short real_height = 0;
    short orig_width = 0;
    short orig_height = 0;

    ImageProps& source(const char* src)
    {
      this->img_src = src;
      return *this;
    }

    ImageProps& original_size(short w, short h) {
      orig_width = w;
      orig_height = h;
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