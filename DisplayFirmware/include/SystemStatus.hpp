#ifndef SYSTEM_STATUS_HPP
#define SYSTEM_STATUS_HPP

#include "screenEnums.hpp"
#include "UI/ui.h"

int systemHighTempState();
class SystemStatus {
private:
    int headerIconFlag = 0;
    int highTemperatureFlag = 0;

    // Private constructor for singleton pattern
    SystemStatus() = default;

public:
    static SystemStatus& getInstance();

    void setHeaderIconFlag(int value);
    int getHeaderIconFlag() const;

    void setHighTemperatureFlag(int value);
    int getHighTemperatureFlag() const;

    // Delete copy constructor and assignment operator
    SystemStatus(const SystemStatus&) = delete;
    void operator=(const SystemStatus&) = delete;
};

#endif // SYSTEM_STATUS_HPP
