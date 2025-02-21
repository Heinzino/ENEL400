/*------------------------------------------LIBRARIES--------------------------------------------*/
#include <ACS712.h> // Handles drivers for the ACS712 current sensor
#include <avr/sleep.h> // Handles putting the CPU to sleep for low power modes



/*-----------------------------------------DEFINE PINS-------------------------------------------*/
#define INTERRUPT_PIN   2
#define PWM_MOSFET_PIN  3
#define LOAD_MOSFET_PIN 5
//#define TEMP_SENSOR_PIN 9
#define BATTERY_VOLTAGE_PIN   A1
#define GENERATOR_VOLTAGE_PIN A2
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



/*----------------------------------------ACS712 Objects-----------------------------------------*/
// Declare ACS712 30A sensor to measure generator current. 5V, 1023 steps, 66mV/A
ACS712  ACS_generator(GENERATOR_CURRENT_PIN, 5.0, 1023, 66);

// Declare ACS712 20A sensor to measure load current. 5V, 1023 steps, 100mV/A
ACS712  ACS_load(LOAD_CURRENT_PIN, 5.0, 1023, 100);



/*---------------------------------------Global Variables----------------------------------------*/
// Variables to hold generator metrics
float generator_voltage;
float generator_current;
float generator_power;

// Variable to hold battery voltage
float battery_voltage;

// Variable to hold load current
float load_current;

// FSM State Variables
// Make these volatile as they are changed in an ISR
volatile uint8_t system_state_variable = 0;
volatile uint8_t charge_state_variable = 0;



/*-----------------------------------------System Setup------------------------------------------*/
void setup() {
  system_init();
}



/*---------------------------------------System Superloop----------------------------------------*/
void loop() {

  // Implements system FSM
  switch(system_state_variable){
    case SYSTEM_SLEEP:
      //system_sleep();
      get_data();
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
