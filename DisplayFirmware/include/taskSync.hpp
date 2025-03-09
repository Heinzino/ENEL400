#pragma once

#include "logging.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "screenManager.hpp"

enum ScreenTitles{
    POWER_DISPLAY,
    RESISTANCE_LEVEL,
};

extern float voltage, current;
extern QueueHandle_t uartQueue;
extern TaskHandle_t displayTaskHandle;