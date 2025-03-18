#pragma once

#include <stdint.h>
#include <cstdio> //for sprintf
#include "taskSync.hpp"
#include "displayTask.hpp"
#include "Screens/Screen.hpp"
#include "Screens/PowerScreen.hpp"
#include "Screens/ResistanceScreen.hpp"
#include "Screens/HealthMetricsScreen.hpp"

class Screen; //Avoid circular dependency

enum class ScreenState {
    OFF = 0,
    ON = 1
};

enum ScreenTitles{
    POWER_DISPLAY,
    RESISTANCE_LEVEL,
    HEALTH_METRICS,
    SCREEN_COUNT
};

class ScreenManager {
public:
    static ScreenManager& getInstance(); // Singleton accessor

    void display();

    Screen* getCurrentScreenObject();
    void updateScreenState(ScreenState state);
    bool isScreenOn();

    void safeSwitchToScreen(ScreenTitles newScreen, lv_obj_t* lvglScreen);

    uint8_t getScreenNumber();

private:
    ScreenManager(); // Private constructor to prevent multiple instances
    Screen* screens[SCREEN_COUNT];
    ScreenState screenState = ScreenState::ON;
    ScreenTitles screenNumber = ScreenTitles::POWER_DISPLAY; // Default to POWER_DISPLAY
};

