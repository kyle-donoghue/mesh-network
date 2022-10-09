#include <stdint.h>
#include "LoraConfig.h"


int setupHMC() {
    LoRa.setPins(csPin, resetPin, irqPin);
    //set spreading factor?
    if (!LoRa.begin(CARRIER_FREQ)) {             // initialize at 915MHz
        //Serial.println("LoRa init failed. Check your connections.");
        return 0; //if LoRa setup failed
    }

    Lora.onReceive(receiveMessage); //messageReceive is the callback function when receiving a message

    LoRa.receive(); // turn on receive mode

    return 1;
}

int setupSerial() {
    Serial.begin(BAUD_RATE, SERIAL_8E2);

    Serial.write(BOOT);
    while (true) { //wait for handshake
        if (Serial.Available()) {
            uint8_t ack = Serial.Read();
            break;
        }
    }
    Serial.write(BOOT);
    return 1;
}

void receiveMessage( int packetSize ) { //can only Lora.read() one byte at a time
    
    if(packetSize == 0) return;
    uint32_t senderTot = 0;
    uint32_t msgIdTot = 0;
    uint32_t recipientTot = 0;
    uint32_t networkTot = 0;
    char messageTot[240] = 0;
    for(int i =0;i<4;i++)
    {
        networkTot += LoRa.read();
        if (i < 3)
            networkTot = networkTot<<8;
    }
    for(int i =0;i<4;i++)
    {
        msgIdTot += LoRa.read();
        if (i < 3)
            msgIdTot = msgIdTot<<8;
    }
    for(int i =0;i<4;i++)
    {
        recipientTot += LoRa.read();
        if (i < 3)
            recipientTot = recipientTot<<8;
    }
    for(int i =0;i<4;i++)
    {
        senderTot += LoRa.read();
        if (i < 3)
            senderTot = senderTot<<8;
    }
    for(int i =0;i<240;i++)
    {
        messageTot[i] = LoRa.read();
    }
    //at this part we have 4 bytes for N_ID/M_ID/R_ID/S_ID
    //and 240 bytes for message :)
    if(networkTot != NETWORK_ID) //if the network is not ours leave the function
        return;
    if(messageExists(msgIdTot)) //if the message is in our memory leave the function
        return;
    else
    {
        logMessageID(msgIdTot);//if message is new then we log the ID in memory
    }
    if(recipientTot != DEVICE_ID)
        sendMessage(networkTot,msgIdTot,recipientTot,senderTot,messageTot); // if the message is not for us rebroadcast with all the stuff we have
    else
    {
        handleMessage(senderTot,messageTot) // if the message is for us we want to save the sender ID and the message contents
    }
    return;
}

void receiveUART (int packetSize) {
    switch(handler) {
        case SEND_N1:
            for (uint8_t i = 0; i < 64; i++) {
                messageToSend[i] = Serial.Read();
            }
            Serial.Write(ACK);
            handler = SEND_N2;
            break;
        case SEND_N2:
            for (uint8_t i = 64; i < 128; i++) {
                messageToSend[i] = Serial.Read();
            }
            Serial.Write(ACK);
            handler = SEND_N3;
            break;
        case SEND_N3:
            for (uint8_t i = 128; i < 192; i++) {
                messageToSend[i] = Serial.Read();
            }
            Serial.Write(ACK);
            handler = SEND_N4;
            break;
        case SEND_N4:
            for (uint8_t i = 192; i < 256; i++) {
                messageToSend[i] = Serial.Read();
            }
            uint32_t N_ID = getID(messageToSend,0);
            uint32_t M_ID = getID(messageToSend,4);
            uint32_t R_ID = getID(messageToSend,8);
            uint32_t S_ID = getID(messageToSend,12);
            char message[240] = {0};
            for (uint8_t i = 0; i < 240) {
                message[i] = messageToSend[i+16];
            }
            sendMessage(N_ID, M_ID, R_ID, S_ID, message);
            Serial.Write(ACK);
            handler = READY;
            expectedSerial = 1;
            break;
        case ADD_CONT:
            uint32_t contactID = 0;
            char contactName[12] = {0};
            for(uint8_t i =0;i<4;i++) //get ID
            {
                contactID += Serial.read();
                if (i < 3)
                    contactID = contactID<<8;
            }
            for(uint8_t i =0;i<12;i++) //get Name
            {
                contactName[i] = Serial.read();
            }
            handler = addContact(contactID, contactName);
            expectedSerial = 1;
            break;
        case DEL_CONT:
            uint32_t contactID = 0;
            for(int i =0;i<4;i++) //get ID
            {
                contactID += Serial.read();
                if (i < 3)
                    contactID = contactID<<8;
            }
            handler = deleteContact(contactID);
            expectedSerial = 1;
            break;
        case CONTS:
            uint_8 firstContact = Serial.read();
            handler = sendContacts(firstContact);
            expectedSerial = 1;
           break;
        case MSG_DISP:
            uint_8 numMessage = Serial.read();
            handler = displayMessage(numMessage);
            expectedSerial = 1;
           break;
        case READY:
            int handleCode = Serial.read();
            handler = handleUART(handleCode);
            break;
        default: ;
            //cry
    }
}

uint8_t handleUART(handleCode) {
    switch(handleCode) {
        case MESSAGE_SEND: //xiao sends message received
            handler = SEND_N1;
            expectedSerial = 64;
            break;
        case ADD_CONT:
            handler = ADD_CONT;
            expectedSerial = 12;
            break;
        case DEL_CONT:
            handler = DEL_CONT;
            expectedSerial = 4;
            break;
        case REQ_CONTS:
            handler = CONTS;
            expectedSerial = 1;
            break;
        case MSG_DISP:
            handler = MSG_DISP;
            expectedSerial = 1;
            break;
        default: ;
    }
}

uint32_t getID(char message[], uint8_t ind) {
    uint32_t ID = 0;
    for (uint8_t i = ind; i < ind+4; i++) {
        ID += message[i];
        if (i < ind+3)
            ID = ID<<8;
    }
}

void logMessageID(int msgID) {
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

//sending the information to our second device
void sendMessage(uint32_t N_ID,uint32_t M_ID,uint32_t R_ID,uint32_t S_ID,char msg[])
{
    LoRa.beginPacket();
    for (int i = 3; i >= 0;i--) {
        LoRa.Write((N_ID >> (8*i)) & 0xFF);
    }
    for (int i = 3; i >= 0;i--) {
        LoRa.Write((M_ID >> (8*i)) & 0xFF);
    }
    for (int i = 3; i >= 0;i--) {
        LoRa.Write((R_ID >> (8*i)) & 0xFF);
    }
    for (int i = 3; i >= 0;i--) {
        LoRa.Write((S_ID >> (8*i)) & 0xFF);
    }
    for (int i = 239; i >= 0;i--) {
        LoRa.Write(msg[i]);
    }
    LoRa.endPacket();
    return;
}

void handleMessage(uint32_t S_ID, char msg[])
{
    //store message
    for(uint16_t i = 0; i < NUM_MSG; i++) {
        if (messages[i][4] == '\0') { //first byte of sender ID is blank
            for (uint8_t j = 0; j < 4; j++) {
                messages[i][j] = (S_ID << (8*j)) & 0xFF000000; 
            }
            for (uint8_t j = 4; j < 244; j++) {
                messages[i][j] = msg[j-4];
            }
            return;
        }
    }
    //if messages are full
    for (uint16_t i = 1; i < NUM_MSG; i++) {
        for (uint8_t j = 0; j < 244; j++) {
            messages[i-1][j] = messages[i][j];
        }
    }
    //store message at last row
    for (uint8_t j = 0; j < 4; j++) {
        messages[NUM_MSG-1][j] = (S_ID << (8*j)) & 0xFF000000; 
    }
    for (uint8_t j = 4; j < 244; j++) {
        messages[NUM_MSG-1][j] = msg[j-4];
    }

    //send message to pro mini
    for (uint8_t j = 0; j < 4; j++) {
        messageToSend[j] = (S_ID << (8*j)) & 0xFF000000; 
    }
    for (uint8_t j = 4; j < 244; j++) {
        messageToSend[j] = msg[j-4];
    } //TO-DO: do we limit characters to 188 to make 3 transfers?
    writeMessage(messageToSend);

    return;
}

void writeMessage(char messageToSend[256]) {
    Serial.Write(MESSAGE_REC); //send message received and wait for acknowledgement
    waitForAck();
    for (uint8_t i = 0; i < 4; i++) { //send 4 chunks of 64 bytes
        for (uint16_t j = i*64; j < (i+1)*64; j++) {
            Serial.Write(messageToSend[j]);
        }
        waitForAck();
    }
    return;
}

void waitForAck() {
    while(true) {
        if (Serial.Available()) {
            uint8_t ack = Serial.Read();
            break;
        }
    }
}

uint8_t addContact(uint32_t ID, char name[12]) {
    //store contact
    for(uint16_t i = 0; i < NUM_CONTS; i++) {
        if (contacts[i][4] == 0) { //first byte of ID is blank
            for (uint8_t j = 0; j < 4; j++) {
                contacts[i][j] = (ID << (8*j)) & 0xFF000000; 
            }
            for (uint8_t j = 4; j < 16; j++) {
                contacts[i][j] = name[j-4];
            }
            break;
        }
    }
    //if contacts are full, bummer
    return READY;
}

uint8_t deleteContact(uint32_t ID) {
    //delete contact
    uint32_t tmpID;
    for(uint16_t i = 0; i < NUM_CONTS; i++) {
        tmpID = 0;
        for (uint8_t j = 0; j < 4; j++) { //get ID of current row
            tmpID += contacts[i][j];
            if (i < 3)
                tmpID = tmpID<<8;
        }
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
                Serial.Write(messages[numMessage][j]);
            }
            else { //if we are over the 244 byte message storage limite
                Serial.Write(0);
            }
        }
        waitForAck();
    }
    return READY;

}

uint8_t sendContacts(uint8_t firstContact) {

    for (uint8_t i = firstContact; i < firstContact+4;i++) {
        if (i < NUM_CONTS) {
            for (uint8_t j = 0; j < 16; j++) {
                Serial.Write(contacts[i][j]);
            }
        }
        else {
            for (uint8_t j = 0; j < 16; j++) {
                Serial.Write(0);
            } 
        }
    }

    return READY;
}

void initializeContacts() {
    //get contacts from EEPROM on startup
}