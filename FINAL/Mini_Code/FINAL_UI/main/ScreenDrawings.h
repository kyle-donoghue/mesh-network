void messageReceived(char message[144]) {
    //wake up screen if off
    if (!awake) {
        awake = 1;
        screenPower(1);
    }

    //change state to received message screen
    //display new message with buttons described in array
    uint16_t senderID = 0;
    for (uint8_t i = 0; i < 2; i++){
        senderID += Serial.read();
        if (i < 3)
            senderID = senderID<<8;
    }
    char messageText[142] = {0};
    for (uint8_t i = 2; i < 144; i++){
        messageText[i] = message[i-2];
    }
}
void displayMessage( char message[144], uint8_t index) {
  return;
}
void displayContacts() {
    
    char str[12] = {0};
    for (uint8_t i = 0; i < 12; i++) {
      str[i] = currentContacts[0][i+3];
    }
    tft.setCursor(CONTACTS_BUTTON_X + 10,CONTACTS_BUTTON_Y + 35);
    tft.setTextColor(WHITE);
    tft.setTextSize(2);
    tft.println(str);
  
}

//all other screen drawings i.e. drawContactsScreen()

//use requestMessage and requestContacts when drawing to get data

void requestMessage(uint8_t numMessage, uint8_t num) { //get the nth message
    Serial.write(MSG_DISP);
    expectedSerial = 64;
    handler = RECEIVE_N1;
    switch(num) {
        case 1:
            messageHandler = MSG_LOG1;
            break;
        case 2:
            messageHandler = MSG_LOG2;
            break;
        case 3:
            messageHandler = MSG_LOG3;
            break;
        case 4:
            messageHandler = MSG_LOG4;
            break;
        default: ;
    }
    return;
}

void requestContacts(uint8_t firstContact) { //get 4 contacts at once
    Serial.write(REQ_CONTS);
    waitForAck();
    Serial.write(firstContact);
    expectedSerial = 64;
    handler = CONTS_REC_N1;
    return;
}

void drawMainScreen() { // 2
    tft.fillScreen(BLACK);
    tft.setCursor(60, 40);
    tft.setTextColor(WHITE);
    tft.setTextSize(2);
    tft.print("ID: ");
    uint16_t deviceID = DEVICE_ID;
    tft.println(deviceID);
    
    // CONTACTS BUTTON
    tft.fillRect(CONTACTS_BUTTON_X, CONTACTS_BUTTON_Y, CONTACTS_BUTTON_X_WIDTH, CONTACTS_BUTTON_Y_WIDTH,RED);
    tft.setCursor(CONTACTS_BUTTON_X + 10,CONTACTS_BUTTON_Y + 35);
    tft.setTextColor(WHITE);
    tft.setTextSize(2);
    tft.println("Contacts");
   
}

void drawContactsScreen() { // 14
   tft.fillScreen(BLACK);
   /* 
    // MAIN/HOME BUTTON
   tft.fillTriangle(MAIN_BUTTON_X,MAIN_BUTTON_Y + 20,MAIN_BUTTON_X + 20,MAIN_BUTTON_Y, MAIN_BUTTON_X + 40,MAIN_BUTTON_Y+20,MAGENTA );
   tft.fillRect(MAIN_BUTTON_X + 5, MAIN_BUTTON_Y + 20, MAIN_BUTTON_X_WIDTH, MAIN_BUTTON_Y_WIDTH,MAGENTA);
   tft.setCursor(MAIN_BUTTON_X + 9,MAIN_BUTTON_Y + 30);
   tft.setTextColor(WHITE);
   tft.setTextSize(1);
   tft.println("HOME");

    // Device ID
   tft.setCursor(60, 15);
   tft.setTextColor(WHITE);
   tft.setTextSize(2);
   tft.print("ID: ");
   uint16_t deviceID = DEVICE_ID;
   tft.println(deviceID);

   // ADD CONTACTS BUTTON
   tft.fillCircle(ADD_CONTACT_BUTTON_X,ADD_CONTACT_BUTTON_Y, ADD_CONTACT_BUTTON_X_WIDTH,BLUE);
   tft.setCursor(ADD_CONTACT_BUTTON_X - 8,ADD_CONTACT_BUTTON_Y-2);
   tft.setTextColor(WHITE);
   tft.setTextSize(1);
   tft.println("ADD");

   // DELETE CONTACTS BUTTON
   tft.fillRect(DELETE_CONTACT_BUTTON_X,DELETE_CONTACT_BUTTON_Y,DELETE_CONTACT_BUTTON_X_WIDTH,DELETE_CONTACT_BUTTON_Y_WIDTH,RED );
   tft.setCursor(DELETE_CONTACT_BUTTON_X + 10,DELETE_CONTACT_BUTTON_Y + 5);
   tft.setTextColor(WHITE);
   tft.setTextSize(1);
   tft.println("DEL");

   // BUTTON-UP
   tft.fillTriangle(CONTACT1_BUTTON_X_WIDTH + 10, CONTACT1_BUTTON_Y - 30, CONTACT1_BUTTON_X_WIDTH, CONTACT1_BUTTON_Y - 5, CONTACT1_BUTTON_X_WIDTH + 20, CONTACT1_BUTTON_Y - 5, WHITE);

   // BUTTON DOWN 
    tft.fillTriangle(CONTACT1_BUTTON_X_WIDTH + 10, CONTACTS_PG_DOWN_BUTTON_Y, CONTACT1_BUTTON_X_WIDTH, CONTACTS_PG_DOWN_BUTTON_Y_WIDTH, CONTACT1_BUTTON_X_WIDTH + 20, CONTACTS_PG_DOWN_BUTTON_Y_WIDTH, WHITE);
   */
    requestContacts(0);
    return;
}// end draw Contacts


void drawComposeScreen() { //
   
    tft.fillScreen(BLACK);

    //MSG WINDOW
    tft.fillRect(0,0,320,100,WHITE);
    tft.setCursor(10,10);
    tft.setTextColor(BLACK);
    tft.setTextSize(2);
    tft.println("To: ");

    //MAIN/HOME BUTTON
    tft.fillTriangle(MAIN_BUTTON_X + 300,MAIN_BUTTON_Y,MAIN_BUTTON_X + 280,MAIN_BUTTON_Y + 20, MAIN_BUTTON_X + 320,MAIN_BUTTON_Y+20,MAGENTA );
    tft.fillRect(MAIN_BUTTON_X + 285, MAIN_BUTTON_Y + 20, MAIN_BUTTON_X_WIDTH, MAIN_BUTTON_Y_WIDTH,MAGENTA);
    tft.setCursor(MAIN_BUTTON_X + 290,MAIN_BUTTON_Y + 30);
    tft.setTextColor(WHITE);
    tft.setTextSize(1);
    tft.println("HOME");

    //SEND BUTTON 
    tft.fillRect(SEND_BUTTON_X,SEND_BUTTON_Y, SEND_BUTTON_X_WIDTH,SEND_BUTTON_Y_WIDTH,CYAN);
    tft.setCursor(SEND_BUTTON_X,SEND_BUTTON_Y);
    tft.setTextColor(BLACK);
    tft.setTextSize(2);
    tft.println("Send");


    // KEYBOARD SECTION
    tft.fillRect(0, 110, 320, 240,YELLOW );
    tft.fillRect(10, 120, 300, 190,BLACK );
    tft.setCursor(20,130);
    tft.setTextColor(WHITE);
    tft.setTextSize(3);
    tft.println("QWERTYUIOP");
    tft.setCursor(20,160);
    tft.println("ASDFGHJKL;'");
    tft.setCursor(20,190);
    tft.println("ZXCVBNM,.");
}

void drawReceivedScreen() { // 

  // RECEIVED MESSAGES
    tft.fillScreen(BLACK);
    tft.setCursor(60, 15);
    tft.setTextColor(WHITE);
    tft.setTextSize(2);
    tft.println("New Messages");
    tft.drawFastHLine(60, 30, 140, WHITE);

     // BACK BUTTON
    tft.fillTriangle(MAIN_BUTTON_X,MAIN_BUTTON_Y + 20,MAIN_BUTTON_X + 40,MAIN_BUTTON_Y + 10, MAIN_BUTTON_X,MAIN_BUTTON_Y+30,WHITE );
//    tft.fillRect(MAIN_BUTTON_X + 5, MAIN_BUTTON_Y + 20, MAIN_BUTTON_X_WIDTH, MAIN_BUTTON_Y_WIDTH,MAGENTA);
//    tft.setCursor(MAIN_BUTTON_X + 9,MAIN_BUTTON_Y + 30);
//    tft.setTextColor(WHITE);
//    tft.setTextSize(1);
//    tft.println("HOME");

    tft.setCursor(0, 70);
    tft.setTextColor(RED);
    tft.setTextSize(3);
    tft.println("Chase");

    tft.setCursor(5, 110);
    tft.setTextColor(GREEN);
    tft.setTextSize(2);
    tft.drawRect(COMPOSE_BUTTON_X,COMPOSE_BUTTON_Y, COMPOSE_BUTTON_X_WIDTH, COMPOSE_BUTTON_Y_WIDTH, WHITE);
    tft.setCursor(250, 10);
    tft.setTextColor(BLUE);
    tft.setTextSize(2);
    tft.println("Reply");
}
