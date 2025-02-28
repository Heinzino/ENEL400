#ifndef UART_INO
#define UART_INO

/*--------------------------------READ UNSIGNED 8 BIT FROM SERIAL--------------------------------*/
// Read UART buffer to obtain the speed setting integer
uint8_t read_serial_int(){

  // Initialize the return buffer array with null characters
  uint8_t int_read_array[3] = {0, 0, 0};

  // Iterate through and grab each valid character
  for(uint8_t chars_read = 0; chars_read < 4; chars_read++){

    // Read the current character
    char current_char = Serial.read();

    // Check whether or not the character is a digit (0-9)
    if((current_char >= 48) && (current_char <= 57)){

      // Convert the char to its digit and add to the array
      int_read_array[chars_read] = current_char - 48;
    }
    else{

      // Break the loop
      break;
    }
  }

  // Sum all the values read into one number (ideally between 0 - 255)
  uint16_t int_value = int_read_array[0] * 100 + int_read_array[1] * 10 + int_read_array[2];

  // If value is greater than 255 (data corrupted)
  if (int_value > 255){

    // Set int_value to 128
    int_value = 128;
  }

  // Return the value cast as an unsigned 8 bit number
  return (uint8_t) int_value;
}

#endif