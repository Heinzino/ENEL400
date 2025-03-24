#ifndef LED_STRIP_INO
#define LED_STRIP_INO

/*----------------------------------Active Mode Pulsing Effect-----------------------------------*/
void runActiveEffect() {
  static float pulsePhase = 0.0;
  int pwrIndex = constrain(powerLevel, 1, POWER_LEVELS-1);
  
  // Calculate natural breathing curve
  float pulseSpeed = 1.0 / pulsePeriods[pwrIndex];
  pulsePhase += pulseSpeed * 20; // Phase increment
  
  // Use full sine wave cycle for better control
  if(pulsePhase >= TWO_PI) pulsePhase = 0;

  // Enhanced brightness calculation with gamma correction
  float pulseValue = (sin(pulsePhase) + 1.0) * 0.5; // Range 0-1
  uint8_t brightness = BASE_BRIGHTNESS + 
                      strip.gamma8(pulseValue * (pulsePeaks[pwrIndex] - BASE_BRIGHTNESS));

  strip.setBrightness(brightness);
  fillStrip(currentColor);
  strip.show();
}



/*------------------------------------Idle Mode Smooth Effect------------------------------------*/
void runIdleEffect() {
  static uint16_t hue = 0;
  for(int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(hue)));
  }
  strip.setBrightness(IDLE_BRIGHTNESS);
  strip.show();
  hue += 128; // Slower color cycling
  delay(20);
}



/*-------------------------------------Calculate Level Color-------------------------------------*/
uint32_t calculateLevelColor(int level) {
  level = constrain(level, 0, USER_LEVELS-1);
  uint8_t green = map(level, 0, USER_LEVELS-1, 255, 0);
  uint8_t red = map(level, 0, USER_LEVELS-1, 0, 255);
  return strip.Color(red, green, 0);
}



/*---------------------------------------Update Led State----------------------------------------*/
void updateSystemState() {

  // Update color (userLevel) and pulse rate (powerLevel)
  userLevel = constrain(user_difficulty, 0, USER_LEVELS-1);
  powerLevel = constrain(getPowerLevel(), 0, POWER_LEVELS-1);

  // Update state and update color if in active state
  if (powerLevel > 0){
    currentState = ACTIVE;
    currentColor = calculateLevelColor(userLevel);
  }
  else{
    currentState = IDLE;
  }
}



/*-------------------------------------Fill Strip With Color-------------------------------------*/
void fillStrip(uint32_t color) {
  for(int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, color);
  }
}



/*----------------------------------------Get Power Level----------------------------------------*/
uint8_t getPowerLevel(){

  if      (generator_power < 1)
    return 0;
  else if (generator_power < 40)
    return 1;
  else if (generator_power < 70)
    return 2;
  else if (generator_power < 100)
    return 3;
  else if (generator_power < 140)
    return 4;
  else 
    return 5;
}



#endif