#pragma once

#include <stdint.h>
#include <cstdio> //for sprintf
#include "taskSync.hpp"
#include "displayTask.hpp"
#include "Screens/Screen.hpp"
#include "Screens/PowerScreen.hpp"
#include "Screens/ResistanceScreen.hpp"
#include "Screens/HealthMetricsScreen.hpp"
#include "Screens/PlotScreen.hpp"
#include "Screens/TemperatureWarningScreen.hpp"
#include "Screens/SpotifyScreen.hpp"
#include "screenEnums.hpp"

class Screen; //Avoid circular dependency

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
    ScreenState screenState = ScreenState::OFF;
    ScreenTitles screenNumber = ScreenTitles::POWER_DISPLAY; // Default to POWER_DISPLAY
};

