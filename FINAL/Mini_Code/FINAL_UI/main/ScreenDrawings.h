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
    
    

    tft.setCursor(0, 80);
    tft.setTextColor(RED);
    tft.setTextSize(2);
    tft.println(&currentContacts[0][2]);

    tft.setCursor(10, 100);
    tft.setTextColor(GREEN);
    tft.setTextSize(1);
    tft.drawRect(CONTACT1_BUTTON_X-5,CONTACT1_BUTTON_Y, CONTACT1_BUTTON_X_WIDTH+50, CONTACT1_BUTTON_Y_WIDTH, WHITE);
    tft.println(&currentContacts[0][14]);

    tft.setCursor(0, 150);
    tft.setTextColor(RED);
    tft.setTextSize(2);
    tft.println(&currentContacts[1][2]);

    tft.setCursor(10, 170);
    tft.setTextColor(GREEN);
    tft.setTextSize(1);
    tft.drawRect(CONTACT2_BUTTON_X-5,CONTACT2_BUTTON_Y, CONTACT1_BUTTON_X_WIDTH+50, CONTACT2_BUTTON_Y_WIDTH, WHITE);
    tft.println(&currentContacts[1][14]);

  
}

//all other screen drawings i.e. drawContactsScreen()

//use requestMessage and requestContacts when drawing to get data



void requestContacts(uint8_t firstContact) { //get 4 contacts at once
    Serial.write(REQ_CONTS);
    waitForAck();
    if(booting)
      waitForAck();
    booting=false;
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
    
    /*// MAIN/HOME BUTTON
   tft.fillTriangle(MAIN_BUTTON_X,MAIN_BUTTON_Y + 20,MAIN_BUTTON_X + 20,MAIN_BUTTON_Y, MAIN_BUTTON_X + 40,MAIN_BUTTON_Y+20,MAGENTA );
   tft.fillRect(MAIN_BUTTON_X + 5, MAIN_BUTTON_Y + 20, MAIN_BUTTON_X_WIDTH, MAIN_BUTTON_Y_WIDTH,MAGENTA);
   tft.setCursor(MAIN_BUTTON_X + 9,MAIN_BUTTON_Y + 30);
   tft.setTextColor(WHITE);
   tft.setTextSize(1);
   tft.println("HOME");*/

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
   
    requestContacts(0);
    return;
}// end draw Contacts


void drawComposeScreen(uint8_t ind) { //
  currContactIndex = ind;
   cursorCords[0] = CURSOR_X;
    cursorCords[1] = CURSOR_Y;
    textBufferLength=0;
    tft.fillScreen(BLACK);

    //MSG WINDOW: SCREEN SI (20 --> 90)
    tft.fillRect(0,30,320,60,WHITE);
    tft.setCursor(40,10);
    tft.setTextColor(WHITE);
    tft.setTextSize(2);
    tft.println("To: ");
    tft.setCursor(80,10);
    tft.println(&currentContacts[ind][2]);

    // BACK TO CONTACTS BUTTON //
    tft.fillTriangle(CONTACTS_BUTTON_X,CONTACTS_BUTTON_Y_WIDTH/4,CONTACTS_BUTTON_X_WIDTH/2,CONTACTS_BUTTON_X/2,CONTACTS_BUTTON_X_WIDTH/2,CONTACTS_BUTTON_Y_WIDTH/2,BLUE); // TODO --> MAKE DEFINES

    //SEND BUTTON //
   tft.fillRect(SEND_BUTTON_X,SEND_BUTTON_Y,SEND_BUTTON_X_WIDTH,SEND_BUTTON_Y_WIDTH,RED );
   tft.setCursor(SEND_BUTTON_X + 10,SEND_BUTTON_Y + 5);
   tft.setTextColor(WHITE);
   tft.setTextSize(1);
   tft.println("SEND");
   
   // DELETE BUTTON
   tft.fillRect(DELETE_BUTTON_X,DELETE_BUTTON_Y,DELETE_BUTTON_X_WIDTH,DELETE_BUTTON_Y_WIDTH,RED );
   tft.setCursor(DELETE_BUTTON_X + 10,DELETE_BUTTON_Y + 5);
   tft.setTextColor(WHITE);
   tft.setTextSize(1);
   tft.println("DEL");
    
    

//    //MAIN/HOME BUTTON
//    tft.fillTriangle(MAIN_BUTTON_X + 300,MAIN_BUTTON_Y,MAIN_BUTTON_X + 280,MAIN_BUTTON_Y + 20, MAIN_BUTTON_X + 320,MAIN_BUTTON_Y+20,MAGENTA );
//    tft.fillRect(MAIN_BUTTON_X + 285, MAIN_BUTTON_Y + 20, MAIN_BUTTON_X_WIDTH, MAIN_BUTTON_Y_WIDTH,MAGENTA);
//    tft.setCursor(MAIN_BUTTON_X + 290,MAIN_BUTTON_Y + 30);
//    tft.setTextColor(WHITE);
//    tft.setTextSize(1);
//    tft.println("HOME");
//
//    //SEND BUTTON 
//    tft.fillRect(SEND_BUTTON_X,SEND_BUTTON_Y, SEND_BUTTON_X_WIDTH,SEND_BUTTON_Y_WIDTH,CYAN);
//    tft.setCursor(SEND_BUTTON_X,SEND_BUTTON_Y);
//    tft.setTextColor(BLACK);
//    tft.setTextSize(2);
//    tft.println("Send");


    // KEYBOARD SECTION: KEYBOARD(90 --> 240)
    tft.fillRect(KEYBOARD_BUTTON_X, KEYBOARD_BUTTON_Y, KEYBOARD_BUTTON_X_WIDTH, KEYBOARD_BUTTON_Y_WIDTH,YELLOW );
    //tft.fillRect(10, 120, 300, 190,BLACK );
    tft.drawFastHLine(0,90,320,BLACK);
    tft.drawFastHLine(0,140,320,BLACK);
    tft.drawFastHLine(0,190,320,BLACK);
    
    tft.drawFastVLine(32,90,150,BLACK);
    tft.drawFastVLine(64,90,150,BLACK);
    tft.drawFastVLine(96,90,150,BLACK);
    tft.drawFastVLine(128,90,150,BLACK);
    tft.drawFastVLine(160,90,150,BLACK);
    tft.drawFastVLine(192,90,150,BLACK);
    tft.drawFastVLine(224,90,150,BLACK);
    tft.drawFastVLine(256,90,150,BLACK);
    tft.drawFastVLine(288,90,150,BLACK);

    // DISPLAYS KEYBOARD CHARACTERS
    for(uint8_t i = 0; i < 3; i++){
      for(uint8_t j = 0; j < 10; j++){
        tft.setCursor(j*32+8,90+i*50+12);
        tft.setTextSize(3);
        tft.setTextColor(BLACK);
        tft.println(row[i][j]);
      }      
    }

    /*tft.setCursor(0,30);
    tft.println("G");
    tft.setCursor(15,30);
    tft.println("G");*/

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
