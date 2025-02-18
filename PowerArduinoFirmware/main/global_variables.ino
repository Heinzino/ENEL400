#ifndef GLOBAL_VARIABLES_INO
#define GLOBAL_VARIABLES_INO

// Declare ACS712 30A sensor to measure generator current. 5V, 1023 steps, 66mV/A
ACS712  ACS_generator(GENERATOR_CURRENT_PIN, 5.0, 1023, 66);

// Declare ACS712 20A sensor to measure load current. 5V, 1023 steps, 100mV/A
ACS712  ACS_load(LOAD_CURRENT_PIN, 5.0, 1023, 100);

// Variables to hold generator metrics
float generator_voltage;
float generator_current;
float generator_power;

// Variable to hold battery voltage
float battery_voltage;

// Variable to hold load current
float load_current;

#endif
