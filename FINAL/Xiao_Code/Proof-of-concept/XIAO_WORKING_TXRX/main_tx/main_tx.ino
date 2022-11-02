#include <SPI.h>
#include <LoRa.h>
int counter = 0;

void setup() {
  // turn the LED off by making the voltage LOW
  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,HIGH);
  delay(5000);
  
  Serial.println("LoRa Receiveeer");
  LoRa.setPins(D3, D2, D1);
  if (!LoRa.begin(915E6)) {
    digitalWrite(LED_BUILTIN,LOW);
    while (1);
  }
  LoRa.setTxPower(17);

}

void loop() {
  digitalWrite(LED_BUILTIN,LOW);

  // send packet
  LoRa.beginPacket();
  LoRa.print("hello ");
  LoRa.print(counter);
  LoRa.endPacket();

  counter++;
  digitalWrite(LED_BUILTIN,HIGH);
  delay(2000);
}
