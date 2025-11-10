#pragma once

#include "lvgl.h"

#include <functional>
#include <memory>

class Styling
{
private:
  lv_style_t style;
  short width = -1;
  short height = -1;

public:
  Styling();
  ~Styling();

  lv_style_t *getStyle();

  Styling *setBackgroundColor(lv_color_t color);
  Styling *
  setBackgroundGradient(lv_color_t start, lv_color_t end, lv_grad_dir_t dir);
  Styling *setBackgroundOpacity(lv_opa_t opa);
  Styling *setBorderRadius(lv_coord_t radius);
  Styling *setBorder(lv_color_t color, lv_coord_t width, lv_opa_t opa);
  Styling *setShadow(lv_color_t color, lv_coord_t width, lv_coord_t spread);
  Styling *setPadding(lv_coord_t top, lv_coord_t bottom, lv_coord_t left,
                      lv_coord_t right);
  Styling *setSize(lv_coord_t width, lv_coord_t height);
  Styling *setMinMaxSize(lv_coord_t min_w, lv_coord_t min_h, lv_coord_t max_w,
                         lv_coord_t max_h);
  Styling *setTextColor(lv_color_t color);
  Styling *setFont(const lv_font_t *font);
  Styling *setTextAlign(lv_text_align_t align);
  Styling *setTextOpacity(lv_opa_t opa);
  Styling *setLineColor(lv_color_t color);
  Styling *setLineWidth(lv_coord_t width);
  Styling *setImageColor(lv_color_t color);
  Styling *setImageOpacity(lv_opa_t opa);
  Styling *applyTo(lv_obj_t *obj);
  Styling *setClipCorner(bool clip);
  Styling *setBgImgSrc(const void *src);
  Styling *setOutline(lv_color_t color, lv_coord_t width, lv_opa_t opa);
  Styling *setTransformAngle(int16_t angle);
  Styling *setTransformZoom(uint16_t zoom);
  Styling *setAlign(lv_align_t align);
  Styling *setTransition(const lv_style_transition_dsc_t *transition);
  Styling *setWidth(const short width);
  Styling *setHeight(const short height);
  Styling *setSizeW(const short width, const short height);
  Styling *setMaxHeight(const short height);
  Styling *setMaxWidth(const short height);
  Styling *setDirection(const short direction);
  Styling *setGap(int row, int column);
  Styling* setLayoutFlex();
  Styling* setFlexFlow(lv_flex_flow_t flow);
  Styling* setAlign(lv_flex_align_t align);
  Styling* setFlexGrow(uint8_t grow);
};

inline std::shared_ptr<Styling> make_style(std::function<void(Styling&)> fn) {
  auto s = std::make_shared<Styling>();
  fn(*s);
  return s;
}