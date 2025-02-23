#ifndef TIMER_INO
#define TIMER_INO

/*----------------------------------------Set Up Timer 1-----------------------------------------*/
// Setup Timer1 in CTC mode to generate an interrupt every ~100ms.
void setupTimer1() {

  // Clear Timer/Counter Control Registers
  TCCR1A = 0;
  TCCR1B = 0;
  
  // Set CTC mode (Clear Timer on Compare Match)
  TCCR1B |= (1 << WGM12);
  
  // For a 16MHz clock with a 1024 prescaler:
  // Tick frequency = 16,000,000 / 1024 = 15625 Hz.
  // Desired period = 100ms -> Required counts = 15625 * 0.1 ≈ 1562.5.
  // We use OCR1A = 1562 so the timer resets after 1563 ticks (~100ms).
  OCR1A = 1563;
  
  // Enable Timer1 Compare Match A interrupt.
  TIMSK1 |= (1 << OCIE1A);
  
  // Start Timer1 with a 1024 prescaler.
  TCCR1B |= (1 << CS12) | (1 << CS10);
}

#endif