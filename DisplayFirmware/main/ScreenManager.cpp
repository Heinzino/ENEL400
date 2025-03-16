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

void ScreenManager::toggleScreen() {
    if(screenNumber == ScreenTitles::POWER_DISPLAY){
        screenNumber = ScreenTitles::RESISTANCE_LEVEL;
    }
    else{
        screenNumber = ScreenTitles::POWER_DISPLAY;
    }
}

uint8_t ScreenManager::getResistanceLevel() {
    return bikerResistanceLevel;
}

void ScreenManager::incrementResistance() {
    bikerResistanceLevel++;
    if(bikerResistanceLevel >= MAX_BIKER_RESISTANCE_LEVEL){
        bikerResistanceLevel = MAX_BIKER_RESISTANCE_LEVEL;
    }
}

void ScreenManager::decrementResistance() {
    if (bikerResistanceLevel > 0) {
        bikerResistanceLevel--;
    }
}

char* ScreenManager::resistanceLevelToString(){
    static char buffer[4];  
    sprintf(buffer, "%u", bikerResistanceLevel);
    return buffer;
}

void ScreenManager::display(){
    if (xSemaphoreTake(lvglMutex, pdMS_TO_TICKS(100)) == pdTRUE)
    {
        LOG(LOG_LEVEL_DEBUG, "Screen Number: " + String(screenNumber) + " \n");
        switch(static_cast<ScreenTitles>(screenNumber)){
            case POWER_DISPLAY:
                updateScreen1();
                break;
            case RESISTANCE_LEVEL:
                updateScreen2();
                break;
        }
        xSemaphoreGive(lvglMutex);  // Release the mutex after done
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
    screenNumber = newScreen;  // Set the current screen to the new one

    portEXIT_CRITICAL(&screenSwitchMux1);
    uart_enable_rx_intr(UART_NUM);
}