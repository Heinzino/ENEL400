#ifndef MEASURE_INO
#define MEASURE_INO

// Function to measure generator voltage
float measure_generator_voltage(){

  // Measure the generator voltage putput from the ADC 
  uint16_t digital_generator_voltage = analogRead(GENERATOR_VOLTAGE_PIN);

  // Convert the 10 bit value to a float representing the voltage in volts (maps 1023 to 50V)
  float generator_volts = digital_generator_voltage / 20.46;

  // Return the value 
  return generator_volts;
}

// Function to measure battery voltage
float measure_battery_voltage(){

  // Measure the battery voltage putput from the ADC 
  uint16_t digital_battery_voltage = analogRead(BATTERY_VOLTAGE_PIN);

  // Convert the 10 bit value to a float representing the voltage in volts (maps 1023 to 20V)
  float battery_volts = digital_battery_voltage / 51.15;

  // Return the value 
  return battery_volts;
}

// Function to measure generator current
float measure_generator_current(){

  // Measure the generator current, average 10 samples and return value in mA
  int generator_current_mA = ACS_generator.mA_DC(10);

  // Convert the value in mA to A
  float generator_current_A = generator_current_mA / 1000.0;

  // Return the generator current in Amps as a float
  return generator_current_A;
}

// Function to measure load current
float measure_load_current(){

  // Measure the load current, average 10 samples and return value in mA
  int load_current_mA = ACS_load.mA_DC(10);

  // Convert the value in mA to A
  float load_current_A = load_current_mA / 1000.0;

  // Return the load current in Amps as a float
  return load_current_A;
}

// Function to measure battery temperature
int measure_battery_temperature(){

  // PLACEHOLDER UNTIL WE GET A TEMPERATURE SENSOR

  return 0;
}

#endif
