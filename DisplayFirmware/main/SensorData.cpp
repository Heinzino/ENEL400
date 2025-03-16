#include "SensorData.hpp"

SensorData& SensorData::getInstance() {
    static SensorData instance;
    return instance;
}

void SensorData::setVoltage(float v) {
    voltage = v;
}

void SensorData::setCurrent(float c) {
    current = c;
}

float SensorData::getVoltage() const {
    return voltage;
}

float SensorData::getCurrent() const {
    return current;
}
