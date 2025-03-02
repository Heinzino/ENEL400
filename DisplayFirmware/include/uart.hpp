#pragma once

#include "driver/uart.h"
#include "Arduino.h"
#include "esp_log.h"
#include <string.h>  // For strtok and string manipulation
#include <stdlib.h>  // For atof
#include "pins.hpp"

#define UART_NUM UART_NUM_2
#define BUF_SIZE 1024
#define READ_UART_DELAY_MS 10 

void setupUART2();
void readUART2(float *voltage, float *current);


