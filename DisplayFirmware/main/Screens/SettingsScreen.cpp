#include "SettingsScreen.hpp"
#include "ScreenManager.hpp"
#include "lvgl.h"

SettingsScreen &SettingsScreen::getInstance() {
    static SettingsScreen instance;
    return instance;
}

void SettingsScreen::setWheelSize(float sizeInInches) {
    wheelSizeInInches = sizeInInches;
}

float SettingsScreen::getWheelSizeInInches() const {
    return wheelSizeInInches;
}

float SettingsScreen::getWheelSizeInMeters() const {
    return wheelSizeInInches * 0.0254;
}

void SettingsScreen::setGearRatio(float ratio) {
    gearRatio = ratio;
}

float SettingsScreen::getGearRatio() const {
    return gearRatio;
}

void SettingsScreen::updateScreen() {
    static char wheelSizeStr[16];
    snprintf(wheelSizeStr, sizeof(wheelSizeStr), "%.1f in", wheelSizeInInches);
    // lv_label_set_text(ui_WheelSizeValue, wheelSizeStr);

    static char gearRatioStr[16];
    snprintf(gearRatioStr, sizeof(gearRatioStr), "%.1f:1", gearRatio);
    // lv_label_set_text(ui_GearRatioValue, gearRatioStr);

    lv_timer_handler();
    lv_task_handler();
    safeLvglRefresh();
}

void SettingsScreen::handleButton(ButtonID btn) {
    switch (btn) {
    case ButtonID::SHIFT_HRZN_BTN:
        ScreenManager::getInstance().safeSwitchToScreen(ScreenTitles::POWER_DISPLAY, ui_Screen1);
        break;
    case ButtonID::FN1_BTN:
        setWheelSize(wheelSizeInInches + 1.0);
        updateScreen();
        break;
    case ButtonID::FN2_BTN:
        setWheelSize(wheelSizeInInches - 1.0);
        updateScreen();
        break;
    default:
        break;
    }
}
