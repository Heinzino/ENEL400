#ifndef TIME_MANAGER_HPP
#define TIME_MANAGER_HPP

#include <time.h>
#include "lvgl.h"
#include "logging.hpp"
#include "WiFi.h"
#include "secrets.hpp"
#include "Screens/SettingsScreen.hpp"

void initWiFi();

class TimeManager {
public:
    static TimeManager& getInstance();

    void update();  // Call this in displayTask every loop
    void syncNTP(); // Syncs time with NTP (every hour)

    void resetWorkoutTime(); // Resets elapsed time to 0

    const char* getElapsedTime();  // Returns "MM:SS" string
    const char* getFormattedTime(); // Returns "HH:MM | Month Day, Year"

private:
    TimeManager(); // Private constructor (singleton)
    
    uint32_t lastUpdate = 0;
    int elapsedSeconds = 0;
    time_t currentTime;
    char elapsedTimeStr[16];  // "MM:SS"
    char formattedTimeStr[32]; // "HH:MM | Month Day, Year"
};

#endif // TIME_MANAGER_HPP
