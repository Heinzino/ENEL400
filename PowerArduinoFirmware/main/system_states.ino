#ifndef SYSTEM_STATES_INO
#define SYSTEM_STATES_INO

uint8_t system_state_variable = 0;

#define SYSTEM_INIT 0
#define GET_DATA 1
#define SEND_DATA 2
#define SYSTEM_SLEEP 3

void system_init(){

  // Setup digital output pins (to drive mosfets)
  pinMode(PWM_MOSFET_PIN, OUTPUT);
  pinMode(LOAD_MOSFET_PIN, OUTPUT);

  // Setup analog input pins
  pinMode(GENERATOR_VOLTAGE_PIN, INPUT);
  pinMode(BATTERY_VOLTAGE_PIN, INPUT);
  pinMode(GENERATOR_CURRENT_PIN, INPUT);
  pinMode(LOAD_CURRENT_PIN, INPUT);

  // Calibrate the generator current sensor
  ACS_generator.autoMidPoint();

  // Calibrate the load current sensor
  ACS_load.autoMidPoint();
}

void get_data(){
  
  // Get generator voltage and current measurements, multiply to get power;
  generator_voltage = measure_generator_voltage();
  generator_current = measure_generator_current();
  generator_power = generator_voltage * generator_current;

  // Get battery voltage
  battery_voltage = measure_battery_voltage();

  // Get load current
  load_current = measure_load_current();
}

void send_data(){

  // Send generator voltage with 2 decimal places accuracy
  Serial.print(generator_voltage, 2);

  // Send a space seperator
  Serial.print(" ");

  // Send generator current with 2 decimal places accuracy, and a newline
  Serial.println(generator_current, 2);
}

void system_sleep(){
  
}

#endif
