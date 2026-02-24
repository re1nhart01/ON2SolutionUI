
#pragma once

#include <lvgl.h>

inline void uart_send_hex(const uint8_t* data, size_t len) {
  printf("SCREENSHOT_BEGIN\n");

  for (size_t i = 0; i < len; i++) {
    printf("%02X", data[i]);
  }

  printf("\nSCREENSHOT_END\n");
}

inline void do_snapshot() {
  lv_img_dsc_t* img = lv_snapshot_take(lv_scr_act(), LV_IMG_CF_TRUE_COLOR);
  uart_send_hex(img->data, img->data_size);
}