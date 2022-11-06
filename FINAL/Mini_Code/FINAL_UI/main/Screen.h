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

void deleteChar() { 
  if (textBufferLength == 0) //dont delete more than 0
    return;
  if (cursorCords[0]-CURSOR_XWIDTH < 15) {
    cursorCords[0] = CURSOR_X+30*CURSOR_XWIDTH;
    cursorCords[1] = cursorCords[1]-CURSOR_YWIDTH;
  }
  else //regular delete
    cursorCords[0] = cursorCords[0]-CURSOR_XWIDTH;
  tft.fillRect(cursorCords[0]*2,cursorCords[1]*2,CURSOR_XWIDTH*2,CURSOR_YWIDTH*2-2, WHITE);
  textBuffer[textBufferLength] = '\0';
  textBufferLength--;
}


void typeNum(int x, int y) {
  if (textBufferLength <= 5) {
    uint16_t x_hat = x;
    uint8_t ix = x_hat / KEY_XWIDTH;
    tft.setCursor(cursorCords[0]*2,cursorCords[1]*2);
    tft.setTextSize(1);
    tft.println(rownum[ix]);
    textBuffer[textBufferLength] = rownum[ix];
    if (cursorCords[0]+CURSOR_XWIDTH > 136) {
      cursorCords[0] = CURSOR_X;
      cursorCords[1] = cursorCords[1]+CURSOR_YWIDTH;
    }
    else
      cursorCords[0] = cursorCords[0]+CURSOR_XWIDTH;
    textBufferLength++;
  }
}

void typeChar(int x, int y) {
  if (textBufferLength <= TEXT_BUFFER_MAX) {
    uint16_t x_hat = x;
    uint16_t y_hat = y - KEYBOARD_BUTTON_Y;
    uint8_t ix = x_hat / KEY_XWIDTH;
    uint8_t iy = y_hat / KEY_YWIDTH;
    tft.setCursor(cursorCords[0]*2,cursorCords[1]*2);
    tft.setTextSize(1);
    tft.println(row[iy][ix] == '_' ? ' ' : row[iy][ix]);
    textBuffer[textBufferLength] = row[iy][ix] == '_' ? ' ' : row[iy][ix];
    if (cursorCords[0]+CURSOR_XWIDTH > 136) {
      cursorCords[0] = CURSOR_X;
      cursorCords[1] = cursorCords[1]+CURSOR_YWIDTH;
    }
    else
      cursorCords[0] = cursorCords[0]+CURSOR_XWIDTH;
    textBufferLength++;
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
                if (composeScreenButtons[i][0] == KEYBOARD_SCREEN_CODE) {
                  typeChar(x, y);
                }
                return composeScreenButtons[i][0];
            }
            return 0;
        case ENTERNAME_SCREEN_CODE:
            for(int i = 0; i < COMPOSE_SCREEN_BUTTON_COUNT; i++){

                if( !((x >= enterNameScreenButtons[i][1]) && (x <= enterNameScreenButtons[i][1]+enterNameScreenButtons[i][3])) ){ // if x , then go to contacts/logs
                    continue;
                }
                if( !((y >= enterNameScreenButtons[i][2]) && (y <= enterNameScreenButtons[i][2]+enterNameScreenButtons[i][4])) ){ // if y matches, then go to contacts/logs
                    continue;
                }
                if (enterNameScreenButtons[i][0] == KEYBOARD_SCREEN_CODE) {
                  tft.fillRect(200,50,80,40,RED);
    tft.fillTriangle(280,30,280,110,320,70,RED);
                  typeChar(x, y);
                }
                return enterNameScreenButtons[i][0];
            }
            return 0;
        case ENTERID_SCREEN_CODE:
            for(int i = 0; i < COMPOSE_SCREEN_BUTTON_COUNT; i++){

                if( !((x >= enterIDScreenButtons[i][1]) && (x <= enterIDScreenButtons[i][1]+enterIDScreenButtons[i][3])) ){ // if x , then go to contacts/logs
                    continue;
                }
                if( !((y >= enterIDScreenButtons[i][2]) && (y <= enterIDScreenButtons[i][2]+enterIDScreenButtons[i][4])) ){ // if y matches, then go to contacts/logs
                    continue;
                }
                if (enterIDScreenButtons[i][0] == KEYBOARDNUM_SCREEN_CODE) {
                  tft.fillRect(200,50,80,40,RED);
    tft.fillTriangle(280,30,280,110,320,70,RED);
                  typeChar(x, y);
                }
                return enterIDScreenButtons[i][0];
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
uint8_t handleUART(uint8_t handleCode) {
    uint8_t tmp_handler = READY;
    switch(handleCode) {
        case MESSAGE_REC: //xiao sends message received
            tmp_handler = RECEIVE_N1;
            expectedSerial = 64;
            Serial.write(ACK);
            Serial.flush();
            break;
        default: ;
    }
    return tmp_handler;
}

void sendMessage() {//use textBuffer as ID and textBuffer2 as message
    Serial.write(MSG_SEND);
    waitForAck();
    uint16_t R_ID = ((uint8_t)currentContacts[currContactIndex][0]<<8)|((uint8_t)currentContacts[currContactIndex][1]);
    char totalMessage[128] = {0};
    for (uint8_t i = 0; i < 2;i++) {
      totalMessage[i] = currentContacts[currContactIndex][i];
    }
    for (uint8_t i = 2; i < 128; i++) {
      totalMessage[i] = textBuffer[i-2];
    }
    
    for (uint8_t i = 0; i < 2; i++) { //send 2 chunks of 64 bytes
        for (uint8_t j = i*64; j < (i+1)*64; j++) {
                Serial.write(totalMessage[j]);
        }
        waitForAck();
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
    serialPipe[0] = Serial.read();//reads first element
    if (currentScreen == COMPOSE_SCREEN_CODE) {
      tft.setCursor(30, 50);
    tft.setTextColor(BLACK);
    tft.setTextSize(2);
    tft.println(serialPipe[0]);
    }
    serialCounter++;
    return;
}

void deleteContact(uint8_t ind) {
  Serial.write(DEL_CONT);
  waitForAck();
  Serial.write(currentContacts[ind][0]);
  Serial.write(currentContacts[ind][1]);
  deleting = false;
  drawContactsScreen();
  return;
}

void getAddID() {
  uint16_t multiplier = 1;
  for (uint8_t i = 114; i > 0; i--) {
    if (textBuffer[i-1] != '\0') {
      addID += (((uint8_t)textBuffer[i])-48)*multiplier;
      multiplier *= 10;
    }
  }
}

void addContact() {
  Serial.write(ADD_CONT);
  waitForAck();
  for(uint8_t i = 0; i < 2; i++) {
    Serial.write((addID>>((1-i)*8)));
  }
  for(uint8_t i = 0; i <11; i++) {
    Serial.write(textBuffer[i]);
  }
  Serial.write('\0');
  
}

void contactsPageChange(bool down) {
  if (down) {
    if (reqContactsInd+2 > NUM_CONTS-1) {
      reqContactsInd = 0;
    }
    else
      reqContactsInd += 2;
  }
  else {
    if (reqContactsInd-2 < 0) {
      reqContactsInd = NUM_CONTS-2;
    }
    else
      reqContactsInd -= 2;
  }
  requestContacts(reqContactsInd);
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
            drawComposeScreen(0);
            break;
        case RECEIVED_SCREEN_CODE:
            currentScreen = screenCode;
            drawReceivedScreen();
            break;
        case DELETE_CONTACT_SCREEN_CODE:
            deleting = true;
            break;
        case CONTACT1_SCREEN_CODE:
            if(deleting)
              deleteContact(0);
            else {
              currentScreen = COMPOSE_SCREEN_CODE;
              drawComposeScreen(0);
            }
            break;
        case CONTACT2_SCREEN_CODE:
            if(deleting)
              deleteContact(1);
            else {
              currentScreen = COMPOSE_SCREEN_CODE;
              drawComposeScreen(1);
            }
            break;
        case DELETE_SCREEN_CODE:
            deleteChar();
            break;
        case SEND_SCREEN_CODE:
            sendMessage();
            handleScreen(CONTACTS_SCREEN_CODE);
            break;
        case CONTACTS_PG_DOWN_SCREEN_CODE:
            contactsPageChange(0); //page up
            break;
        case CONTACTS_PG_UP_SCREEN_CODE:
            contactsPageChange(1); //page down
            break;
        case SHUTDOWN_SCREEN_CODE:
            drawShutdown();
            currentScreen = screenCode;
            break;
        case ENTERIDSEND_SCREEN_CODE:
            currentScreen = ENTERNAME_SCREEN_CODE;
            getAddID();
            drawEnterName();
            break;
        case ENTERNAMESEND_SCREEN_CODE:
            addContact();
            handleScreen(CONTACTS_SCREEN_CODE);
            break;

        case ADD_CONTACT_SCREEN_CODE:
            currentScreen = ENTERID_SCREEN_CODE;
            drawEnterID();
            break;
        default: ;
    }
    return;
}

void evaluatePipe() {
    switch(handler) {
        case RECEIVE_N1: {
            for (uint8_t i = 0; i < 64; i++) {
                currentMessage[i] = serialPipe[63-i];
            }
            serialCounter = 0;
            handler = RECEIVE_N2;
            Serial.write(ACK); 
            break;
        }
        case RECEIVE_N2: {
            for (uint8_t i = 0; i < 50; i++) {//112+2+14=128, 112+2-64=50
                currentMessage[i+64] = serialPipe[63-i];
            }
            //messageReceived(messageToReceive);
            serialCounter = 0;
            handler = READY;
            expectedSerial = 1;
            Serial.write(ACK);
            handleScreen(RECEIVED_SCREEN_CODE);
            break;
        }
        case CONTS_REC_N1: {
            for (uint8_t i = 0; i < 64; i++) {
                currentContacts[0][i] = serialPipe[63-i];
            }
            serialCounter = 0;
            handler = CONTS_REC_N2;
            Serial.write(ACK);
            break;
        }
        case CONTS_REC_N2: {
            for (uint8_t i = 0; i < 64; i++) {
                currentContacts[0][i+64] = serialPipe[63-i];
            }
            serialCounter = 0;
            handler = CONTS_REC_N3;
            Serial.write(ACK);
            break;
        }
        case CONTS_REC_N3: {
            for (uint8_t i = 0; i < 64; i++) {
                currentContacts[1][i] = serialPipe[63-i];
            }
            serialCounter = 0;
            handler = CONTS_REC_N4;
            Serial.write(ACK);
            break;
        }
        case CONTS_REC_N4: {
            for (uint8_t i = 0; i < 64; i++) {
                currentContacts[1][i+64] = serialPipe[63-i];
            }
            serialCounter = 0;
            handler = READY;
            expectedSerial = 1;
            displayContacts();
            Serial.write(ACK);
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
    return;
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
