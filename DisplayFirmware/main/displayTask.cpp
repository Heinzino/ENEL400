#include "displayTask.hpp"

// Safe refresh function with throttling
void safeLvglRefresh(unsigned long minIntervalMs)
{
    static unsigned long lastRefreshTime = 0;
    unsigned long currentTime = millis();

    if (currentTime - lastRefreshTime > minIntervalMs)
    {
        lv_refr_now(NULL); // Force immediate screen update
        lastRefreshTime = currentTime;
    }
}

void updateScreenSetup()
{
    digitalWrite(TFT_SCREEN_LED, HIGH);
    lv_timer_handler();
    lv_task_handler();
}

void displayTask(void *pvParameters)
{
    while (1)
    {
        ScreenManager &screenManager = ScreenManager::getInstance();
        uint32_t notifiedValue;
        if (xTaskNotifyWait(0, ULONG_MAX, &notifiedValue, pdMS_TO_TICKS(DISPLAY_SCREEN_TIMEOUT_MS)) == pdTRUE)
        {
            bool shouldUpdateDisplay = true;
            if (notifiedValue & (1 << 0))
            {
                LOG(LOG_LEVEL_DEBUG, "Button Task Triggered Display");
                xTaskNotifyStateClearIndexed(displayTaskHandle, 0); // Clear index 0 bits only
            }
            if (notifiedValue & (1 << 1))
            {
                if (screenManager.getScreenNumber() == RESISTANCE_LEVEL)
                {
                    shouldUpdateDisplay = false;
                }
                LOG(LOG_LEVEL_DEBUG, "UART Task Triggered Display");
            }

            if (!screenManager.isScreenOn())
            {
                screenManager.updateScreenState(ScreenState::ON);
            }
            updateScreenSetup();
            if (shouldUpdateDisplay)
            {
                ScreenManager::getInstance().display(); // Access LVGL safely
                LOG(LOG_LEVEL_DEBUG, "Updating UI");
                screenManager.display();
            }
        }
        else
        {
            if (screenManager.isScreenOn())
            {
                LOG(LOG_LEVEL_DEBUG, "Turning Screen OFF");
                screenManager.updateScreenState(ScreenState::OFF);
                digitalWrite(TFT_SCREEN_LED, LOW);
            }
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}
