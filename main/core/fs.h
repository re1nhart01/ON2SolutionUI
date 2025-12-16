
#pragma once
#include "lvgl.h"
#include "esp_littlefs.h"
#include "esp_log.h"

static const char *FS_TAG = "LITTLEFS";

inline void littlefs_init() {
  esp_vfs_littlefs_conf_t conf = {
    .base_path = "/littlefs",
    .partition_label = "storage",
    .format_if_mount_failed = true
  };

  esp_err_t ret = esp_vfs_littlefs_register(&conf);

  if (ret != ESP_OK) {
      if (ret == ESP_FAIL) {
          ESP_LOGE(FS_TAG, "Failed to mount or format littlefs");
      } else if (ret == ESP_ERR_NOT_FOUND) {
          ESP_LOGE(FS_TAG, "Failed to find littlefs partition");
      } else {
          ESP_LOGE(FS_TAG, "Failed to initialize littlefs (%s)", esp_err_to_name(ret));
      }
      return;
  }

  ESP_LOGI(FS_TAG, "LittleFS mounted successfully");
}


extern "C" {
  inline lv_fs_drv_t littlefs_drv;
}

static void* fs_open(lv_fs_drv_t *drv, const char *path, lv_fs_mode_t mode) {
  char fs_mode[4];
  if (mode == LV_FS_MODE_RD) snprintf(fs_mode, sizeof(fs_mode), "rb");
  else if (mode == LV_FS_MODE_WR) snprintf(fs_mode, sizeof(fs_mode), "wb");
  else if (mode == LV_FS_MODE_RD | LV_FS_MODE_WR) snprintf(fs_mode, sizeof(fs_mode), "r+b");

  return fopen(path, fs_mode);
}

static lv_fs_res_t fs_close(lv_fs_drv_t *drv, void *file) {
  fclose((FILE*)file);
  return LV_FS_RES_OK;
}

static lv_fs_res_t
fs_read(lv_fs_drv_t *drv, void *file, void *buf, uint32_t btr, uint32_t *br)
{
  *br = fread(buf, 1, btr, (FILE *)file);
  return (*br <= 0) ? LV_FS_RES_UNKNOWN : LV_FS_RES_OK;
}

inline void lvgl_littlefs_driver_init() {
  lv_fs_drv_init(&littlefs_drv);
  littlefs_drv.letter = 'S';
  littlefs_drv.open_cb = fs_open;
  littlefs_drv.close_cb = fs_close;
  littlefs_drv.read_cb = fs_read;
  lv_fs_drv_register(&littlefs_drv);
}