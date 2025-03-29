#ifndef MEASURE_INO
#define MEASURE_INO

/*-----------------------------------Measure Generator Voltage-----------------------------------*/
// Function to measure generator voltage

float measure_generator_voltage(){
  // Measure the generator voltage putput from the ADC 
  uint16_t digital_generator_voltage = analogRead(GENERATOR_VOLTAGE_PIN);

  // Convert the 10 bit value to a float representing the voltage in volts (maps 1023 to 50V)
  float calibrated_value = 21.55; // Uncalibrated value: 20.46
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
  float calibrated_value = 51.54; // Uncalibrated value: 49.69
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



/*------------------------------------Measure Battery Current------------------------------------*/
// Function to measure battery current
float measure_battery_current(){

  // Measure the load current, average 10 samples and return value in mA
  int battery_current_mA = ACS_battery.mA_DC(10);

  // Convert the value in mA to A
  float calibrated_value = -1035.3; // Uncalibrated value: -1000.0
  float battery_current_A = battery_current_mA / calibrated_value;

  // Return the load current in Amps as a float
  return battery_current_A;
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
  //float return_temp = temperature.cel();
  float return_temp = 20.0;

  // Return this value 
  return return_temp;
}

#endif