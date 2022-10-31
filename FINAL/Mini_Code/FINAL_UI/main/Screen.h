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

void evaluatePipe() {
    switch(handler) {
        case RECEIVE_N1:
            for (uint8_t i = 0; i < 64; i++) {
                messageToReceive[i] = serialPipe[63-i];
            }
            serialCounter = 0;
            handler = RECEIVE_N2;
            Serial.write(ACK); 
            break;
        case RECEIVE_N2:
            for (uint8_t i = 0; i < 50; i++) {//112+2+14=128, 112+2-64=50
                messageToReceive[i+64] = serialPipe[63-i];
            }
            messageReceived(messageToReceive);
            serialCounter = 0;
            handler = READY;
            expectedSerial = 1;
            Serial.write(ACK);
            break;
        case CONTS_REC_N1:
            for (uint8_t i = 0; i < 64; i++) {
                currentContacts[0][i] = serialPipe[63-i];
            }
            serialCounter = 0;
            handler = CONTS_REC_N2;
            Serial.write(ACK);
            break;
        case CONTS_REC_N2:
            for (uint8_t i = 0; i < 64; i++) {
                currentContacts[0][i+64] = serialPipe[63-i];
            }
            serialCounter = 0;
            handler = CONTS_REC_N3;
            Serial.write(ACK);
            break;
        case CONTS_REC_N3:
            for (uint8_t i = 0; i < 64; i++) {
                currentContacts[1][i] = serialPipe[63-i];
            }
            serialCounter = 0;
            handler = CONTS_REC_N4;
            Serial.write(ACK);
            break;
        case CONTS_REC_N4:
            for (uint8_t i = 0; i < 64; i++) {
                currentContacts[1][i+64] = serialPipe[63-i];
            }
            serialCounter = 0;
            handler = READY;
            expectedSerial = 1;
            displayContacts();
            break;
        case READY:
            uint8_t handleCode = serialPipe[0];
            serialCounter = 0;
            handler = handleUART(handleCode);
            Serial.write(ACK);
            break;
        default: ;

    }
    return;
}

void processByte() {
    //shift right arrray for loop
    //serial.read into [0]
    //increase serial coutner
    
    for(int i = 63; i > 0; i--)
    {
        serialPipe[i] = serialPipe[i-1]; // right shifts data
    }
    serialPipe[0] = Serial.read();//reads first element
    serialCounter++;
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
    uint16_t ID = 0;
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
