//
// Created by evgeniy on 5/11/25.
//

#pragma once

#include "lvgl.h"

#ifndef SHARED_HH
#define SHARED_HH

namespace foundation {
  struct flexible {
    lv_flex_align_t justify_content = LV_FLEX_ALIGN_START;
    lv_flex_align_t align_items = LV_FLEX_ALIGN_CENTER;
    lv_flex_align_t track_cross_place = LV_FLEX_ALIGN_START;
    lv_flex_flow_t flex_direction = LV_FLEX_FLOW_COLUMN;
  };

  inline flexible flex_row_center = {
    .justify_content = LV_FLEX_ALIGN_CENTER,
    .align_items = LV_FLEX_ALIGN_CENTER,
    .track_cross_place = LV_FLEX_ALIGN_CENTER,
    .flex_direction = LV_FLEX_FLOW_ROW
  };

  inline flexible flex_row_space_between = {
    .justify_content = LV_FLEX_ALIGN_SPACE_BETWEEN,
    .align_items = LV_FLEX_ALIGN_START,
    .track_cross_place = LV_FLEX_ALIGN_START,
    .flex_direction = LV_FLEX_FLOW_ROW
  };

  inline flexible flex_col_reverse = {
    .justify_content = LV_FLEX_ALIGN_END,
    .align_items = LV_FLEX_ALIGN_CENTER,
    .track_cross_place = LV_FLEX_ALIGN_CENTER,
    .flex_direction = LV_FLEX_FLOW_COLUMN_REVERSE
  };

  inline flexible flex_wrap_evenly = {
    .justify_content = LV_FLEX_ALIGN_SPACE_EVENLY,
    .align_items = LV_FLEX_ALIGN_CENTER,
    .track_cross_place = LV_FLEX_ALIGN_CENTER,
    .flex_direction = LV_FLEX_FLOW_ROW_WRAP
  };

  inline flexible flex_col_wrap_around = {
    .justify_content = LV_FLEX_ALIGN_SPACE_AROUND,
    .align_items = LV_FLEX_ALIGN_CENTER,
    .track_cross_place = LV_FLEX_ALIGN_CENTER,
    .flex_direction = LV_FLEX_FLOW_COLUMN_WRAP
  };

  inline flexible flex_wrap_end = {
    .justify_content = LV_FLEX_ALIGN_END,
    .align_items = LV_FLEX_ALIGN_END,
    .track_cross_place = LV_FLEX_ALIGN_END,
    .flex_direction = LV_FLEX_FLOW_ROW_WRAP_REVERSE
  };

  inline flexible flex_full_center = {
    .justify_content = LV_FLEX_ALIGN_CENTER,
    .align_items = LV_FLEX_ALIGN_CENTER,
    .track_cross_place = LV_FLEX_ALIGN_CENTER,
    .flex_direction = LV_FLEX_FLOW_COLUMN
  };
}

#endif //SHARED_HH
