const int voltagePotPin = A1;
const int currentPotPin = A2;
const float voltageScale = 40.0 / 1023.0; // Scaling factor for 3.3V
const float currentScale = 15.0 / 1023.0; // Scaling factor for 5A

#define TESTSEND

uint8_t read_serial_int() {
  static uint8_t last_valid_value = 0;
  //static bool received_first_value = false; 

  if (Serial.available()) {
    char current_char = Serial.read();

    if (current_char >= '0' && current_char <= '9') {
      last_valid_value = current_char - '0';
      //received_first_value = true;
    }
  }

  return last_valid_value; //received_first_value; ? last_valid_value : 9;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
    int voltagePotValue = analogRead(voltagePotPin); // Read voltage potentiometer
    int currentPotValue = analogRead(currentPotPin); // Read current potentiometer
    int rpmVal = 60;

    float voltage = voltagePotValue * voltageScale; // Scale to 3.3V
    float current = currentPotValue * currentScale; // Scale to 5A

    //Format %f %f\n  
    //Space as a delimiter and new line ending
    #ifdef TESTSEND
    Serial.print(voltage, 2); // Send voltage (2 decimal places)
    Serial.print(" ");        // Add a space separator
    Serial.print(current, 2); // Send current and newline
    Serial.print(" ");
    Serial.println(rpmVal);
    #else
    uint8_t level = read_serial_int();
    // Serial.print("Level : ");
    // Serial.println(level);
    #endif

    delay(120);
}
