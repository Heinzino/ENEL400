#pragma once

#include "pins.hpp"
#include "taskSync.hpp"
#include <Arduino.h>

#define SHORT_PRESS_TIME_MS 100 // 1000 milliseconds
#define LONG_PRESS_TIME 5000     // 1000 milliseconds

// Struct to represent a button
struct Button
{
  int pin;                    // GPIO pin for the button
  int lastState;              // previous state of the button
  int currentState;           // current state of the button
  unsigned long pressedTime;  // time when button was pressed
  unsigned long releasedTime; // time when button was released
};

enum ButtonID
{
  SHIFT_HRZN_BTN = 0,
  SHIFT_VERT_BTN = 1,
  FN1_BTN = 2,
  FN2_BTN = 3
};

extern Button buttons[4];
extern TaskHandle_t buttonTaskHandle;

void IRAM_ATTR buttonISRHandler(void *arg);
void buttonSetup();
void buttonTask(void *pvParameters);

// Helper Functions
void handlePowerDisplayButtons(ButtonID btn, ScreenManager &screenManager);
void handleResistanceLevelButtons(ButtonID btn, ScreenManager &screenManager);