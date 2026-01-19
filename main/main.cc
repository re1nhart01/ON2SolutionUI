#include "app.cc"
#include "core/fs.h"

#include <memory>

extern "C" {
  #include "core/waveshare_rgb_lcd_port.h"
}

std::unique_ptr<ON2Solutions::WaveApplication> application;

[[foundation::entrypoint]]
void start() {
  littlefs_init();
  lvgl_littlefs_driver_init();
  waveshare_esp32_s3_rgb_lcd_init();

  if (lvgl_port_lock(-1)) {
    lv_obj_t *screen = lv_scr_act();
    lvgl_littlefs_driver_init();

    application = std::make_unique<ON2Solutions::WaveApplication>(screen);
    application->renderApp();

    lvgl_port_unlock();
  }
}



entry (start) end_entry
