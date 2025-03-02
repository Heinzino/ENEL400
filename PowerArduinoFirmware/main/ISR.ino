#ifndef ISR_INO
#define ISR_INO

/*---------------------------------------Digital Input ISR---------------------------------------*/
void digital_input_ISR(){

  // Change the in_program_state variable to reflect that the system is starting up and will run
  in_program_state = true;

  // Restart Timer 1 to interrupt every 100ms
  enableWDT();
}
 


/*-------------------------------------Watch Dog Timer ISR---------------------------------------*/
// Watchdog Timer ISR
ISR(WDT_vect) {

  // The WDT has fired
  wdtFired = true;

  // If the program is running (not in a sleep mode)
  if (in_program_state){

    // Make sure the next state we enter is the get_data state
    system_state_variable = GET_DATA;

    // Add the current generator voltage to the sum
    generator_voltage_sum += generator_voltage;

    // Increment the counter (to go off after 60 seconds)
    timer_ISR_counter++;
  }
}

#endif