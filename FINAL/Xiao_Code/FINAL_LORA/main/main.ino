#include "HMC.h"

void setup() {
  
    Serial.begin(9600);
    delay(5000);
    //while(!Serial);
    initializeContacts();
    setupHMC();
    setupSerial();
    Serial.println("xiao setup done\n");
}

void loop() {
  if (handlingMessage)
      parseMessage();
  else if (Serial1.available()) {
    //Serial.println("xiao got data from pro mini");
        processByte();
        if (serialCounter == expectedSerial) {
            Serial.println("expected serial count was reached");
            evaluatePipe();
        }
        //receiveUART();
    }
}
