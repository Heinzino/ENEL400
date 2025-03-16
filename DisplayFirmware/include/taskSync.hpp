#pragma once

#include "logging.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "screenManager.hpp"

extern float voltage, current;
extern QueueHandle_t uartQueue;
extern TaskHandle_t displayTaskHandle;
extern SemaphoreHandle_t lvglMutex;
