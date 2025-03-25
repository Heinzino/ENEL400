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

void updateTimeHeader()
{
    TimeManager &timeManager = TimeManager::getInstance();
    timeManager.update();
    lv_label_set_text(elapsed_label, timeManager.getElapsedTime());
    lv_label_set_text(datetime_label, timeManager.getFormattedTime());
}

void updateScreenSetup()
{
    digitalWrite(TFT_SCREEN_LED, HIGH);
    lv_timer_handler();
    lv_task_handler();
}

bool handleNotifiedValue(uint32_t notifiedValue)
{
    ScreenManager &screenManager = ScreenManager::getInstance();

    if (notifiedValue & (1 << 0))
    {
        LOG(LOG_LEVEL_TRACE, "Button Task Triggered Display");
        xTaskNotifyStateClearIndexed(displayTaskHandle, 0); // Clear index 0 bits only
    }
    if (notifiedValue & (1 << 1))
    {
        LOG(LOG_LEVEL_TRACE, "UART Task Triggered Display");
    }

    return true;
}

void displayTask(void *pvParameters)
{
    create_time_header();
    while (1)
    {
        ScreenManager &screenManager = ScreenManager::getInstance();
        uint32_t notifiedValue;
        if (xTaskNotifyWait(0, ULONG_MAX, &notifiedValue, pdMS_TO_TICKS(DISPLAY_SCREEN_TIMEOUT_MS)) == pdTRUE)
        {
            handleNotifiedValue(notifiedValue);

            if (!screenManager.isScreenOn())
            {
                TimeManager::getInstance().resetWorkoutTime(); 
                screenManager.updateScreenState(ScreenState::ON);
            }

            updateTimeHeader();
            updateScreenSetup();

            ScreenManager::getInstance().display(); // Access LVGL safely
            LOG(LOG_LEVEL_DEBUG, "Updating UI");
            screenManager.display();
            
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
