#include "buttonTask.hpp"

Button buttons[4] = {{BTN1, LOW, LOW, 0, 0},
              {BTN2, LOW, LOW, 0, 0},
              {BTN3, LOW, LOW, 0, 0},
              {BTN4, LOW, LOW, 0, 0}};

void buttonSetup()
{
    for (int i = 0; i < 4; ++i)
    {
        pinMode(buttons[i].pin, INPUT);
    }
}

void updateButtonState()
{
    ScreenManager& screenManager = ScreenManager::getInstance();

    for (int i = 0; i < 4; i++)
    {
        buttons[i].currentState = digitalRead(buttons[i].pin);

        // Detect button press and release
        if (buttons[i].lastState == HIGH && buttons[i].currentState == LOW)
        { // button pressed
            buttons[i].pressedTime = millis();
        }
        else if (buttons[i].lastState == LOW && buttons[i].currentState == HIGH)
        { // button released
            buttons[i].releasedTime = millis();
            long pressDuration = buttons[i].releasedTime - buttons[i].pressedTime;

            if (pressDuration >= SHORT_PRESS_TIME_MS){
                Serial.print("BTN");
                Serial.print(i + 1);
                Serial.println(" short press detected");

                switch (i)
                {
                case SHIFT_HRZN_BTN:
                    screenManager.toggleScreen(); //TODO: Update with more screens
                    break;
                case FN1_BTN:
                    if(screenManager.getScreenNumber()== RESISTANCE_LEVEL){ //TODO: Abstract with screen function
                        screenManager.incrementResistance();
                    } 
                    break;
                case FN2_BTN:
                    if(screenManager.getScreenNumber() == RESISTANCE_LEVEL){
                        screenManager.decrementResistance();
                    }
                default:
                    break;
                }

            }
            // else if (pressDuration > LONG_PRESS_TIME){
            //     Serial.print("BTN");
            //     Serial.print(i + 1);
            //     Serial.println(" long press detected");
            // }
        }

        // Save the last state for the button
        buttons[i].lastState = buttons[i].currentState;
    }
}
