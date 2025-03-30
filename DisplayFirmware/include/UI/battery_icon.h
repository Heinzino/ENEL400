#ifndef UI_IMG_BATTERYICON_PNG_H
#define UI_IMG_BATTERYICON_PNG_H

#include "lvgl.h"  // Required for lv_img_dsc_t
#include "UI/images.h"

#ifdef __cplusplus
extern "C" {
#endif

// Extern so other files can use this image descriptor
extern lv_img_dsc_t ui_img_batteryicon_png;

// Call this once before using `ui_img_bikeicon_png`
void init_battery_icon_image(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // UI_IMG_BATTERYICON_PNG_H
