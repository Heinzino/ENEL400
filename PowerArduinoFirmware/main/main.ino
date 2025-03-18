/*------------------------------------------LIBRARIES--------------------------------------------*/
#include <ACS712.h> // Handles drivers for the ACS712 current sensor
#include <avr/sleep.h> // Handles putting the CPU to sleep for low power modes
#include <avr/io.h> // Handles AVR style I/O
#include <avr/interrupt.h> // Handles AVR style interrupts (for timers)
#include <LM35.h> // Handles drivers for the temperature sensor (LM35)
#include <avr/wdt.h> // Handles Watchdog Timer

#include <Wire.h> // Handles I2C communication for OLED display
#include <Adafruit_GFX.h> // Handles drawing shapes for OLED display
#include <Adafruit_SSD1306.h> // Handles low level drivers for OLED display
#include "tachometer.h"



/*-----------------------------------------DEFINE PINS-------------------------------------------*/
#define INTERRUPT_PIN         2
#define GENERATOR_MOSFET_PIN  3
#define CHARGING_MOSFET_PIN   5
#define DUMP_LOAD_MOSFET_1    6  
#define DUMP_LOAD_MOSFET_2    10
#define INVERTER_MOSFET       11
#define TEMP_DUMP_LOAD_1      A0
#define BATTERY_VOLTAGE_PIN   A1
#define GENERATOR_VOLTAGE_PIN A2
#define TEMP_DUMP_LOAD_2      A3
#define GENERATOR_CURRENT_PIN A6
#define BATTERY_CURRENT_PIN   A7
#define HALL_EFFECT_SENSOR_PIN 2  



/*--------------------------------------SYSTEM FSM DEFINES---------------------------------------*/
#define SYSTEM_INIT     0
#define SYSTEM_SLEEP    1
#define GET_DATA        2
#define SEND_DATA       3
#define SET_DIFFICULTY  4
#define CHARGE_FSM      5



/*-------------------------------------CHARGING FSM DEFINES--------------------------------------*/
#define BULK       6
#define ABSORPTION 7
#define FLOATING   8
#define DISCHARGE  9



/*-------------------------------------Instantiate Objects---------------------------------------*/
// Declare ACS712 30A sensor object to measure generator current. 5V, 1023 steps, 66mV/A
ACS712  ACS_generator(GENERATOR_CURRENT_PIN, 5.0, 1023, 66);

// Declare ACS712 20A sensor object to measure load current. 5V, 1023 steps, 100mV/A
ACS712  ACS_battery(BATTERY_CURRENT_PIN, 5.0, 1023, 100);

// Declare LM35 temperature sensor object to measure dump load 1 temperature (2C-150C, 10mV/C)
LM35 dump_load_1_temp(TEMP_DUMP_LOAD_1);

// Declare LM35 temperature sensor object to measure dump load 2 temperature (2C-150C, 10mV/C)
LM35 dump_load_2_temp(TEMP_DUMP_LOAD_2);

// Declare SSD1306 OLED display object to write to the OLED display (128 x 64)
Adafruit_SSD1306 display(128, 64, &Wire, -1);

// Define static instance
HallEffectRPM* HallEffectRPM::instance = nullptr;

// Create sensor object
HallEffectRPM rpmSensor(HALL_EFFECT_SENSOR_PIN);



/*---------------------------------------Global Variables----------------------------------------*/
// Variables to hold generator metrics
volatile float generator_voltage;
float generator_current;
float generator_power;

// Variable to hold battery voltage
float battery_voltage;

// Variable to hold load current
float battery_current;

// Variable to hold dump load temperatures
float dump_load_1_temperature;
float dump_load_2_temperature;

// Variable to hold current system difficulty setting
uint8_t dump_load_difficulty = 128;

// FSM State Variables
// Make these volatile as they are changed in an ISR
volatile uint8_t system_state_variable = SYSTEM_INIT; 
volatile uint8_t charge_state_variable = DISCHARGE;

// Used by the WDT to determine whether or not it has been fired (ISR)
volatile bool wdtFired = false;

// Used to hold battery charge in percentage
uint8_t battery_charge_percentage = 100;



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
    case SET_DIFFICULTY:
      set_difficulty();
      break;
    case CHARGE_FSM:
      charge_FSM();
      break;
    default:
      get_data();
      break;
  }
  delay(100);
}