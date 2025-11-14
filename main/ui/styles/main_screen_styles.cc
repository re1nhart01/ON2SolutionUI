
#include "../../components/foundation/core/style_store/style_store.h"

//----------------------------------- Shorthands
//------------------------------------------------

inline void screen_container_props(foundation::ViewProps & p) {
  p.w(LV_PCT(100));
  p.h(LV_PCT(100));
  p.justify(LV_FLEX_ALIGN_START);
  p.items(LV_FLEX_ALIGN_CENTER);
  p.track_cross(LV_FLEX_ALIGN_CENTER);
  p.direction(LV_FLEX_FLOW_COLUMN);
}

// ---------------------------- HEADER MAIN ----------------------------

inline void header_container_props(foundation::ViewProps & p) {
  p.w(LV_PCT(100));
  p.h(55);
  p.justify(LV_FLEX_ALIGN_SPACE_BETWEEN);
  p.items(LV_FLEX_ALIGN_CENTER);
  p.track_cross(LV_FLEX_ALIGN_CENTER);
  p.direction(LV_FLEX_FLOW_ROW);
}

// ---------------------------- HEADER RIGHT ----------------------------

inline void header_container_right_props(foundation::ViewProps & p) {
  p.w(LV_PCT(40));
  p.h(55);
  p.justify(LV_FLEX_ALIGN_END);
  p.items(LV_FLEX_ALIGN_CENTER);
  p.track_cross(LV_FLEX_ALIGN_CENTER);
  p.direction(LV_FLEX_FLOW_ROW);
}

// ---------------------------- HEADER LABELS ----------------------------

inline void header_labels_container_props(foundation::ViewProps & p) {
  p.w(LV_PCT(60));
  p.h(LV_PCT(100));
  p.justify(LV_FLEX_ALIGN_SPACE_BETWEEN);
  p.items(LV_FLEX_ALIGN_CENTER);
  p.track_cross(LV_FLEX_ALIGN_CENTER);
  p.direction(LV_FLEX_FLOW_ROW);
}
//----------------------------------- Styles ------------------------------------------------


void style_main_screen_register(foundation::StyleStorage& storage) {
  storage.set("header.button", make_style([](Styling& style) {
    style.setBackgroundColor(lv_color_hex(0x5B5AFF));
    style.setBorderRadius(24);
    style.setBorder(lv_color_hex(0x5B5AFF), 0, 0);
    style.setPadding(8, 8, 16, 16);
  }));
  storage.set("header.container", make_style([](Styling& style) {
    style.setPadding(0, 0, 16, 16);
    style.setBorderRadius(0);
    style.setBorder(lv_color_make(255, 255, 255), 0, 0);
  }));
  storage.set("header.label", make_style([](Styling& style) {
    style.setFont(&lv_font_montserrat_12);
  }));
  storage.set("header.labels.container", make_style([](Styling& style) {
    style.setFont(&lv_font_montserrat_12);
    style.setPadding(8, 8, 16, 16);
  }));

  storage.set("footer.button", make_style([](Styling& style) {
    style.setFont(&lv_font_montserrat_16);
     style.setBackgroundColor(lv_color_hex(0x000000));
     style.setBorderRadius(12);
     style.setSize(LV_PCT(96), 50);
     style.setBorder(lv_color_hex(0x5B5AFF), 0, 0);
     style.setPadding(8, 8, 16, 16);
  }));

  storage.set("footer.container", make_style([](Styling& style) {
  // style.setPadding(0, 0, 16, 16);
  // style.setBorderRadius(0);
  // style.setBorder(lv_color_make(255, 255, 255), 0, 0);
}));
}
