#ifndef LV_CONF_H
#define LV_CONF_H

/*====================
   COLOR CONFIG
 ====================*/
#define LV_COLOR_DEPTH 16
#define LV_COLOR_16_SWAP 0
#define LV_COLOR_SCREEN_TRANSP 0

/*====================
   MEMORY
 ====================*/
#define LV_MEM_CUSTOM 1
#define LV_MEM_SIZE (48U * 1024U)
#define LV_MEMCPY_MEMSET_STD 1
#define LV_MEMCPY_USE_DMA 1

/*====================
   DISLAY & RENDER
 ====================*/
#define LV_USE_OBJ_RENDER_ALWAYS 0
#define LV_USE_PARALLEL_DRAW 1
#define LV_USE_OBJ_CACHE 1
#define LV_USE_LAYER_CACHE 1
#define LV_DRAW_LAYER_FIFO_SIZE 2048
#define LV_DRAW_SW_COMPLEXITY_LIMIT 100

#define LV_USE_SHADOW 0
#define LV_USE_OUTLINE 0
#define LV_USE_GRADIENT 0
#define LV_USE_BLUR 0

#define LV_USE_GPU 1
#define LV_USE_DRAW_SW 1  /* CPU renderer */

/*====================
   PERFORMANCE
 ====================*/
#define LV_DISP_DEF_REFR_PERIOD 20      // 50 FPS
#define LV_INDEV_DEF_READ_PERIOD 20
#define LV_LOG_LEVEL LV_LOG_LEVEL_NONE
#define LV_USE_LOG 0

/*====================
    FONT & TEXT
 ====================*/
#define LV_USE_FONT_SUBPX 1
#define LV_FONT_MONTSERRAT_14 1
#define LV_FONT_MONTSERRAT_20 1
#define LV_FONT_DEFAULT &lv_font_montserrat_14

#define LV_USE_PNG 0
#define LV_USE_SJPG 0
#define LV_USE_BMP 0
#define LV_USE_TINY_TTF 0

/*====================
   WIDGETS
 ====================*/
#define LV_USE_BTN 1
#define LV_USE_LABEL 1
#define LV_USE_SWITCH 1
#define LV_USE_SLIDER 0
#define LV_USE_BAR 0
#define LV_USE_TABLE 0
#define LV_USE_ARC 0
#define LV_USE_CANVAS 0
#define LV_USE_CHART 0
#define LV_USE_LIST 0
#define LV_USE_CALENDAR 0
#define LV_USE_TA 1      // TextArea ON if needed

/*====================
   ANIMATION & SCROLL
 ====================*/
#define LV_USE_ANIMATION 1
#define LV_USE_SCROLL 1

/*====================
   THEMES
 ====================*/
#define LV_USE_THEME_DEFAULT 1
#define LV_THEME_DEFAULT_DARK 1
#define LV_THEME_DEFAULT_GROW 0   // save CPU

/*====================
   DEBUG
 ====================*/
#define LV_USE_ASSERT_MEM 0
#define LV_USE_ASSERT_NULL 0
#define LV_USE_ASSERT_OBJ 0

#endif /* LV_CONF_H */
