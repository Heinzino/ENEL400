#ifndef SENSOR_DATA_HPP
#define SENSOR_DATA_HPP

class SensorData
{
public:
    static SensorData &getInstance();

    void setVoltage(float v);
    void setCurrent(float c);
    void setRPM(int rpm);

    float getVoltage() const; // Const ensures getVoltage() doesn't modify SensorData
    float getCurrent() const;

private:
    SensorData() = default; // Private constructor (Singleton)
    float voltage = 0.0;
    float current = 0.0;
    int RPM = 0;
};

#endif // SENSOR_DATA_HPP
