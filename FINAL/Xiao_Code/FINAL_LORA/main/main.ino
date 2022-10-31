#include "HMC.h"

void setup() {
  
    Serial.begin(BAUD_RATE);
    delay(5000);
    //while(!Serial);
    initializeContacts();
    printTest();
    setupHMC();
    setupSerial();
}

void loop() {
if (Serial1.available()) {
        processByte();
        if (serialCounter == expectedSerial) {
            evaluatePipe();
        }
        //receiveUART();
    }
}
