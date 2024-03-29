#include <SPI.h>              // include libraries
#include "Screen.h"

int counter = 0;
void setup() {
    pinMode(A5, OUTPUT);
    setupSerial();
    screenPower(1);
    pinMode(13, OUTPUT);
    handleScreen(MAIN_SCREEN_CODE);
    setupTimers();
}

void loop() { 
    if (Serial.available()) {
        
        processByte();
        if (serialCounter == expectedSerial) {
            evaluatePipe();
        }
    }
    
    if (prevAwake != awake) {
        Serial.print("screen switched to: ");
        Serial.println(awake);
        Serial.println(prevAwake);
        screenPower(awake);
        prevAwake = awake;
    }
    
    else if(awake) {
        button = getPress();
        if (isPressed(button.z) && !pressing && (holdCount > 100)) {
            holdCount = 0;
            pressing = true;
            screenCode = getButton(button.x, button.y);
            if (screenCode) {
                handleScreen(screenCode);
            }
            delay(25);
        }
        else if (!isPressed(button.z)) {
          if (holdCount <= 100)
            holdCount++;
          pressing = false;
        }
    }

}

//no more auto asleep, just press button to sleep and wake up, poll every 500ms to reduce CPU load
ISR(TIMER1_COMPA_vect){
  Serial.println(analogRead(A7));
  TCNT1  = 0;                  //First, set the timer back to 0 so it resets for next interrupt
  // awake = 1 if wake button is pressed
  bool press = analogRead(A7) < 500 ? 1 : 0;
  if ((press != prevPress)) {
    if (press) {
      awake = !awake;
    }
    prevPress = press;
  }
}
