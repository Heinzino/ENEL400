#ifndef OLED_INO
#define OLED_INO

/*-------------------------------------Print Battery Shape---------------------------------------*/
void OLED_draw_battery(){

   // Clear display to start display in known state
  display.clearDisplay();

  // Battery rectangle
  display.fillRect(8, 8, 104, 48, WHITE); 
  display.fillRect(10, 10, 100, 44, BLACK);

  // Anode rectangle
  display.fillRect(112, 24, 8, 16, WHITE);
  display.fillRect(112, 26, 6, 12, BLACK);

  // Must use display.display() to actually show the changes
  display.display(); 
}



/*-------------------------------------Print Battery Charge--------------------------------------*/
void OLED_print_charge(){

  // Draw the rectangle and fill it for the battery percentage
  display.fillRect(11, 11, 98, 42, BLACK);
  display.fillRect(11, 11, battery_charge_percentage - 2, 42, WHITE);
  display.fillRect(41, 24, 38, 16, BLACK);

  // Set text color to white (default is black) and font size to 2
  display.setTextColor(WHITE);
  display.setTextSize(2);

  // Set cursor location depending on battery percentage.
  // To center the text, we need to determine the number of digits
  if (battery_charge_percentage > 99){
    display.setCursor(43, 25);
  }
  else if (battery_charge_percentage < 10) {
    display.setCursor(55, 25);
  }
  else {
    display.setCursor(49, 25);
  }

  // Print the battery percentage
  display.print(battery_charge_percentage);

  // Must use display.display() to actually show the changes
  display.display(); 
}



/*------------------------------------Print Case Temperature-------------------------------------*/
void OLED_print_temperature(){

  // Set text color to white (default is black) and font size to 2
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(10, 10);
  display.print(temperature_celcius, 2);
}

#endif