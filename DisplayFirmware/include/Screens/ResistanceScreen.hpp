#ifndef RESISTANCE_SCREEN_HPP
#define RESISTANCE_SCREEN_HPP

#include "Screen.hpp"
#include "screenManager.hpp"

enum class ButtonID; // Forward declare to avoid circular dependancy;
class ResistanceScreen : public Screen
{
public:
    void updateScreen() override;
    void handleButton(ButtonID btn) override;
    lv_obj_t *getScreen() override { return ui_Screen2; }

    void incrementResistance();
    void decrementResistance();
    char resistanceLevelToChar();

private:
    uint8_t bikerResistanceLevel = 0;
    static constexpr uint8_t MAX_BIKER_RESISTANCE_LEVEL = 9;
};

#endif // RESISTANCE_SCREEN_HPP
