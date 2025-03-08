#ifndef UART_INO
#define UART_INO

/*--------------------------------READ UNSIGNED 8 BIT FROM SERIAL--------------------------------*/
// Read UART buffer to obtain the speed setting integer
uint8_t read_serial_int() {
  static uint8_t last_valid_value = 0;
  //static bool received_first_value = false; 

  if (Serial.available()) {
    char current_char = Serial.read();

    if (current_char >= '0' && current_char <= '9') {
      last_valid_value = current_char - '0';
      //received_first_value = true;
    }
  }

  return last_valid_value; //received_first_value; ? last_valid_value : 9;
}

#endif