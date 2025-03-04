idf_component_register(
    SRCS "ScreenManager.cpp" "secrets.cpp" "displayTask.cpp" "wifiTask.cpp" "main.cpp"
         "sd_log.cpp"
         "uart.cpp"
         "UI/ui.c"
         "UI/ui_Screen1.c"
         "UI/ui_Screen2.c"
         "buttonTask.cpp"
    INCLUDE_DIRS "../include/"
)
