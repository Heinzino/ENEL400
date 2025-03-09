#pragma once

#include <Arduino.h>

#define LOG_LEVEL_TRACE 5
#define LOG_LEVEL_DEBUG 4
#define LOG_LEVEL_INFO  3
#define LOG_LEVEL_WARN  2
#define LOG_LEVEL_ERROR 1
#define LOG_LEVEL_NONE  0  // Disable logging

// Set your log level here (change this to control verbosity)
#define LOG_LEVEL LOG_LEVEL_DEBUG

#define LOG(level, msg) \
    do { \
        if (level <= LOG_LEVEL) { \
            Serial.print("["); \
            if (level == LOG_LEVEL_TRACE) Serial.print("TRACE"); \
            else if (level == LOG_LEVEL_DEBUG) Serial.print("DEBUG"); \
            else if (level == LOG_LEVEL_INFO) Serial.print("INFO"); \
            else if (level == LOG_LEVEL_WARN) Serial.print("WARN"); \
            else if (level == LOG_LEVEL_ERROR) Serial.print("ERROR"); \
            Serial.print("] "); \
            Serial.println(msg); \
        } \
    } while (0)
            