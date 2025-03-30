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
    //TODO Optimize space just keep last two values and integrate them, then update
    uint32_t now = esp_timer_get_time() / 1000; // Convert microseconds to milliseconds
    timestamps.push_back(now);
    speedValues.push_back(getSpeed());
    powerValues.push_back(voltage * current);
}

// Compute Distance (km) = ∫ Speed (km/h) * dt (h)
float SensorData::getDistance() const
{
    if (timestamps.size() < 2)
        return 0.0;

    float totalDistance = 0.0;
    for (size_t i = 1; i < timestamps.size(); ++i)
    {
        float dt = (timestamps[i] - timestamps[i - 1]) / 3600000.0; // Convert ms to hours
        totalDistance += speedValues[i - 1] * dt;
    }
    return totalDistance;
}

// Compute Energy (Joules) = ∫ Power (W) * dt (s)
float SensorData::getEnergy() const
{
    if (timestamps.size() < 2)
        return 0.0;

    float totalEnergy = 0.0;
    for (size_t i = 1; i < timestamps.size(); ++i)
    {
        float dt = (timestamps[i] - timestamps[i - 1]) / 1000.0; // Convert ms to seconds
        totalEnergy += powerValues[i - 1] * dt;
    }
    return totalEnergy;
}

int SensorData::getCalories() const {
    return static_cast<int>(getEnergy() * JOULE_TO_CALORIES * CALORIE_EFFICIENCY_FACTOR);
}
