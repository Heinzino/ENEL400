#pragma once

#include "freertos/semphr.h"
#include "freertos/queue.h"

extern float voltage, current;
extern QueueHandle_t uartQueue;
extern SemaphoreHandle_t uart_data_ready_semaphore;
