#include<stdint.h>

#define NUM_MSG 10 //change to 100 later
#define BAUD_RATE 250000
#define NUM_LOG 100

const int csPin = 7;
const int resetPin = 6;
const int irqPin = 1;  //THESE NEED TO BE CHANGED

const int deviceID = 594385720; //randomly-chosen ID for this device
const int networkID = 11122333; //networkID of our intials: K,L,W,C,C

char messages[NUM_MSG][244] = {""}; // \0 //messages[0][0] = message1;
uint32_t messagePlaceholder = 0;
int logs[NUM_LOG] = {0};

int setupHMC() {
    LoRa.setPins(csPin, resetPin, irqPin);
    //set spreading factor?
    if (!LoRa.begin(915E6)) {             // initialize at 915MHz
        //Serial.println("LoRa init failed. Check your connections.");
        return 0; //if LoRa setup failed
    }

    Lora.onReceive(receiveMessage); //messageReceive is the callback function when receiving a message

    LoRa.receive(); // turn on receive mode

    return 1;
}

int setupSerial() {
    Serial.begin(BAUD_RATE);
    //do handshake
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
    }
    return;
}

void receiveUART (int packetSize) {
    static uint8_t handler = READY;
    static char messageToSend[256] = {0};
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
            sendMessage(messageToSend);
            Serial.Write(ACK);
            handler = READY;
            break;
        case ADD_CONT:
            uint32_t contactID = 0;
            for(int i =0;i<4;i++)
            {
                contactID += Serial.read();
                if (i < 3)
                    contactID = contactID<<8;
            }
            handler = editContacts(contactID,ADD_CODE);
            break;
        case DEL_CONT:
            uint32_t contactID = 0;
            for(int i =0;i<4;i++)
            {
                contactID += Serial.read();
                if (i < 3)
                    contactID = contactID<<8;
            }
            handler = editContacts(contactID,DEL_CODE);
            break;
        case CONTS:
            uint_8 firstContact = Serial.read();
            uint_8 secondContact = Serial.read();
            handler = sendContacts(firstContact, secondContact);
            break;
        case READY:
            int handleCode = Serial.read();
            handler = handleUART(handleCode);
            break;
        default: ;
            //cry
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
    logs[NUM_LOG] = msgID;
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
    for(int i = 0; i < NUM_MSG; i++) {
        if (messages[i][0] == '\0') {
            for (int j = 0; j < 4; j++) {
                messages[i][j] = (S_ID << (8*j)) & 0xFF000000; 
            }
            for (int j = 4; j < 244; j++) {
                messages[i][j] = msg[j-4];
            }
            return;
        }
    }

    if (messagePlaceholder == NUM_MSG-1)
        messagePlaceholder = 0;
    else
        messagePlaceholder++;



    for (int j = 0; j < 4; j++) {
        messages[messagePlaceholder][j] = (S_ID << (8*j)) & 0xFF000000; 
    }
    for (int j = 4; j < 244; j++) {
        messages[messagePlaceholder][j] = msg[j-4];
    }

    
    logs[NUM_LOG] = msgID;
    return;
}