#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const int x = 128; // Define display width
const int y = 64;  // Define display height
const int reset = -1;

uint32_t potentiometer = 0;

Adafruit_SSD1306 display(x, y, &Wire, reset); // Assign the above attributes to the display

void setup() {
  Serial.begin(115200); // Begin serial monitor at 115200 baud

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Check if init failed
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  display.clearDisplay(); // Reset to start display in known state

  // Battery rectangle
  display.fillRect(8, 8, 104, 48, WHITE); 
  display.fillRect(10, 10, 100, 44, BLACK);

  // Anode rectangle
  display.fillRect(112, 24, 8, 16, WHITE);
  display.fillRect(112, 26, 6, 12, BLACK);

  display.display(); 

}

void loop() {

  potentiometer = 1023; 
  Serial.println(potentiometer);

  display.fillRect(11, 11, 98, 42, BLACK);
  display.fillRect(11, 11, (potentiometer * 99) >> 10, 42, WHITE);

  display.fillRect(41, 24, 38, 16, BLACK);
  
  if (potentiometer > 1013){
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setCursor(43, 25);
    display.print((potentiometer  * 101) >> 10);
  }
  else if (potentiometer < 101) {
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setCursor(55, 25);
    display.print((potentiometer  * 101) >> 10);
  }
  else {
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setCursor(49, 25);
    display.print((potentiometer  * 101) >> 10);
  }


  display.display(); 
  delay(50);

}