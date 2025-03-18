#ifndef LV_CONF_H
#define LV_CONF_H

/* Enable LVGL modules */
// #define LV_USE_THEME_DEFAULT 1
#define LV_USE_LOG 0  // Disable logging for now

// #define LV_FONT_MONTSERRAT_18  1
// #define LV_FONT_MONTSERRAT_40  1


/* Screen resolution */
#define LV_HOR_RES_MAX 320
#define LV_VER_RES_MAX 480

/* Enable color depth */
// #define LV_COLOR_DEPTH 16

/* Enable display buffering */
#define LV_USE_PERF_MONITOR 0

#endif // LV_CONF_H