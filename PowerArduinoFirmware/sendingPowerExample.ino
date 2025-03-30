const int voltagePotPin = A1;
const int currentPotPin = A2;
const float voltageScale = 40.0 / 1023.0;
const float currentScale = 15.0 / 1023.0;

#define TESTSEND

uint8_t headerIconFlag = 0;
uint8_t tempFlag = 0;
const uint8_t rpmVal = 60;

void setup() {
  Serial.begin(9600);
}

void loop() {
  // Check for serial input (e.g., h1 or t0)
  if (Serial.available() >= 2) {
    char flagType = Serial.read();
    char valueChar = Serial.read();

    if ((valueChar == '0' || valueChar == '1')) {
      uint8_t value = valueChar - '0';

      if (flagType == 'h') {
        headerIconFlag = value;
      } else if (flagType == 't') {
        tempFlag = value;
      }
    }
  }

  int voltagePotValue = analogRead(voltagePotPin);
  int currentPotValue = analogRead(currentPotPin);

  float voltage = voltagePotValue * voltageScale;
  float current = currentPotValue * currentScale;

#ifdef TESTSEND
  // Send formatted output: %f %f %d %d %d
  Serial.print(voltage, 2);
  Serial.print(" ");
  Serial.print(current, 2);
  Serial.print(" ");
  Serial.print(rpmVal);
  Serial.print(" ");
  Serial.print(headerIconFlag);
  Serial.print(" ");
  Serial.println(tempFlag);
#endif

  delay(120);
}
