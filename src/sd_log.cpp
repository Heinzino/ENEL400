#include "sd_log.hpp"
#include <SD.h>
#include <SPI.h>
#include <WiFi.h>
#include <time.h>
#include "User_Setup.h"

#define GMT_OFFSET_SEC (-7 * 3600)  // UTC-7 hours in seconds
#define DAYLIGHT_OFFSET_SEC (3600)  // Add 1 hour (3600 sec) for daylight savings

// Global timekeeping variables:

// ntp_last_time holds the epoch time (in seconds) from the last full NTP update.
static time_t ntp_last_time = 0;

// ntp_last_update_millis holds the millis() count when that NTP update occurred.
static unsigned long ntp_last_update_millis = 0;

// current_ntp_time holds the latest known NTP time.
// (It will be updated by a direct call to getLocalTime() in updateSDLog().)
static time_t current_ntp_time = 0;

// Used to schedule log entries.
static unsigned long lastLogTime = 0;

SPIClass SPI_SD(VSPI);   // Use the alternate SPI bus (VSPI)

void initSDLog() {
  // Initialize the SD card on SPI using our dedicated CS pin.

  delay(1000);
  SPI_SD.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);

  // Initialize SD card using VSPI bus
  if (!SD.begin(SD_CS, SPI_SD)) {
    Serial.println("SD Card Mount Failed");
  } else {
    Serial.println("SD Card Initialized");
  }

  
  lastLogTime = millis();
  // Perform an initial NTP update.
  updateNTPTime();
}

void updateNTPTime() {
  // Configure time with GMT offset and daylight savings offset as needed.
  // Adjust these parameters and NTP servers as appropriate.
  configTime(GMT_OFFSET_SEC, DAYLIGHT_OFFSET_SEC, "pool.ntp.org", "time.nist.gov");

  // Wait (up to ~5 seconds) for time to be set.
  const int maxRetries = 10;
  int retry = 0;
  struct tm timeinfo;
  while (!getLocalTime(&timeinfo) && retry < maxRetries) {
    delay(500);
    retry++;
  }
  
  if (retry >= maxRetries) {
    Serial.println("Failed to obtain NTP time");
    return;
  }
  
  // Set the baseline NTP time and record when it was fetched.
  ntp_last_time = mktime(&timeinfo);
  ntp_last_update_millis = millis();
  current_ntp_time = ntp_last_time;
  
  Serial.print("NTP Time Updated: ");
  Serial.println(ntp_last_time);
}

void updateSDLog() {
  // Check if one minute (60,000 ms) has passed.
  if (millis() - lastLogTime >= 60000) {
    lastLogTime = millis();
    
    // Calculate the “ESP time” based on the last NTP update plus elapsed seconds.
    unsigned long elapsed_seconds = (millis() - ntp_last_update_millis) / 1000;
    time_t esp_time = ntp_last_time + elapsed_seconds;
    
    // **Get the current NTP time at the moment of logging.**
    // This fetches the actual network time (if available) to compare against the ESP time.
    struct tm fresh_tm;
    time_t fresh_ntp_time = 0;
    if (getLocalTime(&fresh_tm)) {
      fresh_ntp_time = mktime(&fresh_tm);
      current_ntp_time = fresh_ntp_time; // update our global if desired
    } else {
      // If unable to get a fresh value, fall back to the last known NTP time.
      fresh_ntp_time = current_ntp_time;
    }
    
    // Extract minutes and seconds (within the current hour) for each time.
    uint8_t esp_hours = (esp_time / 3600) % 24;
    uint8_t esp_minutes = (esp_time / 60) % 60;
    uint8_t ntp_hours = (fresh_ntp_time / 3600) % 24;
    uint8_t ntp_minutes = (fresh_ntp_time / 60) % 60;
    
    // Format the log string.
    char logStr[150];
    snprintf(logStr, sizeof(logStr),
             "I think it is %02d:%02d but it is actually %02d:%02d ; Maundy has done 10 sets of bouncing",
             esp_hours, esp_minutes, ntp_hours, ntp_minutes);

    // Open the log file in append mode.
    File logFile = SD.open("/log.txt", FILE_APPEND, true);
    if (logFile) {
      logFile.println(logStr);
      logFile.close();
      Serial.println("Logged:");
      Serial.println(logStr);
    } else {
      Serial.println("Error opening log file");
    }
  }
}
