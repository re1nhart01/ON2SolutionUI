
#include "components/style_store.hh"

//----------------------------------- Shorthands ------------------------------------------------

#define screen_container_props \
  .width = LV_PCT(100), \
  .height = LV_PCT(100), \
  .justify_content = LV_FLEX_ALIGN_START, \
  .align_items = LV_FLEX_ALIGN_CENTER, \
  .track_cross_place = LV_FLEX_ALIGN_CENTER, \
  .flex_direction = LV_FLEX_FLOW_COLUMN

#define header_container_props \
  .width = LV_PCT(100), \
  .height = 55, \
  .justify_content = LV_FLEX_ALIGN_SPACE_BETWEEN, \
  .align_items = LV_FLEX_ALIGN_CENTER, \
  .track_cross_place = LV_FLEX_ALIGN_CENTER, \
  .flex_direction = LV_FLEX_FLOW_ROW, \

#define header_container_right_props \
  .width = LV_PCT(40), \
  .height = 55, \
  .justify_content = LV_FLEX_ALIGN_END, \
  .align_items = LV_FLEX_ALIGN_CENTER, \
  .track_cross_place = LV_FLEX_ALIGN_CENTER, \
  .flex_direction = LV_FLEX_FLOW_ROW, \

#define header_container_props \
  .width = LV_PCT(100), \
  .height = 55, \
  .justify_content = LV_FLEX_ALIGN_SPACE_BETWEEN, \
  .align_items = LV_FLEX_ALIGN_CENTER, \
  .track_cross_place = LV_FLEX_ALIGN_CENTER, \
  .flex_direction = LV_FLEX_FLOW_ROW, \

#define header_labels_container_props \
  .width = LV_PCT(60), \
  .height = LV_PCT(100), \
  .justify_content = LV_FLEX_ALIGN_SPACE_BETWEEN, \
  .align_items = LV_FLEX_ALIGN_CENTER, \
  .track_cross_place = LV_FLEX_ALIGN_CENTER, \
  .flex_direction = LV_FLEX_FLOW_ROW, \


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
