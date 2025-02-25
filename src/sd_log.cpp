#include "sd_log.hpp"

#define GMT_OFFSET_SEC (-7 * 3600) // UTC-7 hours in seconds
#define DAYLIGHT_OFFSET_SEC (3600) // Add 1 hour (3600 sec) for daylight savings

// Global timekeeping variables:

// ntp_last_time_t holds the epoch time (in seconds) from the last full NTP update.
static struct tm ntp_last_time_tm_struct;
static time_t ntp_last_time_t = 0;

// ntp_last_update_millis holds the millis() count when that NTP update occurred.
static unsigned long ntp_last_update_millis = 0;

// current_ntp_time_t holds the latest known NTP time.
// (It will be updated by a direct call to getLocalTime() in updateSDLog().)
static struct tm current_ntp_time_tm_struct;
static time_t current_ntp_time_t = 0;

// Used to schedule log entries.
static unsigned long lastLogTime = 0;

#ifdef ESP32S3
SPIClass SPI_SD(SPI2_HOST); // Use the alternate SPI bus (VSPI)
#else
SPIClass SPI_SD(VSPI); // Use the alternate SPI bus (VSPI)
#endif 

void initSDLog()
{
  // Initialize the SD card on SPI using our dedicated CS pin.

  delay(1000);
  SPI_SD.begin(SD_CARD_CLK, SD_CARD_MISO, SD_CARD_MOSI, SD_CARD_CS);

  // Initialize SD card using VSPI bus
  if (!SD.begin(SD_CARD_CS, SPI_SD))
  {
    Serial.println("SD Card Mount Failed");
  }
  else
  {
    Serial.println("SD Card Initialized");
  }

  lastLogTime = millis();
  // Perform an initial NTP update.
  updateNTPTime();
}

void updateNTPTime()
{
  // Configure time with GMT offset and daylight savings offset as needed.
  // Adjust these parameters and NTP servers as appropriate.
  configTime(GMT_OFFSET_SEC, DAYLIGHT_OFFSET_SEC, "pool.ntp.org", "time.nist.gov");

  // Wait (up to ~5 seconds) for time to be set.
  const int maxRetries = 10;
  int retry = 0;
  while (!getLocalTime(&current_ntp_time_tm_struct) && retry < maxRetries)
  {
    delay(500);
    retry++;
  }

  if (retry >= maxRetries)
  {
    Serial.println("Failed to obtain NTP time");
    return;
  }

  ntp_last_time_tm_struct = current_ntp_time_tm_struct;
  ntp_last_update_millis = millis();
  Serial.print("NTP Time Updated");
}

void updateSDLog()
{
  // Check if one minute (60,000 ms) has passed.
  if (millis() - lastLogTime >= 60000)
  {
    lastLogTime = millis();

    // Calculate the “ESP time” based on the last NTP update plus elapsed seconds.
    unsigned long elapsed_seconds = (millis() - ntp_last_update_millis) / 1000.0;
    time_t esp_time = mktime(&ntp_last_time_tm_struct) + elapsed_seconds;
    struct tm esp_time_struct;
    localtime_r(&esp_time, &esp_time_struct);

    // **Get the current NTP time at the moment of logging.**
    // This fetches the actual network time (if available) to compare against the ESP time. FOR TESTING; DELETE LATER
    struct tm fresh_tm;
    time_t fresh_ntp_time = 0;
    if (!getLocalTime(&fresh_tm))
    {
      fresh_ntp_time = current_ntp_time_t;
    }

    // Extract date, month, year, hours, and minutes from both structs
    // NOTE: DEFINETLY A BETTER WAY TO DO THIS
    int esp_day = esp_time_struct.tm_mday;
    int esp_month = esp_time_struct.tm_mon + 1;    // tm_mon is 0-based (0 = Jan, 1 = Feb, etc.)
    int esp_year = esp_time_struct.tm_year + 1900; // tm_year is years since 1900
    int esp_hours = esp_time_struct.tm_hour;
    int esp_minutes = esp_time_struct.tm_min;

    int ntp_day = fresh_tm.tm_mday;
    int ntp_month = fresh_tm.tm_mon + 1;
    int ntp_year = fresh_tm.tm_year + 1900;
    int ntp_hours = fresh_tm.tm_hour;
    int ntp_minutes = fresh_tm.tm_min;

    // Format the log string.
    char logStr[200];
    snprintf(logStr, sizeof(logStr),
             "I think it is %02d-%02d-%04d %02d:%02d but it is actually %02d-%02d-%04d %02d:%02d ; Maundy has done 10 sets of bouncing",
             esp_day, esp_month, esp_year, esp_hours, esp_minutes,
             ntp_day, ntp_month, ntp_year, ntp_hours, ntp_minutes);

    // Open the log file in append mode.
    File logFile = SD.open("/log.txt", FILE_APPEND, true);
    if (logFile)
    {
      logFile.println(logStr);
      logFile.close();
      Serial.println("Logged:");
      Serial.println(logStr);
    }
    else
    {
      Serial.println("Error opening log file");
    }
  }
}
