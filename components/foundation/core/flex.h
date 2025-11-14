#pragma once

#include "extra/layouts/flex/lv_flex.h"


typedef enum {
  flex_direction_row                 = 0x00,
  flex_direction_column              = _LV_FLEX_COLUMN,
  flex_direction_row_wrap            = LV_FLEX_FLOW_ROW | _LV_FLEX_WRAP,
  flex_direction_row_reverse        = LV_FLEX_FLOW_ROW | _LV_FLEX_REVERSE,
  lex_direction_row_wrap_reverse    = LV_FLEX_FLOW_ROW | _LV_FLEX_WRAP | _LV_FLEX_REVERSE,
  flex_direction_column_wrap         = LV_FLEX_FLOW_COLUMN | _LV_FLEX_WRAP,
  flex_direction_column_reverse      = LV_FLEX_FLOW_COLUMN | _LV_FLEX_REVERSE,
  lex_direction_column_wrap_reverse = LV_FLEX_FLOW_COLUMN | _LV_FLEX_WRAP | _LV_FLEX_REVERSE,
} foundation_flex_dir_t;


typedef enum {
  flex_start,
  flex_end,
  center,
  space_evenly,
  space_around,
  space_between,
} foundation_flex_align_t;