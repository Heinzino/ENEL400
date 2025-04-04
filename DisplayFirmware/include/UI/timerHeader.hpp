#ifndef TIME_HEADER_HPP
#define TIME_HEADER_HPP

#include "lvgl.h"
#include "UI/ui.h"
#include "UI/bike_icon.h"
#include "UI/battery_icon.h"

void create_time_header(); // Initializes the UI header
extern lv_obj_t *elapsed_label;  // Reference to elapsed time label
extern lv_obj_t *datetime_label; // Reference to real-time label

#endif // TIME_HEADER_HPP
