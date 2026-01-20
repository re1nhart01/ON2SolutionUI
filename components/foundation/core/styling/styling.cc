
#include "styling.h"

lv_style_t* Styling::getStyle() const {
  return style.get();
}

bool Styling::get_is_dirty() const {
  return this->dirty;
}

void Styling::set_is_dirty(const bool v) {
  this->dirty = v;
}

Styling* Styling::setBackgroundColor(lv_color_t color) {
  lv_style_set_bg_color(style.get(), color);
  this->dirty = true;
  return this;
}

Styling* Styling::setBackgroundGradient(lv_color_t start, lv_color_t end,
                                        lv_grad_dir_t dir) {
  this->dirty = true;
  lv_style_set_bg_grad_color(style.get(), end);
  lv_style_set_bg_grad_dir(style.get(), dir);
  return this;
}

Styling* Styling::setBackgroundOpacity(lv_opa_t opa) {
  this->dirty = true;
  lv_style_set_bg_opa(style.get(), opa);
  return this;
}

Styling* Styling::setBorderRadius(lv_coord_t radius) {
  this->dirty = true;
  lv_style_set_radius(style.get(), radius);
  return this;
}

Styling* Styling::setBorder(lv_color_t color, lv_coord_t width, lv_opa_t opa) {
  this->dirty = true;
  lv_style_set_border_color(style.get(), color);
  lv_style_set_border_width(style.get(), width);
  lv_style_set_border_opa(style.get(), opa);
  return this;
}

Styling* Styling::setShadow(lv_color_t color, lv_coord_t width,
                            lv_coord_t spread) {
  this->dirty = true;
  lv_style_set_shadow_color(style.get(), color);
  lv_style_set_shadow_width(style.get(), width);
  lv_style_set_shadow_spread(style.get(), spread);
  return this;
}

Styling* Styling::setPadding(lv_coord_t top, lv_coord_t bottom, lv_coord_t left,
                             lv_coord_t right) {
  this->dirty = true;
  lv_style_set_pad_top(style.get(), top);
  lv_style_set_pad_bottom(style.get(), bottom);
  lv_style_set_pad_left(style.get(), left);
  lv_style_set_pad_right(style.get(), right);
  return this;
}

Styling* Styling::setSize(lv_coord_t width, lv_coord_t height) {
  this->dirty = true;
  lv_style_set_width(style.get(), width);
  lv_style_set_height(style.get(), height);
  return this;
}

Styling* Styling::setMinMaxSize(lv_coord_t min_w, lv_coord_t min_h,
                                lv_coord_t max_w, lv_coord_t max_h) {
  this->dirty = true;
  lv_style_set_min_width(style.get(), min_w);
  lv_style_set_min_height(style.get(), min_h);
  lv_style_set_max_width(style.get(), max_w);
  lv_style_set_max_height(style.get(), max_h);
  return this;
}

Styling* Styling::setTextColor(lv_color_t color) {
  this->dirty = true;
  lv_style_set_text_color(style.get(), color);
  return this;
}

Styling* Styling::setFont(const lv_font_t* font) {
  this->dirty = true;
  lv_style_set_text_font(style.get(), font);
  return this;
}

Styling* Styling::setTextAlign(lv_text_align_t align) {
  this->dirty = true;
  lv_style_set_text_align(style.get(), align);
  return this;
}

Styling* Styling::setTextOpacity(lv_opa_t opa) {
  this->dirty = true;
  lv_style_set_text_opa(style.get(), opa);
  return this;
}

Styling* Styling::setLineColor(lv_color_t color) {
  this->dirty = true;
  lv_style_set_line_color(style.get(), color);
  return this;
}

Styling* Styling::setLineWidth(lv_coord_t width) {
  this->dirty = true;
  lv_style_set_line_width(style.get(), width);
  return this;
}

Styling* Styling::setImageColor(lv_color_t color) {
  this->dirty = true;
  lv_style_set_img_recolor(style.get(), color);
  return this;
}

Styling* Styling::setImageOpacity(lv_opa_t opa) {
  this->dirty = true;
  lv_style_set_img_opa(style.get(), opa);
  return this;
}

void Styling::applyTo(lv_obj_t* obj) const {
  lv_obj_add_style(obj, style.get(), LV_PART_MAIN);
}

Styling* Styling::setClipCorner(bool clip) {
  this->dirty = true;
  lv_style_set_clip_corner(style.get(), clip);
  return this;
}

Styling* Styling::setBgImgSrc(const void* src) {
  this->dirty = true;
  lv_style_set_bg_img_src(style.get(), src);
  return this;
}

Styling* Styling::setOutline(lv_color_t color, lv_coord_t width, lv_opa_t opa) {
  this->dirty = true;
  lv_style_set_outline_color(style.get(), color);
  lv_style_set_outline_width(style.get(), width);
  lv_style_set_outline_opa(style.get(), opa);
  return this;
}

Styling* Styling::setTransformAngle(int16_t angle) {
  this->dirty = true;
  lv_style_set_transform_angle(style.get(), angle);
  return this;
}

Styling* Styling::setTransformZoom(uint16_t zoom) {
  this->dirty = true;
  lv_style_set_transform_zoom(style.get(), zoom);
  return this;
}

Styling* Styling::setAlign(lv_align_t align) {
  this->dirty = true;
  lv_style_set_align(style.get(), align);
  return this;
}

Styling* Styling::setTransition(const lv_style_transition_dsc_t* transition) {
  this->dirty = true;
  lv_style_set_transition(style.get(), transition);
  return this;
}

Styling* Styling::setWidth(const short width) {
  this->dirty = true;
  this->width = width;
  return this;
}

Styling* Styling::setHeight(const short height) {
  this->dirty = true;
  this->height = height;
  return this;
}

Styling* Styling::setSizeW(const short width, const short height) {
  this->dirty = true;
  this->width = width;
  this->height = height;
  return this;
}

Styling* Styling::setMaxHeight(const short height) {
  this->dirty = true;
  lv_style_set_max_height(style.get(), height);
  return this;
}

Styling* Styling::setMaxWidth(const short height) {
  this->dirty = true;
  lv_style_set_max_width(style.get(), height);
  return this;
}

Styling* Styling::setDirection(const short direction) {
  this->dirty = true;
  lv_style_set_base_dir(style.get(), direction);
  return this;
}

Styling* Styling::setGap(const short row, const short column) {
  this->dirty = true;
  lv_style_set_pad_row(style.get(), row);
  lv_style_set_pad_column(style.get(), column);
  return this;
}

Styling* Styling::setLayoutFlex() {
  this->dirty = true;
  lv_style_set_layout(style.get(), LV_LAYOUT_FLEX);
  return this;
}

Styling* Styling::setFlexFlow(lv_flex_flow_t flow) {
  this->dirty = true;
  lv_style_set_flex_flow(style.get(), flow);
  return this;
}

Styling* Styling::setAlign(lv_flex_align_t align) {
  this->dirty = true;
  lv_style_set_align(style.get(), align);
  return this;
}

Styling* Styling::setFlexGrow(uint8_t grow) {
  this->dirty = true;
  lv_style_set_flex_grow(style.get(), grow);
  return this;
}

Styling* Styling::setBackgroundOpa(lv_opa_t opa) {
  this->dirty = true;
  lv_style_set_bg_opa(style.get(), opa);
  return this;
}

void Styling::reset() {
  this->dirty = false;
  lv_style_reset(style.get());
  lv_style_init(style.get());
}
