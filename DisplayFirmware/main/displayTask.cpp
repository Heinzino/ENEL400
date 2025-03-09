#include "displayTask.hpp"

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
    lv_refr_now(NULL);
}


void updateScreen2(){
    ScreenManager& screenManager = ScreenManager::getInstance();
    lv_refr_now(NULL);
    lv_label_set_text(ui_LEVELVAL,screenManager.resistanceLevelToString());
}

void displayTask(void *pvParameters)
{
    while (1)
    {
        ScreenManager& screenManager = ScreenManager::getInstance();
        if (ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(DISPLAY_SCREEN_TIMEOUT_MS)) > 0 )
        {
            if(!screenManager.isScreenOn()){
                screenManager.updateScreenState(ScreenState::ON);
            }
            LOG(LOG_LEVEL_DEBUG, "Updating UI");
            digitalWrite(TFT_SCREEN_LED, HIGH);
            screenManager.display();
        }
        else
        {
            if(screenManager.isScreenOn()){
                LOG(LOG_LEVEL_DEBUG, "Turning Screen OFF");
                screenManager.updateScreenState(ScreenState::OFF);
                digitalWrite(TFT_SCREEN_LED, LOW);
            }
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
