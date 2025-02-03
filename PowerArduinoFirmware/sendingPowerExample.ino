const int voltagePotPin = A1;
const int currentPotPin = A2;
const float voltageScale = 3.3 / 1023.0; // Scaling factor for 3.3V
const float currentScale = 5.0 / 1023.0; // Scaling factor for 5A

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

}

void loop() {
    int voltagePotValue = analogRead(voltagePotPin); // Read voltage potentiometer
    int currentPotValue = analogRead(currentPotPin); // Read current potentiometer
    
    float voltage = voltagePotValue * voltageScale; // Scale to 3.3V
    float current = currentPotValue * currentScale; // Scale to 5A

    //Format %f %f\n  
    //Space as a delimiter and new line ending
    Serial.print(voltage, 2); // Send voltage (2 decimal places)
    Serial.print(" ");        // Add a space separator
    Serial.println(current, 2); // Send current and newline

    delay(20);
}
