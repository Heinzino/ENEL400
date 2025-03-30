#ifndef SYSTEM_STATES_INO
#define SYSTEM_STATES_INO

/*---------------------------------------Initialize System---------------------------------------*/
void system_init(){

  // Begin the serial at 9600 baud
  Serial.begin(9600);

  // Set digital pin direction
  pinMode(HALL_EFFECT_SENSOR_PIN, INPUT);
  pinMode(CHARGING_MOSFET_PIN, OUTPUT);
  pinMode(GENERATOR_MOSFET_PIN, OUTPUT);
  pinMode(DUMP_LOAD_MOSFET_1, OUTPUT);
  pinMode(DUMP_LOAD_MOSFET_2, OUTPUT);
  pinMode(FAN_MOSFET_PIN, OUTPUT);
  pinMode(INVERTER_MOSFET, OUTPUT);
  pinMode(LED_MOSFET_PIN, OUTPUT);
  pinMode(FAN1_AUX_PIN, OUTPUT);
  pinMode(FAN2_AUX_PIN, OUTPUT);
  pinMode(DISCHARGE_MOSFET_PIN, OUTPUT);

  // Set analog input pins
  pinMode(TEMPERATURE_SENSOR_PIN, INPUT);
  pinMode(BATTERY_VOLTAGE_PIN, INPUT);
  pinMode(GENERATOR_VOLTAGE_PIN, INPUT);
  pinMode(INVERTER_CURRENT_PIN, INPUT);
  pinMode(GENERATOR_CURRENT_PIN, INPUT);
  pinMode(BATTERY_CURRENT_PIN, INPUT);
  analogReference(DEFAULT);

  rpmSensor.begin();

  // Calibrate the generator current sensor
  ACS_generator.autoMidPoint();

  // Calibrate the load current sensor
  ACS_battery.autoMidPoint();

  // Calibrate the inverter current sensor
  ACS_inverter.autoMidPoint();

  // Begin the display and draw battery outline
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(WHITE);
  OLED_draw_battery();

  // Begin the led strip
  digitalWrite(LED_MOSFET_PIN, HIGH);
  strip.begin();
  strip.show();

  // Setup the watchdog timer
  setup_WDT();
  enableWDT();

  // Turn on generator MOSFET to allow current flow
  digitalWrite(GENERATOR_MOSFET_PIN, HIGH);

  // Turn inverter MOSFET off to disallow generator from powering load
  digitalWrite(INVERTER_MOSFET, LOW);

  // Initially disable battery charging and discharging
  digitalWrite(CHARGING_MOSFET_PIN, LOW);
  digitalWrite(DISCHARGE_MOSFET_PIN, LOW);

  // Set PWM frequency to 1kHz (using Timer 2)
  TCCR2B = TCCR2B & B11111000 | 0x03;

  // Enable the fans at highest speed
  digitalWrite(FAN_MOSFET_PIN, HIGH);
  analogWrite(FAN1_AUX_PIN, 255);
  analogWrite(FAN2_AUX_PIN, 255);
  
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
  Serial.print(" ");

  // Send generator current with 2 decimal places accuracy, and a newline
  Serial.print(sanitizeFloat(generator_current) , 2);
  Serial.print(" ");

  // This code to be used for a later version
  // Send wheel RPM
  Serial.print(rpmSensor.getRPM());
  Serial.print(" ");

  // Send load power source flag
  Serial.print(load_power_source);
  Serial.print(" ");

  // Send high temperature flag
  Serial.println(high_temperature_flag);

  // Flush the serial buffer 
  Serial.flush();
}



/*----------------------------------------Get Sensor Data----------------------------------------*/
void get_data(){

  // Unconditional state transition, go to load prioritizer
  system_state_variable = SET_DIFFICULTY;
  
  // Get generator voltage and current, multiply to get power
  generator_voltage = measure_generator_voltage();
  generator_current = measure_generator_current();
  generator_power = generator_voltage * generator_current;

  // Get battery voltage and current
  battery_voltage = measure_battery_voltage();
  battery_current = measure_battery_current();

  // Get inverter current, multiply to get power
  inverter_current = measure_inverter_current();
  inverter_power = inverter_current * 15.0;

  // Update RPM value
  rpmSensor.update();

  // Get case temperature
  temperature_celcius = measure_temperature();
}



/*-------------------------------------Set Bike Difficulty---------------------------------------*/
void set_difficulty(){

  // Unconditional state transition, go to load prioritizer 
  system_state_variable = LOAD_PRIORITIZER;

  user_difficulty = read_serial_int();

  dump_load_difficulty = ((uint16_t)user_difficulty * 168) / 10 + 102;
  //dump_load_difficulty = read_value * 28;

  analogWrite(DUMP_LOAD_MOSFET_1, dump_load_difficulty);
  analogWrite(DUMP_LOAD_MOSFET_2, dump_load_difficulty);
}



/*------------------------------------Load Prioritizer Logic-------------------------------------*/
void load_prioritizer(){

  // Unconditional state transition to charge state
  system_state_variable = CHARGE_STATE;

  // If high temperature flag is set, turn off all loads except fans
  if (high_temperature_flag){

    // Disable all current flow
    digitalWrite(LED_MOSFET_PIN, LOW);
    digitalWrite(GENERATOR_MOSFET_PIN, LOW);
    digitalWrite(INVERTER_MOSFET, LOW);
    digitalWrite(CHARGING_MOSFET_PIN, LOW);

    // Turn on fans at max speed
    digitalWrite(DISCHARGE_MOSFET_PIN, HIGH);
    digitalWrite(FAN_MOSFET_PIN, HIGH);
    analogWrite(FAN1_AUX_PIN, 255);
    analogWrite(FAN2_AUX_PIN, 255);
  }
  
  // Otherwise if the generator isn't on 
  else if (generator_voltage <= 16.0){

    // Disable current flow from generator
    digitalWrite(GENERATOR_MOSFET_PIN, HIGH);

    // Allow battery to power load
    digitalWrite(INVERTER_MOSFET, LOW);
    digitalWrite(DISCHARGE_MOSFET_PIN, HIGH);
    load_power_source = 0;
  }

  // Otherwise if the generator voltage is too high
  else if (generator_voltage > 40.0){

    // Maximize load (reduces voltage)
    digitalWrite(GENERATOR_MOSFET_PIN, HIGH);
    digitalWrite(DISCHARGE_MOSFET_PIN, LOW);
    digitalWrite(INVERTER_MOSFET, HIGH);
    digitalWrite(DUMP_LOAD_MOSFET_1, HIGH);
    digitalWrite(DUMP_LOAD_MOSFET_2, HIGH);
    load_power_source = 1;
  }

  // Otherwise in normal operation
  else{

    // Allow current to flow from generator and to inverter
    digitalWrite(GENERATOR_MOSFET_PIN, HIGH);

    // Disable battery discharging
    digitalWrite(DISCHARGE_MOSFET_PIN, LOW);

    digitalWrite(INVERTER_MOSFET, HIGH);
    load_power_source = 1;
  }
}



/*------------------------------------------Charge FSM-------------------------------------------*/
void charge_state(){

  // Unconditional state transition, go to LED control state
  system_state_variable = LED_CONTROL;

  // If high temperature flag is set, DO NOT CHARGE
  if (high_temperature_flag){

    // Disable all current flow
    digitalWrite(LED_MOSFET_PIN, LOW);
    digitalWrite(GENERATOR_MOSFET_PIN, LOW);
    digitalWrite(INVERTER_MOSFET, LOW);
    digitalWrite(CHARGING_MOSFET_PIN, LOW);

    // Turn on fans at max speed
    digitalWrite(DISCHARGE_MOSFET_PIN, HIGH);
    digitalWrite(FAN_MOSFET_PIN, HIGH);
    analogWrite(FAN1_AUX_PIN, 255);
    analogWrite(FAN2_AUX_PIN, 255);
  }

  // Otherwise, charging is allowed
  else {
    
    // Disallow charging if current is too high
    if (battery_current >= 2.0){
      duty_cycle = 0;
      digitalWrite(CHARGING_MOSFET_PIN, LOW);
    }

    // Allow charging if generator voltage is sufficient
    else if (generator_voltage >= 16.0){

      // If battery voltage is less than 14V, increase charging voltage
      if (battery_voltage < 14.0){
        if (duty_cycle < 255) duty_cycle++;
        else duty_cycle = 255;
      }

      // If battery voltage is greater than 14V, decrease charging voltage
      else if (battery_voltage > 14.0){
        if (duty_cycle > 0) duty_cycle--;
        else duty_cycle = 0;
      }
      digitalWrite(DISCHARGE_MOSFET_PIN, LOW);
      //analogWrite(CHARGING_MOSFET_PIN, duty_cycle); // THIS LINE CAUSES GENERATOR VOLTAGE TO DROP AFTER SOME TIME
    }

    else{
      digitalWrite(CHARGING_MOSFET_PIN, LOW);
    }
  }
}



/*---------------------------------------Led Control State---------------------------------------*/
void led_control(){

  // Unconditional state transition, go to temperature monitoring state
  system_state_variable = TEMP_MONITORING;
  
  // Update the led strip effect
  updateSystemState();
  
  // Execute the led strip effect
  if(currentState == IDLE) {
    runIdleEffect();
  } else {
    runActiveEffect();
  }
}



/*------------------------------------Temperature Monitoring-------------------------------------*/
void temp_monitoring(){

  // Unconditional state transition, go to sleep state
  system_state_variable = SYSTEM_SLEEP;

  // Check for high temperature
  if (temperature_celcius > 30.0){ // FOR CURRENT PROTOTYPE WITHOUT TEMP SENSOR
    
    // Disable all current flow
    digitalWrite(LED_MOSFET_PIN, LOW);
    digitalWrite(GENERATOR_MOSFET_PIN, LOW);
    digitalWrite(INVERTER_MOSFET, LOW);
    digitalWrite(CHARGING_MOSFET_PIN, LOW);

    // Turn on fans at max speed
    digitalWrite(DISCHARGE_MOSFET_PIN, HIGH);
    digitalWrite(FAN_MOSFET_PIN, HIGH);
    analogWrite(FAN1_AUX_PIN, 255);
    analogWrite(FAN2_AUX_PIN, 255);

    // Set the high temperature flag
    high_temperature_flag = 1;
  }

  // If not too hot, clear high temperature flag
  // DO NOT ENABLE MOSFETs HERE! That is done in
  // the load prioritizer, which has its own algorithm
  else {
    high_temperature_flag = 0;
  }
}

#endif