#include "TemperatureWarningScreen.hpp"

void TemperatureWarningScreen::updateScreen()
{
    SystemStatus &systemStatus = SystemStatus::getInstance();
    if (systemStatus.getHighTemperatureFlag()) {
        // Still too hot: hide "safe to proceed" message
        lv_obj_add_flag(ui_SafeProceedMessage, LV_OBJ_FLAG_HIDDEN);
    } else {
        // Safe to proceed
        lv_obj_clear_flag(ui_SafeProceedMessage, LV_OBJ_FLAG_HIDDEN);

        if (pendingSwitchToHomeScreen) {
            pendingSwitchToHomeScreen = false;
            ScreenManager::getInstance().safeSwitchToScreen(ScreenTitles::POWER_DISPLAY, ui_Screen1);
            return;
        }
    }

    lv_task_handler();
    lv_timer_handler();
    safeLvglRefresh();
}

void TemperatureWarningScreen::handleButton(ButtonID btn)
{

    SystemStatus &systemStatus = SystemStatus::getInstance();
    if (systemStatus.getHighTemperatureFlag()) {
        // Ignore input if still too hot
        return;
    }

    // Once safe, any button press should initiate screen switch
    pendingSwitchToHomeScreen = true;
}
