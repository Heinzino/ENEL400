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
#include "tachometer.h" // Handles tachometer drivers



/*-----------------------------------------DEFINE PINS-------------------------------------------*/
#define UART_RX_PIN            0
#define UART_TX_PIN            1
#define HALL_EFFECT_SENSOR_PIN 2 
#define CHARGING_MOSFET_PIN    3
#define GENERATOR_MOSFET_PIN   4
#define DUMP_LOAD_MOSFET_1     5  
#define DUMP_LOAD_MOSFET_2     6
#define FAN_MOSFET_PIN         7
#define INVERTER_MOSFET        8
#define LED_MOSFET_PIN         9
#define FAN1_AUX_PIN           10
#define FAN2_AUX_PIN           11
#define DISCHARGE_MOSFET_PIN   12
#define SPI_SCK                13

#define TEMPERATURE_SENSOR_PIN A0
#define BATTERY_VOLTAGE_PIN    A1
#define GENERATOR_VOLTAGE_PIN  A2
#define INVERTER_CURRENT_PIN   A3
#define I2C_SDA                A4
#define I2C_SCL                A5
#define GENERATOR_CURRENT_PIN  A6
#define BATTERY_CURRENT_PIN    A7



/*--------------------------------------SYSTEM FSM DEFINES---------------------------------------*/
#define SYSTEM_INIT      0
#define SYSTEM_SLEEP     1
#define GET_DATA         2
#define SEND_DATA        3
#define SET_DIFFICULTY   4
#define CHARGE_FSM       5
#define LOAD_PRIORITIZER 6



/*-------------------------------------CHARGING FSM DEFINES--------------------------------------*/
#define BULK       0
#define ABSORPTION 1
#define FLOATING   2
#define DISCHARGE  3



/*-------------------------------------Instantiate Objects---------------------------------------*/
// Declare ACS712 30A sensor object to measure generator current. 5V, 1023 steps, 66mV/A
ACS712  ACS_generator(GENERATOR_CURRENT_PIN, 5.0, 1023, 66);

// Declare ACS712 20A sensor object to measure battery current. 5V, 1023 steps, 100mV/A
ACS712  ACS_battery(BATTERY_CURRENT_PIN, 5.0, 1023, 100);

// Declare ACS712 30A sensor object to measure inverter current. 5V, 1023 steps, 66mV/A
ACS712 ACS_inverter(INVERTER_CURRENT_PIN, 5.0, 1023, 66);

// Declare LM35 temperature sensor object to measure dump load 1 temperature (2C-150C, 10mV/C)
LM35 temperature(TEMPERATURE_SENSOR_PIN);

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

// Variable to hold battery metrics
float battery_voltage;
float battery_current;
float battery_power;
uint8_t battery_charge_percentage = 100;

// Variable to hold dump load metrics
float temperature;
uint8_t dump_load_difficulty = 128;

// Variable to hold inverter load metrics
float inverter_current;
float inverter_power;

// FSM State Variables
// Make these volatile as they are changed in an ISR
volatile uint8_t system_state_variable = SYSTEM_INIT; 
volatile uint8_t charge_state_variable = DISCHARGE;

// Used by the WDT to determine whether or not it has been fired (ISR)
volatile bool wdtFired = false;



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
    case LOAD_PRIORITIZER:
      load_prioritizer();
      break;
    default:
      get_data();
      break;
  }
}