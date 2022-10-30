#include "HMC.h"

void setup() {
    initializeContacts();
    setupHMC();
    setupSerial();
}

void loop() {
    if (Serial.available() >= expectedSerial) {
        receiveUART();
    }
}
