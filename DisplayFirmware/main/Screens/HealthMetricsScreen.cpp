#include "HealthMetricsScreen.hpp"

//NOTE: Refactor this to a queue for button presses
bool pendingSwitchToPowerDisplay = false;

 void HealthMetricsScreen::updateScreen() {

    if(systemHighTempState()){
        pendingSwitchToPowerDisplay = false;
        ScreenManager::getInstance().safeSwitchToScreen(ScreenTitles::TEMP_WARNING, ui_Screen5);
        return; // Exit early to prevent chart update
    }

    if(pendingSwitchToPowerDisplay){
        pendingSwitchToPowerDisplay = false;
        ScreenManager::getInstance().safeSwitchToScreen(ScreenTitles::POWER_DISPLAY, ui_Screen1);
        return; // Exit early to prevent chart update
    }

    if (goToSpotify) {
        goToSpotify = false;
        ScreenManager::getInstance().safeSwitchToScreen(ScreenTitles::SPOTIFY, ui_SpotifyScreen);
        return; // Exit early to prevent chart update
    }

    SensorData &sensorData = SensorData::getInstance();

    // Fetch current values from SensorData
    int current_calories = sensorData.getCalories();
    int current_cadence = sensorData.getCadence();
    float current_speed = sensorData.getSpeed();
    float current_distance = sensorData.getDistance();

    static int last_calories = -1, last_cadence = -1;
    static float last_speed = -1, last_distance = -1;

    // Update Calories Label
    if (current_calories != last_calories) {
        static char calories_str[16];
        snprintf(calories_str, sizeof(calories_str), "%d", current_calories);
        lv_label_set_text(ui_CaloriesValue, calories_str);
        last_calories = current_calories;
    }

    // Update Cadence Label
    if (current_cadence != last_cadence) {
        static char cadence_str[16];
        snprintf(cadence_str, sizeof(cadence_str), "%d", current_cadence);
        lv_label_set_text(ui_CadenceValue, cadence_str);
        last_cadence = current_cadence;
    }

    // Update Speed Label (only if change is greater than 0.1 km/h)
    if (fabs(current_speed - last_speed) > 0.1) {
        static char speed_str[16];
        snprintf(speed_str, sizeof(speed_str), "%.1f", current_speed);
        lv_label_set_text(ui_SpeedValue, speed_str);
        last_speed = current_speed;
    }

    // Update Distance Label (only if change is greater than 0.1 km)
    if (fabs(current_distance - last_distance) >= 0.01) {
        static char distance_str[16];
        snprintf(distance_str, sizeof(distance_str), "%.2f", current_distance);
        lv_label_set_text(ui_SpeedValue1, distance_str); // ui_SpeedValue1 is actually the Distance Label
        last_distance = current_distance;
    }

    lv_timer_handler();
    lv_task_handler();
    safeLvglRefresh();
}

void HealthMetricsScreen::handleButton(ButtonID btn) {
    switch (btn) {
    case ButtonID::SHIFT_HRZN_BTN:
        pendingSwitchToPowerDisplay = true;
        break;
    case ButtonID::FN1_BTN:
        HeaderManager::getInstance().toggleWorkoutTimer();
        break;
    case ButtonID::FN2_BTN:
        goToSpotify = true;
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
