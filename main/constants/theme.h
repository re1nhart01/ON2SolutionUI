//
// Created by evgeniy on 12/21/25.
//
#pragma once

#include "lvgl.h"

namespace ON2Solutions {
  const lv_color_t PRIMARY_COLOR = lv_color_hex(0x8CC63E);
  const lv_color_t PRIMARY_COLOR_2 = lv_color_hex(0x0E928D);
  const lv_color_t PRIMARY_COLOR_3 = lv_color_hex(0xDDEFEF);
  const lv_color_t ERROR_COLOR = lv_color_hex(0xE61F23);
  const lv_color_t ERROR_COLOR_2 = lv_color_hex(0xE61F230A);
  const lv_color_t ERROR_COLOR_3 = lv_color_hex(0xFF7578);
  const lv_color_t TERTIARY_COLOR = lv_color_hex(0x2368DE);


  const lv_color_t PRIMARY_BG = lv_color_hex(0xFFFFFF);
  const lv_color_t SECONDARY_BG = lv_color_hex(0xF8F9FC);
  const lv_color_t TERTIARY_BG = lv_color_hex(0xF7F7F8);

  const lv_color_t BORDER_PRIMARY = lv_color_hex(0xD0D5DD);
  const lv_color_t BORDER_SECONDARY = lv_color_hex(0xE7E8E9);

  const lv_color_t NO_ERROR_COLOR = lv_color_hex(0x20B746);
  const lv_color_t NOT_ACTIVE_INDICATOR = lv_color_hex(0xC7CDD6);
  const lv_color_t ACTIVE_INDICATOR = lv_color_hex(0x2F80ED);

  const lv_color_t STATUS_STANDBY         = lv_color_hex(0x6B7280); // muted gray
  const lv_color_t STATUS_BOOSTER_DELAY   = lv_color_hex(0xB45309); // muted amber/brown
  const lv_color_t STATUS_RUNNING         = lv_color_hex(0x15803D); // deep green
  const lv_color_t STATUS_PRODUCING       = lv_color_hex(0x0F766E); // muted teal
  const lv_color_t STATUS_ALARM           = lv_color_hex(0xB91C1C); // deep red
  const lv_color_t STATUS_TANK_FULL       = lv_color_hex(0x6D28D9); // deep purple
  const lv_color_t STATUS_AUTO_ADJUSTING  = lv_color_hex(0x4338CA); // deep indigo
}