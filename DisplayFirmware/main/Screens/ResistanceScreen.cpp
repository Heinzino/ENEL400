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
    case ButtonID::SHIFT_HRZN_BTN:
        ScreenManager::getInstance().safeSwitchToScreen(ScreenTitles::HEALTH_METRICS, ui_Screen3);
        break;
    case ButtonID::FN1_BTN:
        incrementResistance();
        sendResistanceLevelUART2(3, resistanceLevelToChar());
        break;
    case ButtonID::FN2_BTN:
        decrementResistance();
        sendResistanceLevelUART2(3, resistanceLevelToChar());
        break;
    default:
        break;
    }
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