#ifndef TIME_TASK_HPP
#define TIME_TASK_HPP

#include "lvgl.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "lwip/apps/sntp.h"
#include <time.h>
#include "taskSync.hpp"

// Starts the time update task
void start_time_task();

// Syncs time with an NTP server
void sync_ntp_time();

// Updates the time every second and syncs NTP every hour
void time_task(void *pvParameter);

#endif // TIME_TASK_HPP
