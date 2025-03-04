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