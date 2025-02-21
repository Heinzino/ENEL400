#ifndef SYSTEM_STATES_INO
#define SYSTEM_STATES_INO

/*---------------------------------------Initialize System---------------------------------------*/
void system_init(){

  // Setup digital output pins (to drive mosfets)
  pinMode(INTERRUPT_PIN, INPUT);
  pinMode(PWM_MOSFET_PIN, OUTPUT);
  pinMode(LOAD_MOSFET_PIN, OUTPUT);

  // Attach Digital pin 2 to the hardware digital input interrupt, rising edge triggered
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), digital_input_ISR, RISING);

  // Setup analog input pins
  pinMode(GENERATOR_VOLTAGE_PIN, INPUT);
  pinMode(BATTERY_VOLTAGE_PIN, INPUT);
  pinMode(GENERATOR_CURRENT_PIN, INPUT);
  pinMode(LOAD_CURRENT_PIN, INPUT);

  // Calibrate the generator current sensor
  ACS_generator.autoMidPoint();

  // Calibrate the load current sensor
  ACS_load.autoMidPoint();

  // Begin the serial at 
  Serial.begin(115200);
}



/*--------------------------------------System Sleep (Idle)--------------------------------------*/
void system_sleep(){
  
  // Set up for state transition
  system_state_variable = GET_DATA;

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();

}



/*----------------------------------------Get Sensor Data----------------------------------------*/
void get_data(){

  // Unconditional state transition, go to send data state
  system_state_variable = SEND_DATA;
  
  // Get generator voltage and current measurements, multiply to get power;
  generator_voltage = measure_generator_voltage();
  generator_current = measure_generator_current();
  generator_power = generator_voltage * generator_current;

  // Get battery voltage
  battery_voltage = measure_battery_voltage();

  // Get load current
  load_current = measure_load_current();
}



/*---------------------------------------Send Sensor Data----------------------------------------*/
void send_data(){

  // Unconditional state transition, go to charge state
  system_state_variable = CHARGE_FSM;

  // Send generator voltage with 2 decimal places accuracy
  //Serial.print(generator_voltage, 2);
  delay(1000);
  Serial.println(battery_voltage, 2);

  // Send a space seperator
  //Serial.print(" ");

  // Send generator current with 2 decimal places accuracy, and a newline
  //Serial.println(generator_current, 2);
}



/*------------------------------------------Charge FSM-------------------------------------------*/
void charge_FSM(){

  // Unconditional state transitio, go to get data state
  system_state_variable = GET_DATA;

  // Implements factored charging FSM
  switch(charge_state_variable){
    case BULK:
      bulk();
      break;
    case ABSORPTION:
      absorption();
      break;
    case FLOATING:
      floating();
      break;
    case DISCHARGE:
      discharge();
      break;
    default:
      discharge();
      break;
  }
}

#endif
