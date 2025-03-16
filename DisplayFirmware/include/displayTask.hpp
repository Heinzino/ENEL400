#pragma once 

#include "main.hpp"
#include "taskSync.hpp"

#define DISPLAY_SCREEN_TIMEOUT_MS 15000

void safeLvglRefresh(unsigned long minIntervalMs = 10);
void displayTask(void* pvParameters);
void updateScreenSetup();
