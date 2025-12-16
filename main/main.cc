#include "app.cc"
#include "core/fs.h"

#include <memory>

extern "C" {
  #include "core/waveshare_rgb_lcd_port.h"
}

using namespace foundation;

std::shared_ptr<WaveApplication> application;

[[foundation::entrypoint]]
void start() {
  littlefs_init();
  waveshare_esp32_s3_rgb_lcd_init();

  if (lvgl_port_lock(-1)) {
    lv_obj_t *screen = lv_scr_act();
    lvgl_littlefs_driver_init();

    FILE *f = fopen("/littlefs/logo.bin", "rb");
    if (f == NULL) {
        ESP_LOGE("LFS_TEST", "Failed to open file /littlefs/logo.bin!");
    } else {
        ESP_LOGI("LFS_TEST", "Successfully opened /littlefs/logo.bin.");
        fclose(f);
    }

    application = std::make_shared<WaveApplication>(screen);
    application->renderApp();

    lvgl_port_unlock();
  }
}



entry (start) end_entry
