#include "wifiTask.hpp"

void WiFiSetup(){
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("WiFi connected");


    struct tm timeinfo;
    if (getLocalTime(&timeinfo))
    {
        Serial.println("Time Set Successfully");
        Serial.print("Current time: ");
        Serial.print(timeinfo.tm_hour); // Print hour
        Serial.print(":");
        Serial.print(timeinfo.tm_min); // Print minutes
        Serial.print(":");
        Serial.println(timeinfo.tm_sec); // Print seconds
    }
    else
    {
        Serial.println("Failed to obtain time");
    }

}


void updateNTP(){
    // Update NTP time every hour.
    if (millis() - lastNTPCall >= NTP_UPDATE_INTERVAL)
    {
        updateNTPTime();
        lastNTPCall = millis();
    }
}