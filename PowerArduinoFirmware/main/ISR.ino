#ifndef ISR_INO
#define ISR_INO

/*-------------------------------------Watch Dog Timer ISR---------------------------------------*/
// Watchdog Timer ISR
ISR(WDT_vect) {

  // The WDT has fired
  wdtFired = true;

  // Make sure the next state we enter is the send data state
  system_state_variable = SEND_DATA;

  // Invert the display state flag
  if (display_state_counter > 156){
    display_state_flag = !display_state_flag;
    display_state_counter = 0;
  }

  // Increment display state change counter
  display_state_counter++;
}

#endif