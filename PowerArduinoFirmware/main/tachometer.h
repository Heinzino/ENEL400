#ifndef TACHOMETER_H
#define TACHOMETER_H

#include <Arduino.h>

#define RPM_CONVERSION_FACTOR 60000000  
#define RPM_TIMEOUT_MS 1000000  // 1 second timeout (in microseconds)

class HallEffectRPM {
private:
    volatile unsigned long lastPulseTime = 0;
    volatile unsigned long pulseDuration = 0;
    volatile int rpm = 0;
    static HallEffectRPM* instance;
    int sensorPin;

    static void isrHandler() { 
        instance->handleInterrupt();
    }

    void handleInterrupt();

public:
    HallEffectRPM(int pin);
    void begin();
    void update();
    int getRPM();
};

#endif  // TACHOMETER_H
