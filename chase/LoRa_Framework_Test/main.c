#include <SPI.h>              // include libraries
#include <LoRa.h>

void setup() {
    setupHMC();
    setupSerial();
}

void loop() {
    //callback/IRQ-driven system
    if (Serial.Available()) {
        receiveUART();
    }
}
