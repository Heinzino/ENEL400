#include "SystemStatus.hpp"

// Static method to return singleton instance
SystemStatus& SystemStatus::getInstance() {
    static SystemStatus instance;
    return instance;
}

void SystemStatus::setHeaderIconFlag(int value) {
    headerIconFlag = value;
}

int SystemStatus::getHeaderIconFlag() const {
    return headerIconFlag;
}

void SystemStatus::setHighTemperatureFlag(int value) {
    highTemperatureFlag = value;
}

int SystemStatus::getHighTemperatureFlag() const {
    return highTemperatureFlag;
}
