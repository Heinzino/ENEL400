#pragma once

#include "driver/uart.h"
#include "Arduino.h"
#include "esp_log.h"
#include <string.h>  // For strtok and string manipulation
#include <stdlib.h>  // For atof
#include "pins.hpp"
#include "freertos/task.h"
#include "driver/uart.h"
#include "esp_intr_alloc.h"
#include "freertos/queue.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "taskSync.hpp"

#define UART_NUM UART_NUM_2
#define BUF_SIZE (1024)
#define READ_UART_DELAY_MS (10)

#define QUEUE_LENGTH (10)
#define RD_BUF_SIZE (BUF_SIZE)


void setupUART2();
void readUART2();
static void uart_event_task(void *pvParameters);


