#include <LoRa.h>
//Lora Parameters

#define CS_PIN D3
#define RESET_PIN D2
#define IRQ_PIN D1

#define NETWORK_ID 321
#define DEVICE_ID 3
#define CARRIER_FREQ 915E6

uint16_t M_ID = 0;

void sendMessage(uint16_t N_ID,uint16_t M_ID,uint16_t R_ID,uint16_t S_ID,char msg[])
{
    LoRa.beginPacket();
    for (int i = 1; i >= 0;i--) {
        LoRa.write((N_ID >> (8*i)) & 0xFF);
    }
    for (int i = 1; i >= 0;i--) {
        LoRa.write((M_ID >> (8*i)) & 0xFF);
    }
    for (int i = 1; i >= 0;i--) {
        LoRa.write((R_ID >> (8*i)) & 0xFF);
    }
    for (int i = 1; i >= 0;i--) {
        LoRa.write((S_ID >> (8*i)) & 0xFF);
    }
    for (int i = 0; i < 142;i++) {
        LoRa.write(msg[i]); //need to do randomID
    }
    LoRa.endPacket();
    return;
}
bool setupHMC() {

  LoRa.setPins(D3, D2, D1);
    //set spreading factor?
    if (!LoRa.begin(CARRIER_FREQ)) {             // initialize at 915MHz
        return 0; //if LoRa setup failed
    }
  LoRa.setTxPower(17);

//    Lora.onReceive(receiveMessage); //messageReceive is the callback function when receiving a message
//
//    LoRa.receive(); // turn on receive mode

    return 1;
}

void led(bool p) {
  if (p)
    digitalWrite(LED_BUILTIN,LOW);
  else
    digitalWrite(LED_BUILTIN,HIGH);
}


void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN,OUTPUT);
  led(0);
  if(!setupHMC()) {
    led(1);
    while(1);
  }
}


void loop() {
  // put your main code here, to run repeatedly:
  led(1);
  uint16_t N_ID = NETWORK_ID;
  uint16_t R_ID = 4;
  uint16_t S_ID = DEVICE_ID;
  char msg[142] = "yabba dabba doo";
  sendMessage(N_ID,M_ID++,R_ID,S_ID,msg);
  led(0);
  delay(4000);
}
