#ifndef PINS_H
#define PINS_H

#include "driver/uart.h"

#if defined(ESP32S3) //PCB / Release Version
#define BTN1 4
#define BTN2 5
#define BTN3 18
#define BTN4 8

#define TFT_SCREEN_CS 1
#define TFT_SCREEN_RESET 2
#define TFT_SCREEN_DC 42
#define TFT_SCREEN_LED 41
#define TFT_CLK 40
#define TOUCH_SCREEN_CS 39
#define TFT_MOSI 21
#define TFT_MISO 14
#define TOUCH_INTERRUPT_SIGNAL 9

#define SD_CARD_CS 13
#define SD_CARD_CLK 12
#define SD_CARD_MOSI 11
#define SD_CARD_MISO 10

#define ESP32_UART2_TX 6
#define ESP32_UART2_RX 7

#define ESP_USER_LED 15

#else //Devkit / Breadboard Version

#define BTN1 4
#define BTN2 5
#define BTN3 18
#define BTN4 8

#define TFT_SCREEN_CS 15
#define TFT_SCREEN_RESET 4
#define TFT_SCREEN_DC 2
#define TFT_SCREEN_LED 21
#define TFT_CLK 18
#define TOUCH_SCREEN_CS 39
#define TFT_MOSI 23
#define TFT_MISO 19
#define TOUCH_INTERRUPT_SIGNAL 9

#define SD_CARD_CS 5
#define SD_CARD_CLK 14
#define SD_CARD_MOSI 13
#define SD_CARD_MISO 27

#define ESP32_UART2_TX 17
#define ESP32_UART2_RX 16

#endif

#endif // PINS_H
