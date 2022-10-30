#include <Elegoo_GFX.h>    // Core graphics library
#include <Elegoo_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>

#include "ScreenConfig.h"
#include "ScreenDrawings.h"



TSPoint getPress() {
    digitalWrite(13,HIGH);
    TSPoint p = ts.getPoint();
    digitalWrite(13,LOW);
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    p.x = map(p.x, TS_MINX, TS_MAXX, tft.height(), 0);
    p.y = (map(p.y, TS_MINY, TS_MAXY, tft.width(), 0));
    int tmp = p.x;
    p.x = p.y;
    p.y = tmp;
    return p;
}

bool isPressed(int16_t z) {
    if ( (z < MAX_PRESS) && (z > MIN_PRESS) )
        return 1;
    return 0;
}

void waitForAck() {
  while (true) { //wait for handshake
        if (Serial.available()) {
            uint8_t ack = Serial.read();
            break;
        }
    }
}

uint16_t getButton(int x, int y) {
    switch(currentScreen) {
        case MAIN_SCREEN_CODE:
            for(int i = 0; i < MAIN_SCREEN_BUTTON_COUNT; i++){

                if( !((x >= mainScreenButtons[i][1]) && (x <= mainScreenButtons[i][1]+mainScreenButtons[i][3])) ){ // if x , then go to contacts/logs
                    continue;
                }
                if( !((y >= mainScreenButtons[i][2]) && (y <= mainScreenButtons[i][2]+mainScreenButtons[i][4])) ){ // if y matches, then go to contacts/logs
                    continue;
                }
                return mainScreenButtons[i][0];
            }
            return 0;
        case CONTACTS_SCREEN_CODE:
            for(int i = 0; i < CONTACTS_SCREEN_BUTTON_COUNT; i++){

                if( !((x >= contactsScreenButtons[i][1]) && (x <= contactsScreenButtons[i][1]+contactsScreenButtons[i][3])) ){ // if x , then go to contacts/logs
                    continue;
                }
                if( !((y >= contactsScreenButtons[i][2]) && (y <= contactsScreenButtons[i][2]+contactsScreenButtons[i][4])) ){ // if y matches, then go to contacts/logs
                    continue;
                }
                return contactsScreenButtons[i][0];
            }
            return 0;
        case LOG_SCREEN_CODE:
            for(int i = 0; i < LOG_SCREEN_BUTTON_COUNT; i++){

                if( !((x >= logScreenButtons[i][1]) && (x <= logScreenButtons[i][1]+logScreenButtons[i][3])) ){ // if x , then go to contacts/logs
                    continue;
                }
                if( !((y >= logScreenButtons[i][2]) && (y <= logScreenButtons[i][2]+logScreenButtons[i][4])) ){ // if y matches, then go to contacts/logs
                    continue;
                }
                return logScreenButtons[i][0];
            }
            return 0;
        case COMPOSE_SCREEN_CODE:
            for(int i = 0; i < COMPOSE_SCREEN_BUTTON_COUNT; i++){

                if( !((x >= composeScreenButtons[i][1]) && (x <= composeScreenButtons[i][1]+composeScreenButtons[i][3])) ){ // if x , then go to contacts/logs
                    continue;
                }
                if( !((y >= composeScreenButtons[i][2]) && (y <= composeScreenButtons[i][2]+composeScreenButtons[i][4])) ){ // if y matches, then go to contacts/logs
                    continue;
                }
                return composeScreenButtons[i][0];
            }
            return 0;
        case RECEIVED_SCREEN_CODE:
            for(int i = 0; i < RECEIVED_SCREEN_BUTTON_COUNT; i++){

                if( !((x >= receivedScreenButtons[i][1]) && (x <= receivedScreenButtons[i][1]+receivedScreenButtons[i][3])) ){ // if x , then go to contacts/logs
                    continue;
                }
                if( !((y >= receivedScreenButtons[i][2]) && (y <= receivedScreenButtons[i][2]+receivedScreenButtons[i][4])) ){ // if y matches, then go to contacts/logs
                    continue;
                }
                return receivedScreenButtons[i][0];
            }
            return 0;
        default: ;
    }
    return 0;
}
int handleUART(uint8_t handleCode) {
    switch(handleCode) {
        case MESSAGE_REC: //xiao sends message received
            handler = RECEIVE_N1;
            expectedSerial = 64;
            Serial.write(ACK);
            break;
        default: ;
    }
}

void receiveUART () {
    switch(handler) {
        case RECEIVE_N1:
            for (uint8_t i = 0; i < 64; i++) {
                messageToReceive[i] = Serial.read();
            }
            Serial.write(ACK);
            handler = RECEIVE_N2;
            break;
        case RECEIVE_N2:
            for (uint8_t i = 64; i < 128; i++) {
                messageToReceive[i] = Serial.read();
            }
            Serial.write(ACK);
            handler = RECEIVE_N3;
            break;
        case RECEIVE_N3:
            for (uint8_t i = 128; i < 192; i++) {
                messageToReceive[i] = Serial.read();
            }
            Serial.write(ACK);
            handler = RECEIVE_N4;
            break;
        case RECEIVE_N4:
            for (uint8_t i = 192; i < 256; i++) {
                messageToReceive[i] = Serial.read();
            }
            switch(messageHandler) { //if you get here, just know that i want to kill myself
                case MSG_TO_ME:
                    messageReceived(messageToReceive);
                    messageHandler = MSG_TO_ME;
                    handler = READY;
                    Serial.write(ACK);
                    break;
                case MSG_LOG1:
                    displayMessage(messageToReceive,1);
                    messageHandler = MSG_LOG2;
                    handler = RECEIVE_N1; //TO-DO: make sure messagehandler is set when requesting messages
                    break;
                case MSG_LOG2:
                    displayMessage(messageToReceive,2);
                    Serial.write(ACK);
                    messageHandler = MSG_LOG3;
                    handler = RECEIVE_N1;
                    break;
                case MSG_LOG3:
                    displayMessage(messageToReceive,3);
                    Serial.write(ACK);
                    messageHandler = MSG_LOG4;
                    handler = RECEIVE_N1;
                    break;
                case MSG_LOG4:
                    displayMessage(messageToReceive,4);
                    Serial.write(ACK);
                    messageHandler = MSG_TO_ME;
                    handler = RECEIVE_N1;
                    break;
                default: ;
            }
            Serial.write(ACK);
            handler = READY;
            expectedSerial = 1;
            break;
        case CONTS_REC:
            for (uint8_t i = 0; i<4;i++ ) {
                currentContactIDs[i] = 0;
                for (uint8_t j = 0; j<4;j++) { //get byte read ID's and store in array
                    currentContactIDs[i] += Serial.read();
                    if (i < 3)
                        currentContactIDs[i] = currentContactIDs[i] << 8;
                }
                for (uint8_t j = 0; j<12;j++) { //get byte read characters and store in array
                    currentContactNames[i][j] = Serial.read();
                }
            }
            displayContacts();
            handler = READY;
            expectedSerial = 1;
            Serial.write(ACK);
            break;
        case READY:
            int handleCode = Serial.read();
            handler = handleUART(handleCode);
            Serial.write(ACK);
            break;
        default: ;

    }
    return;
}


void handleScreen( uint16_t screenCode) {
    switch(screenCode) {
        case MAIN_SCREEN_CODE:
            currentScreen = screenCode;
            drawMainScreen();
            break;
        case CONTACTS_SCREEN_CODE:
            currentScreen = screenCode;
            drawContactsScreen();
            break;
        case COMPOSE_SCREEN_CODE:
            currentScreen = screenCode;
            drawComposeScreen();
            break;
        case RECEIVED_SCREEN_CODE:
            currentScreen = screenCode;
            drawReceivedScreen();
            break;
        default: ;
    }
    return;
}

void addContact() {//use textBuffer as ID and textBuffer2 as name
    Serial.write(ADD_CONT);
    waitForAck();
    uint32_t ID;
    for(uint8_t i = 0; i < 8; i++) {//8 because 8 decimal digits?
        ID += ((int) textBuffer[i])*10*(7-i); //i think this will work?
    }
    for (uint8_t i = 3; i >= 0;i++) {
        Serial.write((ID >> (8*i)) & 0xFF); //write each byte of ID to xiao
    }
    for (uint8_t i = 0; i < 12;i++) {
        Serial.write(textBuffer2[i]); //write first 12 bytes of textBuffer2 to xiao
    }
}

void deleteContact() {//use textBuffer as ID
    //TO-DO: make it name based with lookup ID; also on-click of contact
    Serial.write(DEL_CONT);
    waitForAck();
    uint32_t ID = 0;
    for(uint8_t i = 0; i < 8; i++) {//8 because 8 decimal digits?
        ID += ((int) textBuffer[i])*10*(7-i); //i think this will work?
    }
    for (uint8_t i = 3; i >= 0;i++) {
        Serial.write((ID >> (8*i)) & 0xFF); //write each byte of ID to xiao
    }
}

void sendMessage() {//use textBuffer as ID and textBuffer2 as message
    Serial.write(MSG_SEND);
    waitForAck();
    uint32_t ID = 0;
    for(uint8_t i = 0; i < 8; i++) {//8 because 8 decimal digits?
        ID += ((int) textBuffer[i])*10*(7-i); //i think this will work?
    }
    char tmpMessage[144] = {0};
    for (uint8_t i = 0; i >= 0;i++) {
        tmpMessage[i] = ((ID >> (8*i)) & 0xFF); //write each byte of ID to first 4 bytes of tmpMSG
    }
    for (uint8_t i = 0; i < 240; i++) {
        tmpMessage[i+4] = textBuffer2[i]; //write message to tmp message
    }
    for (uint8_t i = 0; i < 4; i++) { //send 4 chunks of 64 bytes
        for (uint16_t j = i*64; j < (i+1)*64; j++) {
            if (j < 144) {
                Serial.write(tmpMessage[j]);
            }
            else { //if we are over the 244 byte message storage limite
                Serial.write(0);
            }
        }
        waitForAck();
    }
}

/*
void addToTextBuffer(char press, uint8_t buffer) { //on key press add characer to buffer
    switch(buffer) {
        case 1:
            if (textPlaceholder < 11) //if we are at end of buffer do nothing
                textBuffer[textPlaceholder++] = press;
            break;
        case 2:
            if (textPlaceholder2 < 239) //if we are at end of buffer do nothing
                textBuffer2[textPlaceholder2++] = press;
            break;
        default: ;
    }
}

void deleteTextBuffer(uint8_t buffer) { //delete last character in textbuffer;
    switch(buffer) {
        case 1:
            textBuffer[textPlaceholder] = press;
            if (textPlaceholder > 0) //if we are at beginnning of buffer do nothing
                textBuffer[--textPlaceholder] = 0;
            else if (textPlaceHolder == 0) 
                textBuffer[0] = 0;
            break;
        case 2:
            if (textPlaceholder2 > 0) //if we are at beginnning of buffer do nothing
                textBuffer2[--textPlaceholder2] = 0;
            else if (textPlaceHolder2 == 0) 
                textBuffer2[0] = 0;
            break;
        default: ;
    }
    return;
}*/
