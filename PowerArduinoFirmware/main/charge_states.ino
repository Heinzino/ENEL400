#ifndef CHARGE_STATES_INO
#define CHARGE_STATES_INO

/*-----------------------------------------Charge State------------------------------------------*/
void charge(){

  // If high temperature flag is set, DO NOT CHARGE
  if (high_temperature_flag){

    // Disable all current flow
    digitalWrite(LED_MOSFET_PIN, LOW);
    digitalWrite(GENERATOR_MOSFET_PIN, LOW);
    digitalWrite(INVERTER_MOSFET, LOW);
    digitalWrite(CHARGING_MOSFET_PIN, LOW);

    // Turn on fans at max speed
    digitalWrite(DISCHARGE_MOSFET_PIN, HIGH);
    digitalWrite(FAN_MOSFET_PIN, HIGH);
    analogWrite(FAN1_AUX_PIN, 255);
    analogWrite(FAN2_AUX_PIN, 255);
  }

  else if ((battery_voltage > 14.0) && (duty_cycle == 0)){

    // disable charging
    analogWrite(CHARGING_MOSFET_PIN, 0);
  }

  // Otherwise, charging is allowed
  else {
    
    // Disallow charging if current is too high
    if (generator_current >= 2.0){
      duty_cycle = 0;
    }

    // Otherwise allow charging
    else{

      // If battery voltage is less than 14V, increase charging voltage
      if ((generator_voltage >= 16.0) && (battery_voltage < 14.0)){
        if (duty_cycle < 255) duty_cycle++;
        else duty_cycle = 255;
      }

      // If battery voltage is greater than 14V, decrease charging voltage
      else if ((generator_voltage >= 16.0) && (battery_voltage > 14.0)){
        if (duty_cycle > 0) duty_cycle--;
        else duty_cycle = 0;
      }
    }

    // Disable discharging, and write duty cycle to charging MOSFET
    digitalWrite(DISCHARGE_MOSFET_PIN, LOW);
    analogWrite(CHARGING_MOSFET_PIN, duty_cycle);
  }
}



/*----------------------------------------Discharge State----------------------------------------*/
void discharge(){


}

#endif