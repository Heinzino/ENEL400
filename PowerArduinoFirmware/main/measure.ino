#ifndef MEASURE_INO
#define MEASURE_INO

/*-----------------------------------Measure Generator Voltage-----------------------------------*/
// Function to measure generator voltage
float measure_generator_voltage(){

  // Measure the generator voltage putput from the ADC 
  uint16_t digital_generator_voltage = analogRead(GENERATOR_VOLTAGE_PIN);

  // Convert the 10 bit value to a float representing the voltage in volts (maps 1023 to 50V)
  float calibrated_value = 22.96; // Uncalibrated value: 20.46
  float generator_volts = digital_generator_voltage / calibrated_value;

  // Return the value 
  return generator_volts;
}



/*------------------------------------Measure Battery Voltage------------------------------------*/
// Function to measure battery voltage
float measure_battery_voltage(){

  // Measure the battery voltage putput from the ADC 
  uint16_t digital_battery_voltage = analogRead(BATTERY_VOLTAGE_PIN);

  // Convert the 10 bit value to a float representing the voltage in volts (maps 1023 to 20V)
  float calibrated_value = 51.15; // Uncalibrated value: 51.15
  float battery_volts = digital_battery_voltage / calibrated_value;

  // Return the value 
  return battery_volts;
}



/*-----------------------------------Measure Generator Current-----------------------------------*/
// Function to measure generator current
float measure_generator_current(){

  // Measure the generator current, average 10 samples and return value in mA
  int generator_current_mA = ACS_generator.mA_DC(10);

  // Convert the value in mA to A
  float calibrated_value = -983.1; // Uncalibrated value: -1000.0
  float generator_current_A = generator_current_mA / calibrated_value;

  // Return the generator current in Amps as a float
  return generator_current_A;
}



/*-------------------------------------Measure Load Current--------------------------------------*/
// Function to measure load current
float measure_load_current(){

  // Measure the load current, average 10 samples and return value in mA
  int load_current_mA = ACS_load.mA_DC(10);

  // Convert the value in mA to A
  float calibrated_value = -1057.5; // Uncalibrated value: -1000.0
  float load_current_A = load_current_mA / calibrated_value;

  // Return the load current in Amps as a float
  return load_current_A;
}



/*----------------------------------Measure Battery Temperature----------------------------------*/
// Function to measure battery temperature
int measure_battery_temperature(){

  // PLACEHOLDER UNTIL WE GET A TEMPERATURE SENSOR

  return 0;
}

#endif