#ifndef SYSTEM_STATES_INO
#define SYSTEM_STATES_INO

/*---------------------------------------Initialize System---------------------------------------*/
void system_init(){

  // Begin the serial at 9600 baud
  Serial.begin(9600);

  // Set input hardware interrupt pin mode
  pinMode(INTERRUPT_PIN, INPUT);

  // Set MOSFET output pins 
  pinMode(GENERATOR_MOSFET_PIN, OUTPUT);
  pinMode(CHARGING_MOSFET_PIN, OUTPUT);
  pinMode(DUMP_LOAD_MOSFET_1, OUTPUT);
  pinMode(INVERTER_MOSFET, OUTPUT);
  pinMode(DUMP_LOAD_MOSFET_2, OUTPUT);

  // Set analog output pins
  pinMode(TEMP_DUMP_LOAD_1, INPUT);
  pinMode(BATTERY_VOLTAGE_PIN, INPUT);
  pinMode(GENERATOR_VOLTAGE_PIN, INPUT);
  pinMode(TEMP_DUMP_LOAD_2, INPUT);
  pinMode(GENERATOR_CURRENT_PIN, INPUT);
  pinMode(BATTERY_CURRENT_PIN, INPUT);
  analogReference(DEFAULT);

  // Attach Digital pin 2 to the hardware digital input interrupt, rising edge triggered
  //attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), digital_input_ISR, RISING);
  rpmSensor.begin();

  // Calibrate the generator current sensor
  ACS_generator.autoMidPoint();

  // Calibrate the load current sensor
  ACS_battery.autoMidPoint();

  // Begin the display and draw battery outline
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(WHITE);
  OLED_draw_battery();

  // Setup the watchdog timer, but turn it off initially
  setup_WDT();
  enableWDT();

  // Turn on generator MOSFET to allow current flow
  digitalWrite(GENERATOR_MOSFET_PIN, HIGH);

  // Turn inverter MOSFET to allow load to be used
  digitalWrite(INVERTER_MOSFET, HIGH);

  // Set PWM frequency to 1kHz (using Timer 2)
  TCCR2B = TCCR2B & B11111000 | 0x03;

  // Write initial high load to prevent current spikes
  analogWrite(DUMP_LOAD_MOSFET_1, 255);
  analogWrite(DUMP_LOAD_MOSFET_2, 255);
  
  // Unconditional transition, go to system sleep state
  system_state_variable = SYSTEM_SLEEP; 
}



/*--------------------------------------System Sleep (Idle)--------------------------------------*/
void system_sleep(){

  // Enable interrupts during sleep
  sei();

  while (!wdtFired){
    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_enable();
    sleep_cpu();
    sleep_disable();
  }

  wdtFired = false;

  // Disable interrupts after sleep
  cli();
}



/*----------------------------------------Get Sensor Data----------------------------------------*/
void get_data(){

  // Unconditional state transition, go to send data state
  system_state_variable = SET_DIFFICULTY;
  //system_state_variable = SEND_DATA;
  
  // Get generator voltage and current
  generator_voltage = measure_generator_voltage();
  generator_current = measure_generator_current();
  rpmSensor.update();

  if (generator_voltage >= 40){
    digitalWrite(GENERATOR_MOSFET_PIN, LOW);
  }
  else{
    digitalWrite(GENERATOR_MOSFET_PIN, HIGH);
  }

  // Get battery voltage and current
  battery_voltage = measure_battery_voltage();
  battery_current = measure_battery_current();
}



/*---------------------------------------Send Sensor Data----------------------------------------*/

float sanitizeFloat(float value) {
    //Avoid <= to avoid unexpected floating-point behavior.
    return (value < 0.00) ? 0.00 : value; 
}

void send_data(){

  // Unconditional state transition, go to get data state
  system_state_variable = GET_DATA;
  
  // Send generator voltage with 2 decimal places accuracy
  Serial.print(sanitizeFloat(generator_voltage) , 2);

  // Send a space seperator
  Serial.print(" ");

  // Send generator current with 2 decimal places accuracy, and a newline
  Serial.print(sanitizeFloat(generator_current) , 2);

  // Send a space seperator
  Serial.print(" ");

  Serial.println(rpmSensor.getRPM());
  //Serial.println(timer_ISR_counter);
  
  // Flush the serial buffer 
  Serial.flush();

}



/*-------------------------------------Set Bike Difficulty---------------------------------------*/
void set_difficulty(){

  // Unconditional state transition, go to charge fsm state
  system_state_variable = CHARGE_FSM;

  uint8_t read_value = read_serial_int();

  dump_load_difficulty = ((uint16_t)read_value * 168) / 10 + 102;
  //dump_load_difficulty = read_value * 28;

  analogWrite(DUMP_LOAD_MOSFET_1, dump_load_difficulty);
  analogWrite(DUMP_LOAD_MOSFET_2, dump_load_difficulty);
}



/*------------------------------------------Charge FSM-------------------------------------------*/
void charge_FSM(){

  // Unconditional state transitio, go to get data state
  system_state_variable = SYSTEM_SLEEP;

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