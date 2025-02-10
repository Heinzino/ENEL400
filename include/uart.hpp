#pragma once

#include "driver/uart.h"
#include "Arduino.h"
#include "esp_log.h"
#include <string.h>  // For strtok and string manipulation
#include <stdlib.h>  // For atof

#define UART_NUM UART_NUM_2
#define UART_TX_PIN 17       
#define UART_RX_PIN 16       
#define BUF_SIZE 1024

void setupUART2();
void readUART2(float *voltage, float *current);



