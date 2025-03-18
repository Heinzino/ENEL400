#ifndef ISR_INO
#define ISR_INO

/*-------------------------------------Watch Dog Timer ISR---------------------------------------*/
// Watchdog Timer ISR
ISR(WDT_vect) {

  // The WDT has fired
  wdtFired = true;

  // Make sure the next state we enter is the send data state
  system_state_variable = SEND_DATA;
}

#endif