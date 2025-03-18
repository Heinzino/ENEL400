#include "ScreenManager.hpp"

static portMUX_TYPE screenSwitchMux1 = portMUX_INITIALIZER_UNLOCKED;

// Singleton instance
ScreenManager& ScreenManager::getInstance() {
    static ScreenManager instance;
    return instance;
}

uint8_t ScreenManager::getScreenNumber() {
    return screenNumber;
}

ScreenManager::ScreenManager() {
    screens[POWER_DISPLAY] = new PowerScreen();
    screens[RESISTANCE_LEVEL] = new ResistanceScreen();
    screens[HEALTH_METRICS] = new HealthMetricsScreen();
}

Screen* ScreenManager::getCurrentScreenObject() {
    return screens[screenNumber];
}

void ScreenManager::display(){
    if (xSemaphoreTake(lvglMutex, pdMS_TO_TICKS(100)) == pdTRUE) {
        LOG(LOG_LEVEL_DEBUG, "Screen Number: " + String(screenNumber) + " \n");
        screens[static_cast<size_t>(screenNumber)]->updateScreen();  // Dynamically calls updateScreen()
        xSemaphoreGive(lvglMutex);
    }
}

void ScreenManager::updateScreenState(ScreenState state){
    screenState = state;    
}

bool ScreenManager::isScreenOn(){
    return screenState == ScreenState::ON;
}

void ScreenManager::safeSwitchToScreen(ScreenTitles newScreen, lv_obj_t* lvglScreen) {
    uart_disable_rx_intr(UART_NUM);
    delay(UART_INTR_TIMEOUT_MS);
    portENTER_CRITICAL(&screenSwitchMux1);

    lv_scr_load(lvglScreen);
    screenNumber = newScreen;

    portEXIT_CRITICAL(&screenSwitchMux1);
    uart_enable_rx_intr(UART_NUM);
}