#include "ResistanceScreen.hpp"

void ResistanceScreen::updateScreen()
{
    LOG(LOG_LEVEL_TRACE, "Current Resistance Level: " + String(resistanceLevelToChar()));
    char levelStr[2] = {resistanceLevelToChar(), '\0'}; // Convert char to null-terminated string
    lv_label_set_text(ui_LEVELVAL, levelStr);
    safeLvglRefresh();
}

void ResistanceScreen::handleButton(ButtonID btn)
{
    switch (btn)
    {
    case SHIFT_HRZN_BTN:
        ScreenManager::getInstance().safeSwitchToScreen(ScreenTitles::POWER_DISPLAY, ui_Screen1);
        break;
    case FN1_BTN:
        incrementResistance();
        break;
    case FN2_BTN:
        decrementResistance();
        break;
    default:
        break;
    }
    // ScreenManager::getInstance().display();
}

void ResistanceScreen::incrementResistance()
{
    if (bikerResistanceLevel < MAX_BIKER_RESISTANCE_LEVEL)
        bikerResistanceLevel++;
}

void ResistanceScreen::decrementResistance()
{
    if (bikerResistanceLevel > 0)
        bikerResistanceLevel--;
}

char ResistanceScreen::resistanceLevelToChar() {
    return '0' + bikerResistanceLevel;  // Convert number (0-9) to ASCII character
}