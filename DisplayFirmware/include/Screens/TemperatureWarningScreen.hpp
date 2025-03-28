#pragma once

#include "Screen.hpp"
#include "SystemStatus.hpp"
#include "screenEnums.hpp"
#include "screenManager.hpp"

class TemperatureWarningScreen : public Screen
{
public:
    void updateScreen() override;
    void handleButton(ButtonID btn) override;
    lv_obj_t* getScreen() override { return ui_Screen5; }
private:

    bool pendingSwitchToHomeScreen = false;

};

