#include "ScreenManager.hpp"

// Singleton instance
ScreenManager& ScreenManager::getInstance() {
    static ScreenManager instance;
    return instance;
}

uint8_t ScreenManager::getScreenNumber() {
    return screenNumber;
}

void ScreenManager::toggleScreen() {
    screenNumber ^= 1; // Toggle between two screens, for now;
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