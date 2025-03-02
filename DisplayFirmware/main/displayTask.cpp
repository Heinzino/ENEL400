#include "displayTask.hpp"

void update_ui()
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

    lv_arc_set_value(ui_Arc3, (int)(voltage * 100 / 3.3));
    lv_arc_set_value(ui_CurrentArc, (int)(current * 100 / 5.0));
    lv_arc_set_value(ui_Arc2, (int)(power * 100 / (3.3 * 5.0)));

    lv_refr_now(NULL);
}

void displayTask(void *pvParameters)
{

    while (1)
    {
        if (xSemaphoreTake(uart_data_ready_semaphore, pdMS_TO_TICKS(DISPLAY_SCREEN_TIMEOUT_MS)) == pdTRUE)
        {
            digitalWrite(TFT_SCREEN_LED, HIGH);
            update_ui();
        }
        else
        {
            digitalWrite(TFT_SCREEN_LED, LOW);
        }
    }
}
