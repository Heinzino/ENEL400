#ifndef PLOT_SCREEN_HPP
#define PLOT_SCREEN_HPP

#include "Screen.hpp"
#include "screenManager.hpp"
#include "SensorData.hpp"
#include "lvgl.h"
#include "screenEnums.hpp"


class PlotScreen : public Screen {
public:

    static PlotScreen &getInstance();

    void updateScreen() override;
    void handleButton(ButtonID btn) override;
    lv_obj_t *getScreen() override { return ui_Screen4; }  // Return the chart object


    void updateChartData(lv_coord_t *new_data, uint16_t data_size);  // Function to update the chart
    static void plotSetup();

private:
    static void axisLabels();  // Function to update the chart axis labels

    lv_chart_series_t *data_series = nullptr;  // Reference to the chart series
};

#endif // PLOT_SCREEN_HPP
