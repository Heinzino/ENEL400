#ifndef TIMER_INO
#define TIMER_INO

/*------------------------------------Set Up Watch Dog Timer-------------------------------------*/
// Configure Watchdog Timer for ~128ms timeout
void setup_WDT(){
  cli();  // Disable interrupts
  wdt_reset();
  WDTCSR = (1 << WDCE) | (1 << WDE); // Enter configuration mode
  WDTCSR = (1 << WDIE) | (1 << WDP1); // Set WDT to ~32ms
  sei();  // Re-enable interrupts
}

/*----------------------------------------Disable The WDT----------------------------------------*/
// Function to disable the WDT (when entering deep sleep)
void disableWDT() {
  cli(); // Disable global interrupts
  WDTCSR = (1 << WDCE) | (1 << WDE); // Enter Watchdog configuration mode
  WDTCSR = 0; // Turn off WDT
  sei(); // Re-enable global interrupts
}

/*----------------------------------------Enable The WDT-----------------------------------------*/
// Function to enable WDT (After exiting deep sleep)
void enableWDT() {
  cli(); // Disable global interrupts
  WDTCSR = (1 << WDCE) | (1 << WDE); // Enter configuration mode
  WDTCSR = (1 << WDIE) | (1 << WDP1); // Set WDT to ~32ms
  sei(); // Re-enable global interrupts
}

#endif