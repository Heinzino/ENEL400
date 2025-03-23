#include "PlotScreen.hpp"

extern lv_obj_t *ui_Chart1; // Use the existing chart from ui_Screen4
bool pendingSwitchToPowerDisplay = false;


void PlotScreen::updateScreen()
{

    if(pendingSwitchToPowerDisplay){
        pendingSwitchToPowerDisplay = false;
        ScreenManager::getInstance().safeSwitchToScreen(ScreenTitles::POWER_DISPLAY, ui_Screen1);
        return; // Exit early to prevent chart update
    }

    if (!data_series)
    {
        // Locate existing series in the chart instead of creating a new one
        data_series = lv_chart_get_series_next(ui_Chart1, NULL);
    }

    const std::vector<float> &history = SensorData::getInstance().getVoltageHistory();

    // Limit to most recent N points
    static constexpr size_t MAX_POINTS = 100;
    size_t start = history.size() > MAX_POINTS ? history.size() - MAX_POINTS : 0;
    size_t visibleCount = history.size() - start;

    // Allocate LVGL-compatible buffer
    static lv_coord_t chartBuffer[MAX_POINTS];
    for (size_t i = 0; i < visibleCount; ++i)
    {
        chartBuffer[i] = static_cast<lv_coord_t>(history[start + i]); // scale to match Y-axis
    }

    // Set chart range (scaled to 60V)
    lv_chart_set_range(ui_Chart1, LV_CHART_AXIS_PRIMARY_Y, 0, 60);

    // Update chart data
    lv_chart_set_point_count(ui_Chart1, visibleCount);
    lv_chart_set_update_mode(ui_Chart1, LV_CHART_UPDATE_MODE_SHIFT); // scroll mode
    lv_chart_set_ext_y_array(ui_Chart1, data_series, chartBuffer);

    axisLabels();
    lv_timer_handler();
    lv_task_handler();
    safeLvglRefresh();
}

void PlotScreen::updateChartData(lv_coord_t *new_data, uint16_t data_size)
{
    if (!data_series)
    {
        data_series = lv_chart_get_series_next(ui_Chart1, NULL);
    }

    if (data_series)
    {
        lv_chart_set_ext_y_array(ui_Chart1, data_series, new_data); // Update with new values
        lv_chart_refresh(ui_Chart1);                                // Refresh chart to reflect changes
    }
}

void PlotScreen::handleButton(ButtonID btn)
{
    switch (btn)
    {
    case ButtonID::SHIFT_HRZN_BTN:
        pendingSwitchToPowerDisplay = true;
        break;
    default:
        break;
    }
}

void PlotScreen::axisLabels()
{
    static char xLabel[16];
    sniprintf(xLabel, sizeof(xLabel), "Time\n  (s)");
    lv_label_set_text(ui_XLabel, xLabel);   

    static char yLabel[32];
    sniprintf(yLabel, sizeof(yLabel), "Voltage (V)");
    lv_label_set_text(ui_YLabel, yLabel);   
}

