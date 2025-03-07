#pragma once

#include <stdint.h>
#include <cstdio> //for sprintf
#include "taskSync.hpp"
#include "displayTask.hpp"

class ScreenManager {
public:
    static ScreenManager& getInstance(); // Singleton accessor

    void display();

    uint8_t getScreenNumber();
    void toggleScreen();

    uint8_t getResistanceLevel();
    void incrementResistance();
    void decrementResistance();
    char* resistanceLevelToString();

private:
    ScreenManager() = default; // Private constructor to prevent multiple instances

    const uint8_t MAX_BIKER_RESISTANCE_LEVEL = 10;
    uint8_t screenNumber = 0;         // Default to POWER_DISPLAY
    uint8_t bikerResistanceLevel = 0; // Initial resistance level
};

