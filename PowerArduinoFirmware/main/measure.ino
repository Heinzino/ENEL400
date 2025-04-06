#ifndef MEASURE_INO
#define MEASURE_INO

/*-----------------------------------Measure Generator Voltage-----------------------------------*/
// Function to measure generator voltage

float measure_generator_voltage(){
  // Measure the generator voltage putput from the ADC 
  uint16_t digital_generator_voltage = analogRead(GENERATOR_VOLTAGE_PIN);

  // Convert the 10 bit value to a float representing the voltage in volts (maps 1023 to 50V)
  float calibrated_value = 19.274; // Uncalibrated value: 20.46
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
  float calibrated_value = 50.48; // Uncalibrated value: 49.69
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
  float calibrated_value = 983.1; // Uncalibrated value: -1000.0
  float generator_current_A = generator_current_mA / calibrated_value;

  // Return the generator current in Amps as a float
  return generator_current_A;// + 0.320;
}



/*------------------------------------Measure Battery Current------------------------------------*/
// Function to measure battery current
float measure_battery_current(){

  // Measure the load current, average 10 samples and return value in mA
  int battery_current_mA = ACS_battery.mA_DC(10);

  // Convert the value in mA to A
  float calibrated_value = -1035.3; // Uncalibrated value: -1000.0
  float battery_current_A = battery_current_mA / calibrated_value;

  // Return the load current in Amps as a float
  return battery_current_A + 0.2507;
}



/*-----------------------------------Measure Inverter Current------------------------------------*/
// Function to measure load current
float measure_inverter_current(){

  // Measure the load current, average 10 samples and return value in mA
  int inverter_current_mA = ACS_inverter.mA_DC(10);

  // Convert the value in mA to A
  float calibrated_value = -1035.3; // Uncalibrated value: -1000.0
  float inverter_current_A = inverter_current_mA / calibrated_value;

  // Return the load current in Amps as a float
  return inverter_current_A;
}



/*-----------------------------------Temperature Measurement-------------------------------------*/
// Function to measure temperature
float measure_temperature(){

  // Measure the temperature from the LM35 sensor
  float return_temp = temperature.cel();

  // Return this value 
  return return_temp;
}



/*-----------------------------------Temperature Measurement-------------------------------------*/
// Function to convert battery voltage to a percentage value
uint8_t get_battery_charge(){

  // Apply linear battery charge model (maps 12.89V to 100% and 11.63V to 0%)
  // use f notation to force compiler to use floating point rather than doubles
  float intermediate_value = 79.37f * (battery_voltage - 11.63f);

  // Make sure to clamp any outliers (if battery is non-standard)
  if (intermediate_value <= 0){
    intermediate_value = 0;
  }

  if (intermediate_value >= 100){
    intermediate_value = 100;
  }

  // Hand over the value cast as an unsigned 8 bit integer
  return (uint8_t) intermediate_value;
}

#endif