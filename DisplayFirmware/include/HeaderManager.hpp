#ifndef TIME_MANAGER_HPP
#define TIME_MANAGER_HPP

#include <time.h>
#include "lvgl.h"
#include "logging.hpp"
#include "WiFi.h"
#include "secrets.hpp"
#include "Screens/SettingsScreen.hpp"

void initWiFi();

class HeaderManager {
public:
    static HeaderManager& getInstance();

    void update();  // Call this in displayTask every loop

    void updateWorkoutTimer(uint32_t delta);
    void updateRealTime(uint32_t delta);

    void syncNTP(); // Syncs time with NTP (every hour)

    void resetWorkoutTime(); // Resets elapsed time to 0

    const char* getElapsedTime();  // Returns "MM:SS" string
    const char* getFormattedTime(); // Returns "HH:MM | Month Day, Year"

    void toggleWorkoutTimer();
    bool isWorkoutPaused() const;



private:
    HeaderManager(); // Private constructor (singleton)
    
    bool workoutPaused = false;
    uint32_t lastUpdate = 0;
    int elapsedSeconds = 0;
    time_t currentTime;
    char elapsedTimeStr[16];  // "MM:SS"
    char formattedTimeStr[32]; // "HH:MM | Month Day, Year"
};

#endif // TIME_MANAGER_HPP
