#include "PowerScreen.hpp"

void PowerScreen::updateScreen()
{
    SensorData &sensorData = SensorData::getInstance();

    float voltage = sensorData.getVoltage();
    float current = sensorData.getCurrent();
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

    // Update LVGL UI elements
    lv_arc_set_value(ui_Arc3, (int)(voltage * 100 / MAX_VOLTAGE));
    lv_arc_set_value(ui_CurrentArc, (int)(current * 100 / MAX_CURRENT));
    lv_arc_set_value(ui_Arc2, (int)(power * 100 / MAX_POWER));

    lv_timer_handler();
    lv_task_handler();
    safeLvglRefresh();
}

void PowerScreen::handleButton(ButtonID btn)
{
    switch (btn)
    {
    case ButtonID::SHIFT_HRZN_BTN:
        ScreenManager::getInstance().safeSwitchToScreen(ScreenTitles::RESISTANCE_LEVEL, ui_Screen2);
        break;
    default:
        break;
    }
}

void PowerScreen::setPowerVoltage(float v, float c)
{
    voltage = v;
    current = c;
}
