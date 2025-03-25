#pragma once

#include "pins.hpp"
#include "taskSync.hpp"
#include <Arduino.h>
#include "screenManager.hpp"

class ScreenManager; // Forward declaration, used to avoid circular dependancies , clean later

#define SHORT_PRESS_TIME_MS 100 // 100 milliseconds
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

enum class ButtonID
{
  SHIFT_HRZN_BTN = 0,
  PLOT_BTN = 1,
  FN1_BTN = 2,
  FN2_BTN = 3
};

extern Button buttons[4];
extern TaskHandle_t buttonTaskHandle;

void buttonISRHandler(void *arg);
void buttonSetup();
void buttonTask(void *pvParameters);
