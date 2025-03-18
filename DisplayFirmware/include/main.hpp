#pragma once 

#include <Arduino.h>
#include "TFT_eSPI.h"
#include "User_Setup.h"
#include <lvgl.h>
#include "UI/ui.h"
#include "UI/lv_conf.h"
#include "esp_adc_cal.h"
#include "driver/adc.h"
#include "driver/uart.h"
#include "uart.hpp"
#include "secrets.hpp"
#include "Wifi.h"
#include "sd_log.hpp"
#include "pins.hpp"
#include "wifiTask.hpp"
#include "displayTask.hpp"
#include "buttonTask.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"