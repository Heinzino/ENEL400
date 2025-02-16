#include <Arduino.h>
#include "TFT_eSPI.h"
#include "User_Setup.h"
#include <lvgl.h>
#include "ui.h"
#include "esp_adc_cal.h"
#include "driver/adc.h"
#include "driver/uart.h"
#include "uart.hpp"
#include "secrets.hpp"
#include "Wifi.h"
#include "sd_log.hpp"

TFT_eSPI tftDisplay = TFT_eSPI(); // TFT Instance
float voltage, current;


const unsigned long NTP_UPDATE_INTERVAL = 3600000; // Update NTP every hour
unsigned long lastNTPCall = 0;

void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  uint16_t c;

  tftDisplay.startWrite();
  tftDisplay.setAddrWindow(area->x1, area->y1, (area->x2 - area->x1 + 1), (area->y2 - area->y1 + 1));

  for (int y = area->y1; y <= area->y2; y++)
  {
    for (int x = area->x1; x <= area->x2; x++)
    {
      c = color_p->full; // Convert color
      tftDisplay.pushColor(c);
      color_p++;
    }
  }
  tftDisplay.endWrite();

  lv_disp_flush_ready(disp); // Tell LVGL that flushing is done
}

void update_ui()
{
    static float last_voltage = -1, last_current = -1, last_power = -1;
    float power = voltage * current;

    if (fabs(voltage - last_voltage) > 0.02)
    {
        static char voltage_str[16];
        snprintf(voltage_str, sizeof(voltage_str), "%.2f V", voltage);
        lv_label_set_text(ui_Label6, voltage_str);
        last_voltage = voltage;
    }

    if (fabs(current - last_current) > 0.02)
    {
        static char current_str[16];
        snprintf(current_str, sizeof(current_str), "%.2f A", current);
        lv_label_set_text(ui_Label5, current_str);
        last_current = current;
    }

    if (fabs(power - last_power) > 0.02)
    {
        static char power_str[16];
        snprintf(power_str, sizeof(power_str), "%.2f W", power);
        lv_label_set_text(ui_Label1, power_str);
        last_power = power;
    }

    lv_arc_set_value(ui_Arc3, (int)(voltage * 100 / 3.3)); 
    lv_arc_set_value(ui_CurrentArc, (int)(current * 100 / 5.0)); 
    lv_arc_set_value(ui_Arc2, (int)(power * 100 / (3.3 * 5.0))); 

    lv_refr_now(NULL);  
}

void setup()
{
  lv_init();

  // Serial2.begin(115200,SERIAL_8N1,ESP32_RX_PIN,ESP32_TX_PIN);
  Serial.begin(115200);

  WiFi.begin(ssid,password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");

  initSDLog();

   struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    Serial.println("Time Set Successfully");
    Serial.print("Current time: ");
    Serial.print(timeinfo.tm_hour);  // Print hour
    Serial.print(":");
    Serial.print(timeinfo.tm_min);   // Print minutes
    Serial.print(":");
    Serial.println(timeinfo.tm_sec); // Print seconds
  } else {
    Serial.println("Failed to obtain time");
  }

  //TODO: tftDisplay.begin() SPI Config conflicts with SD Card
  tftDisplay.begin();
  tftDisplay.setRotation(1);

  setupUART2();

  // Initialize display buffer and driver
  static lv_disp_draw_buf_t draw_buf;
  static lv_color_t buf[LV_HOR_RES_MAX * 10]; // Buffer for 10 lines
  lv_disp_draw_buf_init(&draw_buf, buf, NULL, LV_HOR_RES_MAX * 10);

  static lv_disp_drv_t disp_drv;     // Display driver
  lv_disp_drv_init(&disp_drv);       // Basic initialization
  disp_drv.hor_res = 480;            // Set horizontal resolution
  disp_drv.ver_res = 320;            // Set vertical resolution
  disp_drv.flush_cb = my_disp_flush; // Set flush callback
  disp_drv.draw_buf = &draw_buf;     // Assign the buffer to the driver
  lv_disp_drv_register(&disp_drv);   // Register the driver

  ui_init();
}

void loop()
{
  readUART2(&voltage,&current);
  update_ui();

  updateSDLog();
    // Update NTP time every hour.
  if (millis() - lastNTPCall >= NTP_UPDATE_INTERVAL) {
    updateNTPTime();
    lastNTPCall = millis();
  }

  lv_timer_handler();
}