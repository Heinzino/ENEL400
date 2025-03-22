#include "TimeManager.hpp"
#include "esp_log.h"
#include "lwip/apps/sntp.h"

void initWiFi()
{
    WiFi.begin(ssid, password);
    int retries = 0;
    const int maxRetries = 20;

    while (WiFi.status() != WL_CONNECTED && retries < maxRetries)
    {
        LOG(LOG_LEVEL_TRACE, "Connecting to WiFi...");
        delay(500);
        retries++;
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        LOG(LOG_LEVEL_DEBUG, "WiFi connected!");
        LOG(LOG_LEVEL_DEBUG, "SSID: " + String(ssid));
        LOG(LOG_LEVEL_DEBUG, "IP Address: " + WiFi.localIP().toString());
    }
    else
    {
        LOG(LOG_LEVEL_ERROR, "Failed to connect to WiFi.");
    }
}

TimeManager::TimeManager()
{
    currentTime = 0;
    snprintf(elapsedTimeStr, sizeof(elapsedTimeStr), "00:00");
    snprintf(formattedTimeStr, sizeof(formattedTimeStr), "00:00 | January 1, 1970");
}

TimeManager &TimeManager::getInstance()
{
    static TimeManager instance;
    return instance;
}

// Sync NTP once per hour
void TimeManager::syncNTP()
{
    LOG(LV_LOG_LEVEL_TRACE, "Syncing time with NTP...");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org");
    sntp_init();

    time_t now = 0;
    struct tm timeinfo = {0};
    int retry = 0;
    const int max_retries = 10;

    while (timeinfo.tm_year < (2020 - 1900) && ++retry < max_retries)
    {
        LOG(LOG_LEVEL_TRACE, "Waiting for NTP sync... (" + String(retry) + "/" + String(max_retries) + ")");
        vTaskDelay(pdMS_TO_TICKS(2000));
        time(&now);
        localtime_r(&now, &timeinfo);
    }

    if (retry == max_retries)
    {
        LOG(LOG_LEVEL_ERROR, "NTP sync failed!");
    }
    else
    {
        LOG(LOG_LEVEL_TRACE, "Time synced successfully!");
        currentTime = now;

        // Get timezone from SettingsScreen
        String timezone = SettingsScreen::getInstance().getTimezone();
        setenv("TZ", timezone.c_str(), 1); // Convert String to C-string
        tzset();

        // Verify the new local time
        time(&now);
        localtime_r(&now, &timeinfo);
        LOG(LOG_LEVEL_TRACE, "Local time: " + String(timeinfo.tm_hour) + ":" + String(timeinfo.tm_min)); // Get timezone from SettingsScreen
    }
}

void TimeManager::update()
{
    static uint32_t lastUpdate = 0;
    uint32_t now = millis();

    if (now - lastUpdate >= 1000)
    {
        uint32_t delta = (now - lastUpdate) / 1000;  // How many full seconds passed

        elapsedSeconds += delta;
        currentTime += delta;
        lastUpdate += delta * 1000;

        //  Update elapsed time string (MM:SS)
        snprintf(elapsedTimeStr, sizeof(elapsedTimeStr), "%02d:%02d", elapsedSeconds / 60, elapsedSeconds % 60);

        //  Update formatted real-time string (HH:MM | Month Day, Year)
        struct tm timeinfo;
        localtime_r(&currentTime, &timeinfo);

        const char *months[] = {"January", "February", "March", "April", "May", "June",
                                "July", "August", "September", "October", "November", "December"};
        snprintf(formattedTimeStr, sizeof(formattedTimeStr), "%02d:%02d | %s %d, %d",
                 timeinfo.tm_hour, timeinfo.tm_min, months[timeinfo.tm_mon], timeinfo.tm_mday, timeinfo.tm_year + 1900);

        //  Sync NTP every hour
        if (elapsedSeconds % 3600 == 0)
        {
            syncNTP();
        }
    }
}

//  Get elapsed workout time (MM:SS)
const char *TimeManager::getElapsedTime()
{
    return elapsedTimeStr;
}

//  Get formatted current time (HH:MM | Month Day, Year)
const char *TimeManager::getFormattedTime()
{
    return formattedTimeStr;
}

void TimeManager::resetWorkoutTime()
{
    elapsedSeconds = 0;
    snprintf(elapsedTimeStr, sizeof(elapsedTimeStr), "00:00");
}