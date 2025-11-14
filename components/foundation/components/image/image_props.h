#pragma once


namespace foundation
{
  struct ImageProps final : BaseProps<ImageProps> {
    std::shared_ptr<Ref> ref = nullptr;
    std::shared_ptr<Styling> style;

    short real_width = 0;
    short real_height = 0;

    static ImageProps up() { return ImageProps{}; }

    ImageProps& set_ref(std::shared_ptr<Ref> r) {
      ref = std::move(r);
      return *this;
    }

    ImageProps& set_style(std::shared_ptr<Styling> s) {
      style = std::move(s);
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