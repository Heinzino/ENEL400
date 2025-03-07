#ifndef UART_INO
#define UART_INO

/*--------------------------------READ UNSIGNED 8 BIT FROM SERIAL--------------------------------*/
// Read UART buffer to obtain the speed setting integer
/*
uint8_t read_serial_int() {
  static uint8_t last_valid_value = 0;  // Holds the last successfully read value
  static char buffer[4];  // Buffer for up to 3 digits + null terminator
  static uint8_t index = 0;  // Current buffer index

  // Check if new data is available
  while (Serial.available()) {
    char current_char = Serial.read();  // Read one character

    // If newline (`\n`) or carriage return (`\r`) is received, process the number
    if (current_char == '\n' || current_char == '\r') {
      buffer[index] = '\0';  // Null-terminate the string
      uint16_t int_value = atoi(buffer); // Convert to integer

      // Validate range (return 128 if out of range)
      last_valid_value = (int_value <= 255) ? (uint8_t)int_value : 128;

      // Reset buffer for next number
      index = 0;
      Serial.print("Received ");
      Serial.println(last_valid_value);
      return last_valid_value;
    }

    // Store only numeric characters ('0' to '9') in the buffer
    if (current_char >= '0' && current_char <= '9') {
      if (index < 3) {  // Prevent buffer overflow
        buffer[index++] = current_char;
      }
    }
  }

  return last_valid_value;  // Return last valid value if no complete number received
}*/

uint8_t read_serial_int() {
  static uint8_t last_valid_value = 0;

  // Check if new data is available
  if (Serial.available()) {
    char current_char = Serial.read(); // Read one character

    // Ensure it's a valid digit ('0' to '9')
    if (current_char >= '0' && current_char <= '9') {
      last_valid_value = current_char - '0'; // Convert ASCII to integer
      Serial.print("Received ");
      Serial.println(last_valid_value);
    }
  }

  return last_valid_value; // Return last valid value if no new data
}

#endif