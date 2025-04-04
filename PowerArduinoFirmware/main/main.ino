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
#include <Adafruit_NeoPixel.h> // Handles LED strip drivers



/*-----------------------------------------DEFINE PINS-------------------------------------------*/
#define UART_RX_PIN            0 // VERIFIED
#define UART_TX_PIN            1 // VERIFIED
#define HALL_EFFECT_SENSOR_PIN 2 // VERIFIED
#define CHARGING_MOSFET_PIN    3
#define GENERATOR_MOSFET_PIN   4
#define DUMP_LOAD_MOSFET_1     5
#define DUMP_LOAD_MOSFET_2     6
#define FAN_MOSFET_PIN         7 // VERIFIED
#define INVERTER_MOSFET        8 // VERIFIED
#define LED_MOSFET_PIN         9 // VERIFIED
#define FAN1_AUX_PIN           10 // VERIFIED
#define FAN2_AUX_PIN           11 // VERIFIED
#define DISCHARGE_MOSFET_PIN   12 // VERIFIED
#define LED_PWM_CONTROL_PIN    13 // VERIFIED

#define TEMPERATURE_SENSOR_PIN A0
#define BATTERY_VOLTAGE_PIN    A1
#define GENERATOR_VOLTAGE_PIN  A2
#define INVERTER_CURRENT_PIN   A3
#define I2C_SDA                A4 // VERIFIED
#define I2C_SCL                A5 // VERIFIED
#define GENERATOR_CURRENT_PIN  A6
#define BATTERY_CURRENT_PIN    A7   



/*--------------------------------------SYSTEM FSM DEFINES---------------------------------------*/
#define SYSTEM_INIT       0
#define SYSTEM_SLEEP      1
#define GET_DATA          2
#define SEND_DATA         3
#define SET_DIFFICULTY    4
#define CHARGE_STATE      5
#define LOAD_PRIORITIZER  6
#define LED_CONTROL       7
#define TEMP_MONITORING   8



/*-------------------------------------Instantiate Objects---------------------------------------*/
// Declare ACS712 30A sensor object to measure generator current. 5V, 1023 steps, 66mV/A
ACS712  ACS_generator(GENERATOR_CURRENT_PIN, 5.0, 1023, 66);

// Declare ACS712 30A sensor object to measure battery current. 5V, 1023 steps, 66mV/A
ACS712  ACS_battery(BATTERY_CURRENT_PIN, 5.0, 1023, 66);

// Declare ACS712 30A sensor object to measure inverter current. 5V, 1023 steps, 66mV/A
ACS712 ACS_inverter(INVERTER_CURRENT_PIN, 5.0, 1023, 66);

// Declare LM35 temperature sensor object to measure dump load 1 temperature (2C-150C, 10mV/C)
LM35 temperature(TEMPERATURE_SENSOR_PIN);

// Define static instance
HallEffectRPM* HallEffectRPM::instance = nullptr;

// Create sensor object
HallEffectRPM rpmSensor(HALL_EFFECT_SENSOR_PIN);

// Declare SSD1306 OLED display object to write to the OLED display (128 x 64)
Adafruit_SSD1306 display(128, 64, &Wire, -1);

// Declare neopixel led strip object to write to the led strip
Adafruit_NeoPixel strip(30, LED_MOSFET_PIN, NEO_GRB + NEO_KHZ800);



/*---------------------------------------Global Variables----------------------------------------*/
// Variables to hold generator metrics
volatile float generator_voltage = 0.0;
float generator_current = 0.0;
float generator_power = 0.0;

// Variable to hold battery metrics
float battery_voltage = 0.0;
float battery_current = 0.0;
uint8_t battery_charge_percentage = 100;

// Variable to hold dump load metrics
uint8_t user_difficulty = 0;
uint8_t dump_load_difficulty = 128;

// Variable to hold inverter load metrics
float inverter_current = 0.0;
float inverter_power = 0.0;
uint8_t load_power_source = 0;

// Variables to hold temperature and charging metrics
float temperature_celcius = 0.0;
uint8_t high_temperature_flag = 0;
uint8_t duty_cycle = 0;

// Variables to hold OLED display state information
volatile uint8_t display_state_counter = 152; // count to a max of 156 (5 seconds)
volatile uint8_t display_state_flag = 0; // 0 for battery charge, 1 for temperature
volatile uint8_t display_change_flag = 0;

// FSM State Variables
// Make these volatile as they are changed in an ISR
volatile uint8_t system_state_variable = SYSTEM_INIT; 

// Used by the WDT to determine whether or not it has been fired (ISR)
volatile bool wdtFired = false;

// Used by the LED strip
const int NUM_LEDS = 30;
const int BASE_BRIGHTNESS = 50;
const int IDLE_BRIGHTNESS = 120;
const int USER_LEVELS = 10;   // 0-9 (green to red)         
const int POWER_LEVELS = 6;   // 0-5 (0 = idle)
int userLevel = 0;   
int powerLevel = 0;
const uint16_t pulsePeriods[POWER_LEVELS] = {0, 7000, 4000, 1500, 900, 500};
const uint8_t pulsePeaks[POWER_LEVELS] = {255, 255, 255, 255, 255, 255}; 
uint32_t currentColor = strip.Color(0, 0, 0); 
enum SystemState {IDLE, ACTIVE};
SystemState currentState = IDLE;



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
    case SEND_DATA:
      send_data();
      break;
    case GET_DATA:
      get_data();
      break;
    case SET_DIFFICULTY:
      set_difficulty();
      break;
    case LOAD_PRIORITIZER:
      load_prioritizer();
      break;
    case CHARGE_STATE:
      charge_state();
      break;
    case LED_CONTROL:
      led_control();
      break;
    case TEMP_MONITORING:
      temp_monitoring();
      break;
    default:
      system_sleep();
      send_data();
      break;
  }
}