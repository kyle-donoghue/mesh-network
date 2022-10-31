#include <SPI.h>              // include libraries
#include "Screen.h"

int counter = 0;
void setup() {
    pinMode(A5, OUTPUT);
    setupSerial();
    screenPower(1);
    pinMode(13, OUTPUT);
    handleScreen(CONTACTS_SCREEN_CODE);
    //setupTimers();
}

void loop() { 
    if (Serial.available()) {
        processByte();
        if (serialCounter == expectedSerial) {
            evaluatePipe();
        }
    }
    /*
    if (prevAwake != awake) {
        screenPower(awake);
        prevAwake = awake;
    }
    else if(awake) {
        button = getPress();
        if (isPressed(button.z)) {
            screenCode = getButton(button.x, button.y);
            if (screenCode) {
                handleScreen(screenCode);
            }
        }
    }*/

}

////no more auto asleep, just press button to sleep and wake up, poll every 500ms to reduce CPU load
//ISR(TIMER1_COMPA_vect){
//  TCNT1  = 0;                  //First, set the timer back to 0 so it resets for next interrupt
//  // awake = 1 if wake button is pressed
//  bool press = analogRead(A7) < 100 ? 1 : 0;
//  if ((press != prevPress)) {
//    if (press) {
//      awake = !awake;
//    }
//    prevPress = press;
//  }
//}
