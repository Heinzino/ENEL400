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
    if (display_state_flag){
      display_state_flag = 0;
    }
    else{
      display_state_flag = 1;
    }
    display_state_counter = 0;
    display_change_flag = 1;
  }

  // Increment display state change counter
  display_state_counter++;
}

#endif