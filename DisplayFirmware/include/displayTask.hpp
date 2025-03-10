#pragma once 

#include "main.hpp"
#include "taskSync.hpp"

#define DISPLAY_SCREEN_TIMEOUT_MS 15000

const float MAX_VOLTAGE = 40.0;
const float MAX_CURRENT = 20.0;
const float MAX_POWER   = MAX_VOLTAGE * MAX_CURRENT;

void safeLvglRefresh(unsigned long minIntervalMs = 10);
void updateScreen1();
void updateScreen2();
void displayTask(void* pvParameters);
