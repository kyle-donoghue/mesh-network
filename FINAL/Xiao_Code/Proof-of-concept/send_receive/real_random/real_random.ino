#include <LoRa.h>
#include <time.h>

#define DEVICE_ID 15
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Receiver Callback");
  LoRa.setPins(D3, D2, D1);

  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  LoRa.setSpreadingFactor(7);

  // Uncomment the next line to disable the default AGC and set LNA gain, values between 1 - 6 are supported
  //LoRa.setGain(6);
  
  // register the receive callback
  LoRa.onReceive(receiveMessage);

  // put the radio into receive mode
  LoRa.receive();
}

uint16_t randomGen() {
  uint8_t seed1 = LoRa.random();
  uint8_t seed2 = LoRa.random();
  seed1 += millis();
  seed2 += DEVICE_ID;
  uint16_t seed = ((uint8_t)seed1<<8)|((uint8_t)seed2);

  return seed;
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(randomGen());
  Serial.println("----");
  delay(1000);
}

uint16_t getID(char message[], uint8_t ind) {
    uint16_t ID = ((uint8_t)message[ind]<<8)|((uint8_t)message[ind+1]);
    return ID;
}
void receiveMessage( int packetSize ) { //can only Lora.read() one byte at a time
    
    if(packetSize == 0) return;
    char messageTot[142] = {0};
    char rawPacket[150] = {0};
    for (uint8_t i = 0; i < 150; i++) {
        rawPacket[i] = LoRa.read();
    }
    uint16_t senderTot = getID(rawPacket, 0);
    uint16_t msgIdTot = getID(rawPacket, 2);
    uint16_t recipientTot = getID(rawPacket, 4);
    uint16_t networkTot = getID(rawPacket, 6);
    for (uint8_t i = 0; i < 142; i++) {
        messageTot[i] = rawPacket[i+8];
    }

  Serial.println("New Message:");
  Serial.print("\t N_ID: ");
  Serial.println(networkTot);
  Serial.print("\t M_ID: ");
  Serial.println(msgIdTot);
  Serial.print("\t R_ID: ");
  Serial.println(recipientTot);
  Serial.print("\t S_ID: ");
  Serial.println(senderTot);
  Serial.print("'");
  Serial.print(messageTot);
  Serial.println("'");
  Serial.print("RSSI: ");
  Serial.println(LoRa.packetRssi());
  Serial.print("SNR: ");
  Serial.println(LoRa.packetSnr());
    //at this part we have 2 bytes for N_ID/M_ID/R_ID/S_ID
    //and 142 bytes for message :)
    /*uint16_t networkID = NETWORK_ID;
    uint16_t deviceID = DEVICE_ID;
    if(networkTot != networkID) //if the network is not ours leave the function
        return;
    if(messageExists(msgIdTot)) //if the message is in our memory leave the function
        return;
    else
    {
        logMessageID(msgIdTot);//if message is new then we log the ID in memory
    }
    if(recipientTot != deviceID)
        sendMessage(networkTot,msgIdTot,recipientTot,senderTot,messageTot); // if the message is not for us rebroadcast with all the stuff we have
    else
    {
        handleMessage(senderTot,messageTot) // if the message is for us we want to save the sender ID and the message contents
    }*/
    return;
}
