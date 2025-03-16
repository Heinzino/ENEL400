#include "HealthMetricsScreen.hpp"

void HealthMetricsScreen::updateScreen() {
    static int last_calories = -1, last_cadence = -1;
    static float last_speed = -1, last_distance = -1;

    if (calories != last_calories) {
        static char calories_str[16];
        snprintf(calories_str, sizeof(calories_str), "%d", calories);
        lv_label_set_text(ui_CaloriesValue, calories_str);
        last_calories = calories;
    }

    if (cadence != last_cadence) {
        static char cadence_str[16];
        snprintf(cadence_str, sizeof(cadence_str), "%d", cadence);
        lv_label_set_text(ui_CadenceValue, cadence_str);
        last_cadence = cadence;
    }

    if (fabs(speed - last_speed) > 0.1) {
        static char speed_str[16];
        snprintf(speed_str, sizeof(speed_str), "%.1f", speed);
        lv_label_set_text(ui_SpeedValue, speed_str);
        last_speed = speed;
    }

    if (fabs(distance - last_distance) > 0.1) {
        static char distance_str[16];
        snprintf(distance_str, sizeof(distance_str), "%.1f", distance);
        lv_label_set_text(ui_SpeedValue1, distance_str); // ui_SpeedValue1 is actually the Distance Label
        last_distance = distance;
    }

    lv_timer_handler();
    lv_task_handler();
    safeLvglRefresh();
}

void HealthMetricsScreen::handleButton(ButtonID btn) {
    switch (btn) {
    case ButtonID::SHIFT_HRZN_BTN:
        ScreenManager::getInstance().safeSwitchToScreen(ScreenTitles::POWER_DISPLAY, ui_Screen1);
        break;
    default:
        break;
    }
}

void HealthMetricsScreen::setCalories(int cals) {
    calories = cals;
}

void HealthMetricsScreen::setCadence(int cad) {
    cadence = cad;
}

void HealthMetricsScreen::setSpeed(float spd) {
    speed = spd;
}

void HealthMetricsScreen::setDistance(float dist) {
    distance = dist;
}
