#include "buttonTask.hpp"

Button buttons[4] = {{BTN1, LOW, LOW, 0, 0},
                     {BTN2, LOW, LOW, 0, 0},
                     {BTN3, LOW, LOW, 0, 0},
                     {BTN4, LOW, LOW, 0, 0}};

void IRAM_ATTR buttonISRHandler(void *arg)
{
    Button *btn = (Button *)arg;
    int state = digitalRead(btn->pin);

    if (state == LOW)
    { // Button Pressed
        btn->pressedTime = millis();
    }
    else
    { // Button Released
        btn->releasedTime = millis();
        long pressDuration = btn->releasedTime - btn->pressedTime;

        if (pressDuration >= SHORT_PRESS_TIME_MS)
        {
            // Notify a task to handle the event
            xTaskNotifyFromISR(buttonTaskHandle, (1 << btn->pin), eSetBits, NULL);
        }
    }
}

void buttonSetup()
{
    for (int i = 0; i < 4; ++i)
    {
        pinMode(buttons[i].pin, INPUT);
        attachInterruptArg(buttons[i].pin, buttonISRHandler, (void *)&buttons[i], CHANGE);
    }
}

void buttonTask(void *pvParameters)
{
    ScreenManager &screenManager = ScreenManager::getInstance();
    uint32_t notifiedValue;

    while (true)
    {
        if (xTaskNotifyWait(0, ULONG_MAX, &notifiedValue, portMAX_DELAY) == pdTRUE)
        {
            for (int i = 0; i < 4; i++)
            {
                if (notifiedValue & (1 << buttons[i].pin)) // If the ISR notified this button
                {
                    Serial.print("BTN");
                    Serial.print(i + 1);
                    Serial.println(" short press detected");

                    switch (screenManager.getScreenNumber())
                    {
                    case POWER_DISPLAY:
                        handlePowerDisplayButtons(static_cast<ButtonID>(i), screenManager);
                        break;

                    case RESISTANCE_LEVEL:
                        handleResistanceLevelButtons(static_cast<ButtonID>(i), screenManager);
                        break;
                    }
                }
            }
        }
    }
}

void handleResistanceLevelButtons(ButtonID btn, ScreenManager &screenManager)
{
    switch (btn)
    {
    case SHIFT_HRZN_BTN:
        screenManager.toggleScreen();
        break;
    case FN1_BTN:
        screenManager.incrementResistance();
        break;
    case FN2_BTN:
        if (screenManager.getScreenNumber() == RESISTANCE_LEVEL)
            screenManager.decrementResistance();
        break;
    default:
        break;
    }
}

void handlePowerDisplayButtons(ButtonID btn, ScreenManager &screenManager)
{
    switch (btn)
    {
    case SHIFT_HRZN_BTN:
        screenManager.toggleScreen();
        break;
    default:
        break;
    }
}