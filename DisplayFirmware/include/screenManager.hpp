#pragma once

#include <stdint.h>
#include <cstdio> //for sprintf
#include "taskSync.hpp"
#include "displayTask.hpp"

enum class ScreenState {
    OFF = 0,
    ON = 1
};

enum ScreenTitles{
    POWER_DISPLAY,
    RESISTANCE_LEVEL,
};

class ScreenManager {
public:
    static ScreenManager& getInstance(); // Singleton accessor

    void display();

    void updateScreenState(ScreenState state);
    bool isScreenOn();

    void safeSwitchToScreen(ScreenTitles newScreen, lv_obj_t* lvglScreen);

    uint8_t getScreenNumber();
    void toggleScreen();

    uint8_t getResistanceLevel();
    void incrementResistance();
    void decrementResistance();
    char* resistanceLevelToString();

private:
    ScreenManager() = default; // Private constructor to prevent multiple instances

    const uint8_t MAX_BIKER_RESISTANCE_LEVEL = 9;
    ScreenState screenState = ScreenState::ON;
    ScreenTitles screenNumber = ScreenTitles::POWER_DISPLAY; // Default to POWER_DISPLAY
    uint8_t bikerResistanceLevel = 0; // Initial resistance level
};

