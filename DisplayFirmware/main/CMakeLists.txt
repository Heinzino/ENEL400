idf_component_register(
    SRCS "TimeManager.cpp" "SensorData.cpp" "ScreenManager.cpp" "secrets.cpp" "displayTask.cpp" "wifiTask.cpp" "main.cpp"
         "sd_log.cpp"
         "uart.cpp"
         "UI/ui.c"
         "UI/ui_Screen1.c"
         "UI/ui_Screen2.cpp"
         "UI/ui_Screen3.cpp"
         "UI/ui_Screen4.c"
         "UI/timerHeader.cpp"
         "buttonTask.cpp"
         "Screens/PowerScreen.cpp"
         "Screens/ResistanceScreen.cpp"
         "Screens/HealthMetricsScreen.cpp"
         "Screens/SettingsScreen.cpp"
         "Screens/PlotScreen.cpp"
    INCLUDE_DIRS "../include/"
                 "../include/Screens"
)
