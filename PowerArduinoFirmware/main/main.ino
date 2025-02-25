/*------------------------------------------LIBRARIES--------------------------------------------*/
#include <ACS712.h> // Handles drivers for the ACS712 current sensor
#include <avr/sleep.h> // Handles putting the CPU to sleep for low power modes
#include <avr/io.h> // Handles AVR style I/O
#include <avr/interrupt.h> // Handles AVR style interrupts (for timers)
#include <LM35.h> // Handles drivers for the temperature sensor (LM35)



/*-----------------------------------------DEFINE PINS-------------------------------------------*/
#define INTERRUPT_PIN   2
#define PWM_MOSFET_PIN  3
#define LOAD_MOSFET_PIN 5
#define BATTERY_VOLTAGE_PIN   A1
#define GENERATOR_VOLTAGE_PIN A2
#define TEMP_SENSOR_PIN A3
#define GENERATOR_CURRENT_PIN A6
#define LOAD_CURRENT_PIN   A7



/*--------------------------------------SYSTEM FSM DEFINES---------------------------------------*/
#define SYSTEM_INIT 0
#define SYSTEM_SLEEP 1
#define GET_DATA 2
#define SEND_DATA 3
#define CHARGE_FSM 4



/*-------------------------------------CHARGING FSM DEFINES--------------------------------------*/
#define BULK       6
#define ABSORPTION 7
#define FLOATING   8
#define DISCHARGE  9



/*-------------------------------------Instantiate Objects---------------------------------------*/
// Declare ACS712 30A sensor object to measure generator current. 5V, 1023 steps, 66mV/A
ACS712  ACS_generator(GENERATOR_CURRENT_PIN, 5.0, 1023, 66);

// Declare ACS712 20A sensor object to measure load current. 5V, 1023 steps, 100mV/A
ACS712  ACS_load(LOAD_CURRENT_PIN, 5.0, 1023, 100);

// Declare LM35 temperature sensor object to measure enclosure temperature (2C-150C, 10mV/C)
LM35 enclosure_temp(TEMP_SENSOR_PIN);



/*---------------------------------------Global Variables----------------------------------------*/
// Variables to hold generator metrics
float generator_voltage;
float generator_current;
float generator_power;

// Variable to hold battery voltage
float battery_voltage;

// Variable to hold load current
float load_current;

// Variable to hold enclosure temperature
float enclosure_temperature;

// FSM State Variables
// Make these volatile as they are changed in an ISR
volatile uint8_t system_state_variable = SYSTEM_INIT; 
volatile uint8_t charge_state_variable = DISCHARGE;

// Variables to handle system sleep mode (within the timer ISR, therefore volatile)
volatile uint16_t timer_ISR_counter = 0;
volatile bool in_program_state = false;
volatile float generator_voltage_sum = 0.0;



/*-----------------------------------------System Setup------------------------------------------*/
void setup() {
  system_init();
}



/*---------------------------------------System Superloop----------------------------------------*/
void loop() {

  // Implements system FSM
  switch(system_state_variable){
    case SYSTEM_SLEEP:
      system_sleep();
      break;
    case GET_DATA:
      get_data();
      break;
    case SEND_DATA:
      send_data();
      break;
    case CHARGE_FSM:
      charge_FSM();
      break;
    default:
      get_data();
      break;
  }
}