#pragma once

#include <WiFi.h>
#include <time.h>
#include "sd_log.hpp"
#include "secrets.hpp"

extern const unsigned long NTP_UPDATE_INTERVAL; // Update NTP every hour
extern unsigned long lastNTPCall;

void WiFiSetup();
