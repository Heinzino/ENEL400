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

void updateScreen1()
{
    static float last_voltage = -1, last_current = -1, last_power = -1;
    float power = voltage * current;

    if (fabs(voltage - last_voltage) > 0.02)
    {
        static char voltage_str[16];
        snprintf(voltage_str, sizeof(voltage_str), "%.2f V", voltage);
        lv_label_set_text(ui_Label6, voltage_str);
        last_voltage = voltage;
    }

    if (fabs(current - last_current) > 0.02)
    {
        static char current_str[16];
        snprintf(current_str, sizeof(current_str), "%.2f A", current);
        lv_label_set_text(ui_Label5, current_str);
        last_current = current;
    }

    if (fabs(power - last_power) > 0.02)
    {
        static char power_str[16];
        snprintf(power_str, sizeof(power_str), "%.2f W", power);
        lv_label_set_text(ui_Label1, power_str);
        last_power = power;
    }

    lv_arc_set_value(ui_Arc3, (int)(voltage * 100 / MAX_VOLTAGE));
    lv_arc_set_value(ui_CurrentArc, (int)(current * 100 / MAX_CURRENT));
    lv_arc_set_value(ui_Arc2, (int)(power * 100 / (MAX_POWER)));

    lv_timer_handler();
    lv_task_handler();
    safeLvglRefresh();
}

void updateScreen2()
{
    ScreenManager &screenManager = ScreenManager::getInstance();
    LOG(LOG_LEVEL_TRACE, "Current Resistance Level: " + String(screenManager.resistanceLevelToString()));
    lv_label_set_text(ui_LEVELVAL, screenManager.resistanceLevelToString());
    safeLvglRefresh();
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
                if(screenManager.getScreenNumber() == RESISTANCE_LEVEL){
                    shouldUpdateDisplay = false;
                }
                LOG(LOG_LEVEL_DEBUG, "UART Task Triggered Display");
            }

            if (!screenManager.isScreenOn())
            {
                screenManager.updateScreenState(ScreenState::ON);
            }
            updateScreenSetup();
            if(shouldUpdateDisplay){
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

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
