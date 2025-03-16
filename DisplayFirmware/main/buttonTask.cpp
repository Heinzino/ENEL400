#include "buttonTask.hpp"

Button buttons[4] = {{BTN1, LOW, LOW, 0, 0},
                     {BTN2, LOW, LOW, 0, 0},
                     {BTN3, LOW, LOW, 0, 0},
                     {BTN4, LOW, LOW, 0, 0}};

static portMUX_TYPE screenSwitchMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR buttonISRHandler(void *arg)
{
    Button *btn = (Button *)arg;
    // Notify button task about the button pin that triggered the ISR
    xTaskNotifyFromISR(buttonTaskHandle, (1 << btn->pin), eSetBits, NULL);
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
        LOG(LOG_LEVEL_DEBUG, "ENTERED Button Task");
        if (xTaskNotifyWait(0, ULONG_MAX, &notifiedValue, portMAX_DELAY) == pdTRUE)
        {
            for (int i = 0; i < 4; ++i)
            {
                if (!(notifiedValue & (1 << buttons[i].pin)))
                    continue; // Skip if button was not notified from ISR

                // Update button states directly in the struct
                buttons[i].lastState = buttons[i].currentState;
                buttons[i].currentState = digitalRead(buttons[i].pin);

                if (buttons[i].currentState == LOW && buttons[i].lastState == HIGH)
                {
                    // Button Pressed
                    buttons[i].pressedTime = millis();
                }
                else if (buttons[i].currentState == HIGH && buttons[i].lastState == LOW)
                {
                    // Button Released
                    buttons[i].releasedTime = millis();
                    long pressDuration = buttons[i].releasedTime - buttons[i].pressedTime;

                    if (pressDuration >= SHORT_PRESS_TIME_MS)
                    {
                        LOG(LOG_LEVEL_DEBUG, "BTN" + String(i + 1) + " short press detected");

                        screenManager.getCurrentScreenObject()->handleButton(static_cast<ButtonID>(i));

                        // Notify display task only if a short press was detected
                        screenManager.display();
                        xTaskNotify(displayTaskHandle, (1 << 0), eSetBits);
                    }
                }
            }
        }
    }
}
