#include "SensorData.hpp"
#include "SettingsScreen.hpp"
#include "esp_timer.h"
#include <cmath>

SensorData &SensorData::getInstance()
{
    static SensorData instance;
    return instance;
}

// Setters
void SensorData::setVoltage(float v)
{
    voltage = v;
}

void SensorData::setCurrent(float c)
{
    current = c;
}

void SensorData::setRPM(int rpm)
{
    RPM = rpm;
}

// Getters
float SensorData::getVoltage() const
{
    return voltage;
}

float SensorData::getCurrent() const
{
    return current;
}

int SensorData::getRPM() const
{
    return RPM;
}

// Compute Cadence (RPM / Gear Ratio)
int SensorData::getCadence() const
{
    float gearRatio = SettingsScreen::getInstance().getGearRatio();
    return static_cast<int>(std::round(RPM / gearRatio));
}

// Compute Speed (km/h) using Wheel Size in Meters
float SensorData::getSpeed() const
{
    float wheelDiameterMeters = SettingsScreen::getInstance().getWheelSizeInMeters();
    return RPM * M_PI * wheelDiameterMeters * (1.0 / 60.0) * 3.6;
}

// Record timestamp and data points
void SensorData::recordDataPoint()
{
    uint32_t now = esp_timer_get_time() / 1000; // ms

    if (lastTimestampMs > 0) {
        double dtHours = (now - lastTimestampMs) / 3600000.0f;
        double dtSeconds = (now - lastTimestampMs) / 1000.0f;

        totalDistance += lastSpeed * dtHours;
        totalEnergy += lastPower * dtSeconds;
    }

    lastTimestampMs = now;
    lastSpeed = getSpeed();
    lastPower = voltage * current;
}

// Compute Distance (km) = ∫ Speed (km/h) * dt (h)
float SensorData::getDistance() const
{
    return totalDistance;
}

// Compute Energy (Joules) = ∫ Power (W) * dt (s)
float SensorData::getEnergy() const
{
    return totalEnergy;
}

int SensorData::getCalories() const {
    return static_cast<int>(getEnergy() * JOULE_TO_CALORIES * CALORIE_EFFICIENCY_FACTOR);
}

void SensorData::resetData() {
    totalDistance = 0.0f;
    totalEnergy = 0.0f;
    lastTimestampMs = 0;
    lastSpeed = 0.0f;
    lastPower = 0.0f;
}
