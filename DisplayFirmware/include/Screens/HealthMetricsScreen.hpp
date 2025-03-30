#ifndef HEALTH_METRICS_SCREEN_HPP
#define HEALTH_METRICS_SCREEN_HPP

#include "Screen.hpp"
#include "screenManager.hpp"
#include "lvgl.h"
#include "UI/ui.h"  // Ensure UI elements are properly included
#include <math.h>
#include "HeaderManager.hpp"

enum class ButtonID; // Forward declare to avoid circular dependency

class HealthMetricsScreen : public Screen {
public:
    void updateScreen() override;
    void handleButton(ButtonID btn) override;
    lv_obj_t* getScreen() override { return ui_Screen3; }

    void setCalories(int cals);
    void setCadence(int cad);
    void setSpeed(float spd);
    void setDistance(float dist);

private:
    int calories = 0;
    int cadence = 0;
    float speed = 0.0;
    float distance = 0.0;
};

#endif // HEALTH_METRICS_SCREEN_HPP
