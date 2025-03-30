#ifndef SENSOR_DATA_HPP
#define SENSOR_DATA_HPP

#include <vector>
#include <cstdint>

class SettingsScreen; // Forward declaration to prevent circular dependency

class SensorData {
public:
    static SensorData &getInstance();

    void resetData();  // Resets energy and distance accumulation


    void setVoltage(float v);
    void setCurrent(float c);
    void setRPM(int rpm);
    void recordDataPoint();

    float getVoltage() const;
    float getCurrent() const;
    int getRPM() const;
    int getCadence() const;
    float getSpeed() const;
    float getDistance() const;
    float getEnergy() const;
    int getCalories() const;

private:
    SensorData() = default;
    
    float voltage = 0.0;
    float current = 0.0;
    int RPM = 0;

    uint32_t lastTimestampMs = 0;
    double lastSpeed = 0.0;
    double lastPower = 0.0;

    double totalDistance = 0.0; // in km
    double totalEnergy = 0.0;   // in Joules

    static constexpr float JOULE_TO_CALORIES = 1.0 / 4.184; // 1 Joule = 1 / 4.184 Calories
    static constexpr float CALORIE_EFFICIENCY_FACTOR = 0.00125; // Adjust this to tune calorie calculation
};

#endif // SENSOR_DATA_HPP
