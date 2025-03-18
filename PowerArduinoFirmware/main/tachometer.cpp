#include "tachometer.h"

HallEffectRPM::HallEffectRPM(int pin) : sensorPin(pin) {}

void HallEffectRPM::begin() {
    pinMode(sensorPin, INPUT);
    instance = this;
    attachInterrupt(digitalPinToInterrupt(sensorPin), isrHandler, FALLING);
}

void HallEffectRPM::handleInterrupt() {
    unsigned long currentTime = micros();
    pulseDuration = currentTime - lastPulseTime;
    lastPulseTime = currentTime;
}

void HallEffectRPM::update() {
    unsigned long currentTime = micros();

    noInterrupts();
    unsigned long durationCopy = pulseDuration;
    unsigned long lastPulseCopy = lastPulseTime;
    interrupts();

    if (durationCopy > 0) {
        rpm = RPM_CONVERSION_FACTOR / durationCopy;
    }

    if (currentTime - lastPulseCopy > RPM_TIMEOUT_MS) {
        rpm = 0;
    }
}

int HallEffectRPM::getRPM() { 
    return rpm; 
}
