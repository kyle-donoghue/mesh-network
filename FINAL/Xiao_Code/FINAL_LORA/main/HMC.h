#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <SPI.h>
#include "LoraDefine.h"
#include <LoRa.h>




char messages[NUM_MSG][144] = {""}; // \0 //messages[0][0] = message1;
uint16_t logs[NUM_LOG] = {0};
char contacts[NUM_CONTS][16] = {0};
char messageToDisplay[144] = {0};
uint8_t handler = READY;
uint8_t expectedSerial = 1; //1 for waiting for code or ack; 64 for most other cases
char messageToSend[150] = {0};


uint16_t randomID(uint16_t seed) { //TO-DO use lora random
    srand(seed);
    uint16_t num = rand();
    return num;
}

uint8_t initializeContacts() {
  return 0;  
}

uint8_t addContact(uint16_t ID, char name[14]) {
    //store contact
    for(uint8_t i = 0; i < NUM_CONTS; i++) {
        if (contacts[i][1] == 0) { //first byte of ID is blank
            for (uint8_t j = 0; j < 2; j++) {
                contacts[i][j] = (ID>>((1-j)*8)) & 0xFF; 
            }
            for (uint8_t j = 2; j < 16; j++) {
                contacts[i][j] = name[j-2];
            }
            break;
        }
    }
    //if contacts are full, bummer
    return READY;
}

void waitForAck() {
    while(true) {
        if (Serial.available()) {
            uint8_t ack = Serial.read();
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
    
    return READY;
}

uint8_t displayMessage(uint8_t numMessage) {
    for (uint8_t i = 0; i < 4; i++) { //send 4 chunks of 64 bytes
        for (uint16_t j = i*64; j < (i+1)*64; j++) {
            if (j < 244) {
                Serial.write(messages[numMessage][j]); //should change to write('',n)
            }
            else { //if we are over the 244 byte message storage limite
                Serial.write((byte) 0x00);
            }
        }
        waitForAck();
    }
    return READY;

}

uint8_t sendContacts(uint8_t firstContact) {

    for (uint8_t i = firstContact; i < firstContact+2;i++) {
        if (i < NUM_CONTS) {
            for (uint8_t j = 0; j < 16; j++) {
                Serial.write(contacts[i][j]); //should change to write('',n)
            }
        }
        else {
            for (uint8_t j = 0; j < 16; j++) {
                Serial.write((byte) 0x00);
            } 
        }
    }

    return READY;
}

bool messageExists(uint16_t msgID) {
  return 0;
}


void logMessageID(uint16_t msgID) {
    for(int i = 0; i < NUM_LOG; i++) {
        if (logs[i] == 0) {
            logs[i] = msgID;
            return;
        }
    }
    for(int i = 1; i < NUM_LOG; i++) {
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
    for (int i = 0; i < 142;i--) {
        LoRa.write(msg[i]); //need to do randomID
    }
    LoRa.endPacket();
    LoRa.receive();
    return;
}

void writeMessage(char messageToSend[144]) {
    Serial.write(MESSAGE_REC); //send message received and wait for acknowledgement
    waitForAck();
    for (uint8_t i = 0; i < 3; i++) { //send 4 chunks of 64 bytes
        for (uint16_t j = i*64; j < (i+1)*64; j++) {
            Serial.write(messageToSend[j]); //need to make 144 comply
        }
        waitForAck();
    }
    return;
}

void handleMessage(uint16_t S_ID, char msg[])
{
    //store message
    for(uint8_t i = 0; i < NUM_MSG; i++) {
        if (messages[i][1] == 0) { //first byte of sender ID is blank
            for (uint8_t j = 0; j < 2; j++) {
                messages[i][j] = (S_ID>>((1-j)*8)) & 0xFF; 
            }
            for (uint8_t j = 2; j < 144; j++) {
                messages[i][j] = msg[j-2];
            }
            return;
        }
    }
    //if messages are full
    for (uint8_t i = 1; i < NUM_MSG; i++) {
        for (uint8_t j = 0; j < 244; j++) {
            messages[i-1][j] = messages[i][j];
        }
    }
    //store message at last row
    for (uint8_t j = 0; j < 2; j++) {
        messages[NUM_MSG-1][j] = (S_ID>>((1-j)*8)) & 0xFF; 
    }
    for (uint8_t j = 2; j < 144; j++) {
        messages[NUM_MSG-1][j] = msg[j-2];
    }

    //send message to pro mini
    for (uint8_t j = 0; j < 2; j++) {
        messageToDisplay[j] = (S_ID>>((1-j)*8)) & 0xFF; 
    }
    for (uint8_t j = 2; j < 144; j++) {
        messageToDisplay[j] = msg[j-2];
    }
    writeMessage(messageToDisplay);

    return;
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
    //at this part we have 4 bytes for N_ID/M_ID/R_ID/S_ID
    //and 142 bytes for message :)
    uint16_t networkID = NETWORK_ID;
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
        handleMessage(senderTot,messageTot); // if the message is for us we want to save the sender ID and the message contents
    }
    return;
}


uint8_t handleUART(uint8_t handleCode) {
    uint8_t tmp_handler = 0;
    switch(handleCode) {
        case MSG_SEND: //xiao sends message received
            tmp_handler = SEND_N1;
            expectedSerial = 64;
            break;
        case ADD_CONT:
            tmp_handler = ADD_CONT;
            expectedSerial = 12;
            break;
        case DEL_CONT:
            tmp_handler = DEL_CONT;
            expectedSerial = 4;
            break;
        case REQ_CONTS:
            tmp_handler = REQ_CONTS;
            expectedSerial = 1;
            break;
        case MSG_DISP:
            tmp_handler = MSG_DISP;
            expectedSerial = 1;
            break;
        default: ;
    }
    return tmp_handler;
}

void receiveUART () {
    switch(handler) {
        case SEND_N1: {
            for (uint8_t i = 0; i < 64; i++) {
                messageToSend[i] = Serial.read(); //need to comply with 144
            }
            Serial.write(ACK);
            handler = SEND_N2;
            break;
        }
        case SEND_N2: {
            for (uint8_t i = 64; i < 128; i++) {
                messageToSend[i] = Serial.read();
            }
            Serial.write(ACK);
            handler = SEND_N3;
            break;
        }
        case SEND_N3: {
            for (uint8_t i = 128; i < 192; i++) {
                messageToSend[i] = Serial.read();
            }
            Serial.write(ACK);
            handler = SEND_N4;
            break;
        }
        case SEND_N4: {
            for (uint8_t i = 192; i < 256; i++) {
                messageToSend[i] = Serial.read();
            }
            uint16_t N_ID = getID(messageToSend,0);
            uint16_t M_ID = getID(messageToSend,2);
            uint16_t R_ID = getID(messageToSend,4);
            uint16_t S_ID = getID(messageToSend,5);
            char message[142] = {0};
            for (uint8_t i = 0; i < 150; i++) {
                message[i] = messageToSend[i+8];
            }
            sendMessage(N_ID, M_ID, R_ID, S_ID, message);
            Serial.write(ACK);
            handler = READY;
            expectedSerial = 1;
            break;
        }
        case ADD_CONT: {
            uint16_t contactID = 0;
            char contactName[12] = {0};
            char tmpID[2] = {0};
            for(uint8_t i =0;i<2;i++) //get ID
            {
                tmpID[i] = Serial.read();
            }
            contactID = ((uint8_t)tmpID[0]<<8)|((uint8_t)tmpID[1]);
            for(uint8_t i =0;i<14;i++) //get Name
            {
                contactName[i] = Serial.read();
            }
            handler = addContact(contactID, contactName);
            expectedSerial = 1;
            break;
        }
        case DEL_CONT: {
            uint16_t contactID = 0;
            char tmpID[2] = {0};
            for(uint8_t i =0;i<2;i++) //get ID
            {
                tmpID[i] = Serial.read();
            }
            contactID = ((uint8_t)tmpID[0]<<8)|((uint8_t)tmpID[1]);
            handler = deleteContact(contactID);
            expectedSerial = 1;
            break;
        }
        case REQ_CONTS: {
            uint8_t firstContact = Serial.read();
            handler = sendContacts(firstContact);
            expectedSerial = 1;
           break;
        }
        case MSG_DISP: {
            uint8_t numMessage = Serial.read();
            handler = displayMessage(numMessage);
            expectedSerial = 1;
           break;
        }
        case READY: {
            uint8_t handleCode = Serial.read();
            handler = handleUART(handleCode);
            break;
        }
        default: ;
            
    }
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
    Serial.begin(BAUD_RATE, SERIAL_8E2);

    Serial.write(BOOT);
    while (true) { //wait for handshake
        if (Serial.available()) {
            uint8_t ack = Serial.read();
            break;
        }
    }
    Serial.write(BOOT);
    return 1;
}
