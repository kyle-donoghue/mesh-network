#include <SPI.h>
#include <LoRa.h>

void setup() {
  // turn the LED off by making the voltage LOW
  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,HIGH);
  Serial.begin(250000);
  delay(5000);
  LoRa.setPins(D3, D2, D1);

  if (!LoRa.begin(915E6)) {
    digitalWrite(LED_BUILTIN,LOW);
    while (1);
  }
}

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");

    // read packet
    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
}
