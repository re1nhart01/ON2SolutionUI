//
// Created by evgeniy on 12/21/25.
//
#pragma once

#include "misc/lv_color.h"

namespace ON2Solutions {
  const lv_color_t PRIMARY_COLOR = lv_color_hex(0x8BC53D);
  const lv_color_t PRIMARY_COLOR_2 = lv_color_hex(0x0D928D);
  const lv_color_t ERROR_COLOR = lv_color_hex(0xE53935);
  const lv_color_t NO_ERROR_COLOR = lv_color_hex(0x388E3C);
  const lv_color_t NOT_ACTIVE_INDICATOR = lv_color_hex(0xC7CDD6);
  const lv_color_t ACTIVE_INDICATOR = lv_color_hex(0x2F80ED);

  const lv_color_t STATUS_STANDBY         = lv_color_hex(0x6B7280); // muted gray
  const lv_color_t STATUS_BOOSTER_DELAY   = lv_color_hex(0xB45309); // muted amber/brown
  const lv_color_t STATUS_STARTUP         = lv_color_hex(0x1E40AF); // deep blue
  const lv_color_t STATUS_RUNNING         = lv_color_hex(0x15803D); // deep green
  const lv_color_t STATUS_PRODUCING       = lv_color_hex(0x0F766E); // muted teal
  const lv_color_t STATUS_ALARM           = lv_color_hex(0xB91C1C); // deep red
  const lv_color_t STATUS_TANK_FULL       = lv_color_hex(0x6D28D9); // deep purple
  const lv_color_t STATUS_AUTO_ADJUSTING  = lv_color_hex(0x4338CA); // deep indigo
}