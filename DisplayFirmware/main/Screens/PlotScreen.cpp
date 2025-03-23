#include "PlotScreen.hpp"

extern lv_obj_t *ui_Chart1; // Use the existing chart from ui_Screen4


void PlotScreen::updateScreen()
{
    if (!data_series)
    {
        // Locate existing series in the chart instead of creating a new one
        data_series = lv_chart_get_series_next(ui_Chart1, NULL);
    }

    // if (data_series)
    // {
    //     // Refresh the chart UI
    //     lv_chart_refresh(ui_Chart1);
    // }

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
    ScreenManager& sm = ScreenManager::getInstance();
    switch (btn)
    {
    case ButtonID::SHIFT_HRZN_BTN:
        sm.safeSwitchToScreen(ScreenTitles::POWER_DISPLAY, ui_Screen1);
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

