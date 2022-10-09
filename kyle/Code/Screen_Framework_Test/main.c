#include <SPI.h>              // include libraries
#include <Serial.h>
#include "Screen.h"

TSPoint button;
int screenCode = 0;
bool awake = 1;
bool prevAwake = 1;

void setup() {
    pinMode(A5, OUTPUT);
    cli();                      //stop interrupts for till we make the settings
    screenPower(1);
    setupSerial();
    setupTimers();
    sei();
}

void loop() { // TO-DO: we need to add timeout
    //busy loop of waiting for serial or button press

    if (Serial.Available() >= expectedSerial) {
        receiveUART();
    }
    if (prevAwake != awake) {
        screenPower(awake);
        prevAwake = awake;
    }
    if(awake) {
        button = getPress();
        if (isPressed(button.z)) {
            screenCode = getButton(button.x, button.y);
            handleScreen(screenCode);
        }
    }

}
//no more auto asleep, just press button to sleep and wake up, poll every 500ms to reduce CPU load
ISR(TIMER1_COMPA_vect){
  TCNT1  = 0;                  //First, set the timer back to 0 so it resets for next interrupt
  // awake = 1 if wake button is pressed
  if (analogRead(A7 < .5)) {
    awake = !awake;
  }
}