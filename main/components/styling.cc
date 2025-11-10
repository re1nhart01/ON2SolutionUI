#include "styling.hh"

Styling::Styling() {
    lv_style_init(&style);
}

Styling::~Styling() {
    lv_style_reset(&style);
}

lv_style_t* Styling::getStyle() {
    return &style;
}

Styling* Styling::setBackgroundColor(lv_color_t color) {
    lv_style_set_bg_color(&style, color);
    return this;
}

Styling* Styling::setBackgroundGradient(lv_color_t start, lv_color_t end, lv_grad_dir_t dir) {
    lv_style_set_bg_grad_color(&style, end);
    lv_style_set_bg_grad_dir(&style, dir);
    return this;
}

Styling* Styling::setBackgroundOpacity(lv_opa_t opa) {
    lv_style_set_bg_opa(&style, opa);
    return this;
}

Styling* Styling::setBorderRadius(lv_coord_t radius) {
    lv_style_set_radius(&style, radius);
    return this;
}

Styling* Styling::setBorder(lv_color_t color, lv_coord_t width, lv_opa_t opa) {
    lv_style_set_border_color(&style, color);
    lv_style_set_border_width(&style, width);
    lv_style_set_border_opa(&style, opa);
    return this;
}

Styling* Styling::setShadow(lv_color_t color, lv_coord_t width, lv_coord_t spread) {
    lv_style_set_shadow_color(&style, color);
    lv_style_set_shadow_width(&style, width);
    lv_style_set_shadow_spread(&style, spread);
    return this;
}

Styling* Styling::setPadding(lv_coord_t top, lv_coord_t bottom, lv_coord_t left, lv_coord_t right) {
    lv_style_set_pad_top(&style, top);
    lv_style_set_pad_bottom(&style, bottom);
    lv_style_set_pad_left(&style, left);
    lv_style_set_pad_right(&style, right);
    return this;
}

Styling* Styling::setSize(lv_coord_t width, lv_coord_t height) {
    lv_style_set_width(&style, width);
    lv_style_set_height(&style, height);
    return this;
}

Styling* Styling::setMinMaxSize(lv_coord_t min_w, lv_coord_t min_h, lv_coord_t max_w, lv_coord_t max_h) {
    lv_style_set_min_width(&style, min_w);
    lv_style_set_min_height(&style, min_h);
    lv_style_set_max_width(&style, max_w);
    lv_style_set_max_height(&style, max_h);
    return this;
}

Styling* Styling::setTextColor(lv_color_t color) {
    lv_style_set_text_color(&style, color);
    return this;
}

Styling* Styling::setFont(const lv_font_t* font) {
    lv_style_set_text_font(&style, font);
    return this;
}

Styling* Styling::setTextAlign(lv_text_align_t align) {
    lv_style_set_text_align(&style, align);
    return this;
}

Styling* Styling::setTextOpacity(lv_opa_t opa) {
    lv_style_set_text_opa(&style, opa);
    return this;
}

Styling* Styling::setLineColor(lv_color_t color) {
    lv_style_set_line_color(&style, color);
    return this;
}

Styling* Styling::setLineWidth(lv_coord_t width) {
    lv_style_set_line_width(&style, width);
    return this;
}

Styling* Styling::setImageColor(lv_color_t color) {
    lv_style_set_img_recolor(&style, color);
    return this;
}

Styling* Styling::setImageOpacity(lv_opa_t opa) {
    lv_style_set_img_opa(&style, opa);
    return this;
}

Styling* Styling::applyTo(lv_obj_t* obj) {
    lv_obj_add_style(obj, &style, LV_PART_MAIN);
    return this;
}

Styling* Styling::setClipCorner(bool clip) {
    lv_style_set_clip_corner(&style, clip);
    return this;
}

Styling* Styling::setBgImgSrc(const void* src) {
    lv_style_set_bg_img_src(&style, src);
    return this;
}

Styling* Styling::setOutline(lv_color_t color, lv_coord_t width, lv_opa_t opa) {
    lv_style_set_outline_color(&style, color);
    lv_style_set_outline_width(&style, width);
    lv_style_set_outline_opa(&style, opa);
    return this;
}

Styling* Styling::setTransformAngle(int16_t angle) {
    lv_style_set_transform_angle(&style, angle);
    return this;
}

Styling* Styling::setTransformZoom(uint16_t zoom) {
    lv_style_set_transform_zoom(&style, zoom);
    return this;
}

Styling* Styling::setAlign(lv_align_t align) {
    lv_style_set_align(&style, align);
    return this;
}

Styling* Styling::setTransition(const lv_style_transition_dsc_t* transition) {
    lv_style_set_transition(&style, transition);
    return this;
}

Styling* Styling::setWidth(const short width) {
    this->width = width;
    return this;
}

Styling* Styling::setHeight(const short height) {
    this->height = height;
    return this;
}

Styling* Styling::setSizeW(const short width, const short height) {
    this->width = width;
    this->height = height;
    return this;
}

Styling *Styling::setMaxHeight(const short height) {
  lv_style_set_max_height(&style, height);
  return this;
}

Styling *Styling::setMaxWidth(const short height) {
  lv_style_set_max_width(&style, height);
  return this;
}


Styling *Styling::setDirection(const short direction) {
  lv_style_set_base_dir(&style, direction);
  return this;
}

Styling* Styling::setGap(const int row, const int column) {
  lv_style_set_pad_row(&style, row);
  lv_style_set_pad_column(&style, column);
  return this;
}

Styling* Styling::setLayoutFlex() {
  lv_style_set_layout(&style, LV_LAYOUT_FLEX);
  return this;
}

Styling* Styling::setFlexFlow(lv_flex_flow_t flow) {
  lv_style_set_flex_flow(&style, flow);
  return this;
}

Styling* Styling::setAlign(lv_flex_align_t align) {
  lv_style_set_align(&style, align);
  return this;
}

Styling* Styling::setFlexGrow(uint8_t grow) {
  lv_style_set_flex_grow(&style, grow);
  return this;
}