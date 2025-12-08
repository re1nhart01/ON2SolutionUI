//
// // Created by evgeniy on 12/4/25.
// //
//
// #ifndef IDLE_H
// #define IDLE_H
//
// #endif //IDLE_H
//
//
// #define SCREEN_TIMEOUT_MS   (30 * 1000)
//
// static uint32_t last_touch_time = 0;
// static bool backlight_on = true;
//
//
// bool my_touch_read(lv_indev_drv_t *drv, lv_indev_data_t *data)
// {
//   bool touched = gt911_read_xy(&data->point.x, &data->point.y);
//
//   if (touched) {
//       data->state = LV_INDEV_STATE_PRESSED;
//       last_touch_time = lv_tick_get(); // reset idle timer
//
//       if (!backlight_on) {
//           waveshare_rgb_lcd_bl_on();
//           backlight_on = true;
//       }
//   } else {
//       data->state = LV_INDEV_STATE_RELEASED;
//   }
//
//   return false;
// }
//
//
// static void screen_idle_timer(lv_timer_t * timer)
// {
//   if (backlight_on && lv_tick_get() - last_touch_time > SCREEN_TIMEOUT_MS) {
//       waveshare_rgb_lcd_bl_off();
//       backlight_on = false;
//   }
// }
//
// void ui_init()
// {
//   last_touch_time = lv_tick_get();
//   waveshare_rgb_lcd_bl_on();
//   backlight_on = true;
//
//   lv_timer_create(screen_idle_timer, 500, NULL); // check every 500ms
// }