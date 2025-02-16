#ifndef SD_LOG_H
#define SD_LOG_H

#include <Arduino.h>

// Define SPI pins for SD card (VSPI)
#define SD_SCK    14
#define SD_MISO   27
#define SD_MOSI   13
#define SD_CS     5 

// Initializes the SD card and performs an initial NTP update.
// Call this once (for example, in setup()).
void initSDLog();

// Checks if one minute has passed and, if so, logs a line to the SD card.
// The log string compares the “ESP time” (using millis() and the last known NTP time)
// with the current NTP time (queried directly at log time) in the format:
// "I think it is mm:ss but it is actually mm:ss ; Maundy has done 10 sets of bouncing"
void updateSDLog();

// Updates the NTP time by querying the NTP servers.
// This function sets the baseline for the ESP’s clock (ntp_last_time) and is normally
// called once at startup (or less frequently if desired).
void updateNTPTime();

#endif // SD_LOG_H
