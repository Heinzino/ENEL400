#define RPM_CONVERSION_FACTOR 60000000  
#define RPM_TIMEOUT_MS 1000000  // 1 second timeout (in microseconds)

class HallEffectRPM {
private:
    volatile unsigned long lastPulseTime = 0;
    volatile unsigned long pulseDuration = 0;
    volatile int rpm = 0;  // Encapsulated

    static void isrHandler() { 
        instance->handleInterrupt();
    }

    void handleInterrupt() {
        unsigned long currentTime = micros();
        pulseDuration = currentTime - lastPulseTime;
        lastPulseTime = currentTime;
    }

public:
    static HallEffectRPM* instance; 

    HallEffectRPM(int pin) : sensorPin(pin) {}

    void begin() {
        pinMode(sensorPin, INPUT_PULLUP);
        instance = this;
        attachInterrupt(digitalPinToInterrupt(sensorPin), isrHandler, FALLING);
    }

    void update() {
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

    int getRPM() { 
        return rpm; 
    }

private:
    int sensorPin;
};

// Define static instance
HallEffectRPM* HallEffectRPM::instance = nullptr;

// Create sensor object
HallEffectRPM rpmSensor(HALL_EFFECT_SENSOR_PIN);

void setup() {
    Serial.begin(9600);
    rpmSensor.begin();
}

void loop() {
    rpmSensor.update();  
    Serial.print("RPM: ");
    Serial.println(rpmSensor.getRPM()); 
    delay(100);
}
