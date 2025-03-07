#pragma once

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
extern SemaphoreHandle_t uart_data_ready_semaphore;
