#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <SPI.h>
#include "LoraDefine.h"
#include <LoRa.h>
#include <time.h>
#include <EEPROM.h>




//char messages[NUM_MSG][144] = {""}; // \0 //messages[0][0] = message1;
uint16_t logs[NUM_LOG] = {0};
char contacts[NUM_CONTS][128] = {'\0'};// 100x156 array where each row is split into [ID(0-1),NAME(1-14).MSG(14-156)]
char messageToDisplay[128] = {'\0'};
uint8_t handler = READY;
uint8_t expectedSerial = 1; //1 for waiting for code or ack; 64 for most other cases
char messageToSend[150] = {'\0'};
    char rawPacket[122] = {'\0'};

uint8_t serialPipe[64] = {0};
uint8_t serialCounter = 0;

bool handlingMessage = false;

uint8_t batteryPercentage() {
  Serial.print("battery percentage is: ");
  uint16_t val = analogRead(A0);
  float battvolt = val*4.5/1024.0;
  Serial.println(battvolt/4.5*100);
  return (uint8_t)(battvolt/4.5*100);
}



uint16_t randomGen() {
  uint8_t seed1 = LoRa.random();
  uint8_t seed2 = LoRa.random();
  seed1 += millis();
  seed2 += DEVICE_ID;
  uint16_t seed = ((uint8_t)seed1<<8)|((uint8_t)seed2);

  return seed;
}

uint8_t initializeContacts() { //split row into bytes to be processed
  EEPROM.begin(2048);
  for (uint8_t i = 0; i < NUM_CONTS; i++) {
    if (EEPROM.read(128*i+127) == '\0') {
      for (uint8_t j = 0; j < 128; j++) {
        contacts[i][j] = EEPROM.read(128*i+j);
      }
    }
    else {
      for (uint8_t j = 0; j < 128; j++) {
        contacts[i][j] = '\0';
      }
    }
    
  }
    /*uint16_t ID = 1;
    char name[12] = "John Doe";
    //messages should be 114 length
    char msg[114] = "TestMessage1  Character Counter is a 100% free online character count calculator that's simple to use. Sometimes"; //142 characters from wordcounter.net
    for (uint8_t j = 0; j < 2; j++) {
          contacts[0][j] = (ID>>((1-j)*8)) & 0xFF; //sets first 2 bytes to be the ID
      }
    for (uint8_t j = 2; j < 14; j++) {
          contacts[0][j] = name[j-2];// sets bytes 2 - 14 as the Name 
      }
    for (uint8_t j = 14; j < 156; j++){
          contacts[0][j] = msg[j-14]; // sets 14-156 as the msg 
      }
      Serial.print(contacts[0][2]);
    uint16_t ID1 = 2;
    char name1[12] = "James Smith";
    char msg1[142] = "TestMessage2  Character Counter is a 100% free online character count calculator that's simple to use. Sometimes"; //142 characters from wordcounter.net
    for (uint8_t j = 0; j < 2; j++) {
          contacts[1][j] = (ID1>>((1-j)*8)) & 0xFF; //sets first 2 bytes to be the ID
      }
    for (uint8_t j = 2; j < 14; j++) {
          contacts[1][j] = name1[j-2];// sets bytes 2 - 14 as the Name 
      }
    for (uint8_t j = 14; j < 156; j++){
          contacts[1][j] = msg1[j-14]; // sets 14-156 as the msg 
      }*/
    
  return 0;  //Serial1.write(contacts[0],156)
}

uint8_t searchContact(uint16_t ID) {
  for (uint8_t i = 0; i < NUM_CONTS;i++) {
    uint16_t currID = ((uint8_t)contacts[i][0]<<8)|((uint8_t)contacts[i][1]);
    if (currID == ID)
      return i;
  }
  return NUM_CONTS;
}

uint8_t addContact(uint16_t ID, char name[14]) {
    //store contact
    uint8_t search = searchContact(ID);
    if (search != NUM_CONTS)
      return READY;
    Serial.println("contact not found in contacts, adding");
    for(uint8_t i = 0; i < NUM_CONTS; i++) {
        if (contacts[i][1] == '\0') { //first byte of ID is blank
            for (uint8_t j = 0; j < 2; j++) {
                contacts[i][j] = (ID>>((1-j)*8)) & 0xFF; 
            }
            for (uint8_t j = 2; j < 14; j++) {
                contacts[i][j] = name[j-2];
            }
            break;
        }
    }
    //if contacts are full, bummer
    return READY;
}

void shutdownHMC() {
  for (uint8_t i = 0; i < NUM_CONTS; i++) {
    for (uint8_t j = 0; j < 128; j++) {
      EEPROM.write(128*i+j,contacts[i][j]);
    }
  }
  EEPROM.commit();
}

void waitForAck() {
    Serial1.flush();
    delay(10);
    while(true) {
        if (Serial1.available()) {
          
            uint8_t ack = Serial1.read();
            break;
        }
    }
}

uint8_t deleteContact(uint16_t ID) {
    //delete contact
    uint16_t tmpID;
    for(uint8_t i = 0; i < NUM_CONTS; i++) {
        tmpID = ((uint8_t)contacts[i][0]<<8)|((uint8_t)contacts[i][1]);
        if (tmpID == ID) { //if Id's match
            for (uint8_t j = 0; j < 16; j++) {
                contacts[i][j] = 0;
            }   
            break;
        }
    }
    Serial.println("contact deleted");
    return READY;
}

uint8_t sendContacts(uint8_t firstContact) {

    
    for (uint8_t i = 0; i < 2; i++) { //send 2 chunks of 64 bytes
         //for j=i*64,j<(i+1)*64,i++
         //serial.write contacts[index][j]
         for(uint8_t j = (i*64); j < ((i+1)*64); j++){
            Serial1.write(contacts[firstContact][j]);
         }
         Serial.println("sent chunk, waiting");
        waitForAck();
        Serial.println("got ack");
    }
    for (uint8_t i = 0; i < 2; i++) { //send 2 chunks of 64 bytes
        for(uint8_t j = (i*64); j < ((i+1)*64); j++){
            Serial1.write(contacts[firstContact + 1][j]);
         } 
        Serial.println("sent chunk, waiting.2");    
        waitForAck();
        Serial.println("got ack.2");
    }
    return READY;
}

bool messageExists(uint16_t msgID) {
  // 1 means message exists
  //check LOGS, if msgID is already in LOGS then return 1 else return 0
  for(uint16_t point = 0; point < NUM_LOG; point++){
    if(logs[point] == msgID){
      return 1;//found id TRUE
    }
  }
  return 0;
}


void logMessageID(uint16_t msgID) {
    for(uint8_t i = 0; i < NUM_LOG; i++) {
        if (logs[i] == 0) {
            logs[i] = msgID;
            return;
        }
    }
    
    for(uint8_t i = 1; i < NUM_LOG; i++) {
        logs[i-1] = logs[i];
    }
    logs[NUM_LOG-1] = msgID;
    return;
}



uint16_t getID(char message[], uint8_t ind) {
    uint16_t ID = ((uint8_t)message[ind]<<8)|((uint8_t)message[ind+1]);
    return ID;
}

//sending the information to the RFM95W
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
    for (int i = 0; i < 114;i++) {
        LoRa.write(msg[i]); //need to do randomID
    }
    LoRa.endPacket();
    LoRa.receive();
    return;
}

void writeMessage() {
    Serial1.write(MESSAGE_REC); //send message received and wait for acknowledgement

    waitForAck();
    Serial.println("got past ack!");
    for (uint8_t i = 0; i < 2; i++) { //send 4 chunks of 64 bytes
        for (uint16_t j = i*64; j < (i+1)*64; j++) {
            if (j >= 114)
              Serial1.write('\0');
            else
              Serial1.write(messageToDisplay[j]); //need to make 144 comply
        }
        waitForAck();
    }
    return;
}

void handleMessage(uint16_t S_ID, char msg[])
{
    
    
    
    //send message to pro mini
    for (uint8_t j = 0; j < 2; j++) {
        messageToDisplay[j] = (S_ID>>((1-j)*8)) & 0xFF;
    }
    uint8_t ind = searchContact(S_ID);
    if (ind == NUM_CONTS) {
      char unknown[12] = "Unknown ID";
      for (uint8_t j = 2; j < 14; j++) {
        messageToDisplay[j] = unknown[j-2];
      }
    }
    else {
      for (uint8_t j = 2; j < 14; j++) {
          messageToDisplay[j] = contacts[ind][j];
      }
    }
    for (uint8_t j = 14; j < 128; j++) {
        messageToDisplay[j] = msg[j-14];
    }

    //store message with index found by search
    if (ind != NUM_CONTS) { //contact not found, dont store
      for (uint8_t i = 14; i < 128; i++) {
        contacts[ind][i] = msg[i-14];
      }

    }
    return;
}

void parseMessage() {
    handlingMessage = false;
    Serial.println("got a message!");
   uint16_t networkTot = getID(rawPacket, 0);
    uint16_t msgIdTot = getID(rawPacket, 2);
    uint16_t recipientTot = getID(rawPacket, 4);
    uint16_t senderTot = getID(rawPacket, 6);
    char messageTot[114] = {0};
    for (uint8_t i = 0; i < 114; i++) {
        messageTot[i] = rawPacket[i+8];
    }

    Serial.print("R_ID: ");
    Serial.println(recipientTot);
    Serial.print("N_ID: ");
    Serial.println(networkTot);
    Serial.print("S_ID: ");
    Serial.println(senderTot);
    Serial.print("M_ID: ");
    Serial.println(msgIdTot);
    Serial.println(messageTot);
    //at this part we have 8 bytes for N_ID/M_ID/R_ID/S_ID
    //and 114 bytes for message :)
    uint16_t networkID = NETWORK_ID;
    uint16_t deviceID = DEVICE_ID;
    /*Serial1.write(MESSAGE_REC); //send message received and wait for acknowledgement
    Serial1.flush();
    delay(500);*/
    
    if(networkTot != networkID) { //if the network is not ours leave the function
        Serial.println("network ID didnt match");
        return;
    }
    if(messageExists(msgIdTot)) { //if the message is in our memory leave the function
        Serial.println("message existed in logs");
        return;
    }
    else
    {
        Serial.println("message didnt exist in logs, logging it");
        logMessageID(msgIdTot);//if message is new then we log the ID in memory
    }
    if(recipientTot != deviceID) {
        Serial.println("we aren't the intended address... retransmitting");
        sendMessage(networkTot,msgIdTot,recipientTot,senderTot,messageTot); // if the message is not for us rebroadcast with all the stuff we have
    }
    else
    {
      Serial.println("we are the intended adress... printing message");
      handleMessage(senderTot,messageTot); // if the message is for us we want to save the sender ID and the message contents
      writeMessage();//this is probably borken
    }
}
void receiveMessage( int packetSize ) { //can only Lora.read() one byte at a time
    handlingMessage = true;
    for (uint8_t i = 0; i < 122; i++) {
        rawPacket[i] = LoRa.read();
    }

   
    
    

    //}
    return;
}


uint8_t handleUART(uint8_t handleCode) {
    uint8_t tmp_handler = READY;
    Serial.print("handling code: ");
    Serial.println(handleCode); 
    switch(handleCode) {
        case MSG_SEND: { //xiao sends message received
            Serial.println("got send request");
            tmp_handler = SEND_N1;
            expectedSerial = 64;
            Serial1.write(ACK);
            break;
        }
        case ADD_CONT: {
            Serial.println("got add cont code");
            tmp_handler = ADD_CONT;
            expectedSerial = 14;
            Serial1.write(ACK);
            break;
        }
        case DEL_CONT: {
            Serial.println("got delete cont code");
            tmp_handler = DEL_CONT;
            expectedSerial = 2;
            Serial1.write(ACK);
            break;
        }
        case REQ_CONTS: {
            Serial.println("got req conts code");
            tmp_handler = REQ_CONTS;
            expectedSerial = 1;
            Serial1.write(ACK);
            break;
        }
        case ACK: {
          if (handlingMessage) {
            Serial.println("got ack from msg_rec");
          }
          break;
        }
        case SHUTDOWN: {
          shutdownHMC();
          Serial1.write(ACK);
          break;
        }
        case BATT: {
          Serial1.write(batteryPercentage());
          break;
        }
        default: {Serial.println("UART command did not match");}
    }
    return tmp_handler;
}


void evaluatePipe () {
    Serial.print("evaluating pipe with handler=");
    Serial.println(handler);
    switch(handler) {
        case SEND_N1: {
            for (uint8_t i = 0; i < 64; i++) {
                messageToSend[i] = serialPipe[63-i]; //need to comply with 144
            }
            Serial.println("got first chunk");
            serialCounter = 0;
            handler = SEND_N2;
            Serial1.write(ACK);
            break;
        }
        case SEND_N2: {
            for (uint8_t i = 0; i < 50; i++) {
                messageToSend[i+64] = serialPipe[63-i];
            }
             Serial.println("got second chunk");
           uint16_t N_ID = NETWORK_ID;
            uint16_t M_ID = randomGen();//TO-DO: this
            uint16_t R_ID = getID(messageToSend,0);
            uint16_t S_ID = DEVICE_ID;
            char message[114] = {0};
            for (uint8_t i = 0; i < 114; i++) {
                message[i] = messageToSend[i+2];
            }
            Serial.print("N_ID: ");
            Serial.println(N_ID);
            Serial.print("M_ID: ");
            Serial.println(N_ID);
            Serial.print("R_ID: ");
            Serial.println(R_ID);
            Serial.print("S_ID: ");
            Serial.println(S_ID);
            Serial.println(message);
            sendMessage(N_ID, M_ID, R_ID, S_ID, message);
            handler = READY;
            expectedSerial = 1;
            serialCounter = 0;
            Serial1.write(ACK);
            break;
        }
        case ADD_CONT: {
            uint16_t contactID = 0;
            char contactName[12] = {0};
            char tmpID[2] = {0};
            for(uint8_t i =0;i<2;i++) //get ID
            {
                tmpID[i] = serialPipe[13-i];
            }
            contactID = ((uint8_t)tmpID[0]<<8)|((uint8_t)tmpID[1]);
            for(uint8_t i =0;i<12;i++) //get Name
            {
                contactName[i] = serialPipe[11-i];
            }
            Serial.print("adding contact with ID: ");
            Serial.print(contactID);
            Serial.print(" and name: ");
            Serial.println(contactName);
            handler = addContact(contactID, contactName);
            expectedSerial = 1;
            serialCounter = 0;
            break;
        }
        case DEL_CONT: {
            Serial.print("deleting contact with ID: ");
            uint16_t contactID = ((uint8_t)serialPipe[1]<<8)|((uint8_t)serialPipe[0]);
            Serial.println(contactID);
            handler = deleteContact(contactID);
            expectedSerial = 1;
            serialCounter = 0;
            break;
        }
        case REQ_CONTS: {
            Serial.print("got index ");
            uint8_t firstContact = serialPipe[0];
            Serial.println(firstContact);
            handler = sendContacts(firstContact);
            expectedSerial = 1;
            serialCounter = 0;
            handler = READY;
            Serial1.write(ACK);
            Serial.println("req conts done");
           break;
        }
        case READY: {
            uint8_t handleCode = serialPipe[0];
            handler = handleUART(handleCode);
            serialCounter = 0;
            break;
        }
        default: ;
            
    }
}


void processByte() {
    
    //shift right arrray for loop
    //serial.read into [0]
    //increase serial coutner
    
    for(int i = 63; i > 0; i--)
    {
        serialPipe[i] = serialPipe[i-1]; // right shifts data
    }
    serialPipe[0] = Serial1.read();//reads first element
    Serial.print(serialPipe[0]);
    Serial.print(" with currrent expected length of: ");
    Serial.println(expectedSerial);
    serialCounter++;
    return;
}

int setupHMC() {

    LoRa.setPins(D3, D2, D1);
    //set spreading factor?
    if (!LoRa.begin(CARRIER_FREQ)) {             // initialize at 915MHz
        return 0; //if LoRa setup failed
    }

    LoRa.onReceive(receiveMessage); //messageReceive is the callback function when receiving a message

    LoRa.receive(); // turn on receive mode

    return 1;
}

int setupSerial() {
    Serial1.begin(BAUD_RATE);
    

    Serial1.write(BOOT);
    Serial.println("xiao sent boot signal");
    while (true) { //wait for handshake
        if (Serial1.available()) {
            uint8_t ack = Serial1.read();
            uint8_t boot = BOOT;
            Serial.print("xiao received boot signal: ");
            Serial.println(ack);
            if (ack == boot) {
              Serial.println("it was the correct boot signal");
              break;
            }
            else {
              Serial1.write(BOOT);
              Serial.println("it wasn't the correct boot signal, resending...");
            }
        }
    }
    Serial1.write(BOOT);
    Serial.println("xiao sent final boot signal");
    return 1;
}
