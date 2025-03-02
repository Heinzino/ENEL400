#pragma once

#include <WiFi.h>
#include <time.h>
#include "sd_log.hpp"
#include "secrets.hpp"

const unsigned long NTP_UPDATE_INTERVAL = 3600000; // Update NTP every hour
unsigned long lastNTPCall = 0;

void WiFiSetup();
