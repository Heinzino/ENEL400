#ifndef RESISTANCE_SCREEN_HPP
#define RESISTANCE_SCREEN_HPP

#include "Screen.hpp"
#include "../ScreenManager.hpp"

class ResistanceScreen : public Screen {
public:
    void updateScreen() override;
    void handleButton(ButtonID btn) override;

    void incrementResistance();
    void decrementResistance();
    char resistanceLevelToChar();  

private:
    uint8_t bikerResistanceLevel = 0;
    static constexpr uint8_t MAX_BIKER_RESISTANCE_LEVEL = 9;
};

#endif // RESISTANCE_SCREEN_HPP
