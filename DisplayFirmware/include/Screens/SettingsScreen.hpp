#ifndef SETTINGS_SCREEN_HPP
#define SETTINGS_SCREEN_HPP

#include "Screen.hpp"
#include <Arduino.h> //String support

class SettingsScreen : public Screen
{
public:
    static SettingsScreen &getInstance();

    void updateScreen() override;
    void handleButton(ButtonID btn) override;
    lv_obj_t *getScreen() override { return ui_Screen1; } //NOTE: Placeholder screen return

    void setWheelSize(float sizeInInches);
    float getWheelSizeInInches() const;
    float getWheelSizeInMeters() const;

    void setGearRatio(float ratio);
    float getGearRatio() const;

    void setTimezone(String tz);
    String getTimezone() const;

private:
    SettingsScreen() = default;
    float wheelSizeInInches = 27; // Default wheel size
    float gearRatio = 5.0;        // Default gear ratio (1:5)
    String timezone = "CST6";  // Fixed GMT-6 timezone (no Daylight savings)
};

#endif // SETTINGS_SCREEN_HPP
