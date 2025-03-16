#ifndef POWER_SCREEN_HPP
#define POWER_SCREEN_HPP

#include "Screen.hpp"
#include "screenManager.hpp"
#include <cmath>
#include <cstdio>

class PowerScreen : public Screen {
public:
    void updateScreen() override;
    void handleButton(ButtonID btn) override;

    void setPowerVoltage(float v, float c);  // Update power & voltage

private:
    float voltage = 0.0;
    float current = 0.0;
    static constexpr float MAX_VOLTAGE = 60.0;
    static constexpr float MAX_CURRENT = 20.0;
    static constexpr float MAX_POWER   = MAX_VOLTAGE * MAX_CURRENT;

    // Cached values to prevent unnecessary LVGL updates
    float last_voltage = -1;
    float last_current = -1;
    float last_power = -1;
};

#endif // POWER_SCREEN_HPP
