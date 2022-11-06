void displayMessage( char message[144], uint8_t index) {
  return;
}
void displayContacts() {
    
    tft.fillRect(CONTACT1_BUTTON_X-5,CONTACT1_BUTTON_Y, CONTACT1_BUTTON_X_WIDTH+50, CONTACT1_BUTTON_Y_WIDTH, BLACK);
    tft.fillRect(CONTACT2_BUTTON_X-5,CONTACT2_BUTTON_Y, CONTACT1_BUTTON_X_WIDTH+50, CONTACT2_BUTTON_Y_WIDTH, BLACK);
    tft.fillRect(CONTACT1_BUTTON_X-5,CONTACT1_BUTTON_Y-30, CONTACT1_BUTTON_X_WIDTH-50, 100, BLACK);


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
        tft.setCursor(0, 50);
    tft.setTextColor(GREEN);
    tft.setTextSize(2);
    tft.println(handler);

    tft.setCursor(30, 50);
    tft.setTextColor(GREEN);
    tft.setTextSize(2);
    tft.println(expectedSerial);
    tft.setCursor(60, 50);
    tft.setTextColor(GREEN);
    tft.setTextSize(2);
    tft.println(serialCounter);
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
    
    tft.fillRect(SHUTDOWN_BUTTON_X, SHUTDOWN_BUTTON_Y, SHUTDOWN_BUTTON_X_WIDTH, SHUTDOWN_BUTTON_Y_WIDTH,RED);
    tft.setCursor(SHUTDOWN_BUTTON_X+5,SHUTDOWN_BUTTON_Y + 7);
   tft.setTextColor(WHITE);
   tft.setTextSize(1);
   tft.println("SHUTDOWN");  
   // BUTTON-UP
    tft.fillTriangle(CONTACTS_PG_UP_BUTTON_X+CONTACTS_PG_UP_BUTTON_X_WIDTH/4,CONTACTS_PG_UP_BUTTON_Y,CONTACTS_PG_UP_BUTTON_X,CONTACTS_PG_UP_BUTTON_Y_WIDTH/2+CONTACTS_PG_UP_BUTTON_Y,CONTACTS_PG_UP_BUTTON_X_WIDTH/2+CONTACTS_PG_UP_BUTTON_X,CONTACTS_PG_UP_BUTTON_Y_WIDTH/2+CONTACTS_PG_UP_BUTTON_Y,WHITE); // TODO --> MAKE DEFINES

   // BUTTON DOWN 
    tft.fillTriangle(CONTACTS_PG_DOWN_BUTTON_X+CONTACTS_PG_DOWN_BUTTON_X_WIDTH/2,CONTACTS_PG_DOWN_BUTTON_Y,CONTACTS_PG_DOWN_BUTTON_X,CONTACTS_PG_DOWN_BUTTON_Y,CONTACTS_PG_DOWN_BUTTON_X_WIDTH/4+CONTACTS_PG_DOWN_BUTTON_X,CONTACTS_PG_DOWN_BUTTON_Y_WIDTH/2+CONTACTS_PG_DOWN_BUTTON_Y,WHITE); // TODO --> MAKE DEFINES
    reqContactsInd = 0;
    requestContacts(reqContactsInd);
    return;
}// end draw Contacts


void drawComposeScreen(uint8_t ind) { //
  for (uint8_t i = 0; i < 114; i++) {
    textBuffer[i] = '\0';
  }
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
    tft.fillTriangle(CONTACTS_BUTTON_X+CONTACTS_BUTTON_X_WIDTH/2,CONTACTS_BUTTON_Y,CONTACTS_BUTTON_X,CONTACTS_BUTTON_Y_WIDTH/4+CONTACTS_BUTTON_Y,CONTACTS_BUTTON_X_WIDTH/2+CONTACTS_BUTTON_X,CONTACTS_BUTTON_Y_WIDTH/2+CONTACTS_BUTTON_Y,BLUE); // TODO --> MAKE DEFINES

    //SEND BUTTON //
   tft.fillRect(SEND_BUTTON_X,SEND_BUTTON_Y,SEND_BUTTON_X_WIDTH,SEND_BUTTON_Y_WIDTH,GREEN );
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

    if (!awake) {
        awake = 1;
        prevAwake=1;
        screenPower(awake);
    }
  // RECEIVED MESSAGES
    tft.fillScreen(BLACK);
    
    tft.setCursor(60, 15);
    tft.setTextColor(WHITE);
    tft.setTextSize(2);
    tft.println("New Message!");
    //tft.drawFastHLine(60, 30, 140, WHITE);

     // BACK BUTTON
    tft.fillTriangle(CONTACTS_BUTTON_X+CONTACTS_BUTTON_X_WIDTH/2,CONTACTS_BUTTON_Y,CONTACTS_BUTTON_X,CONTACTS_BUTTON_Y_WIDTH/4+CONTACTS_BUTTON_Y,CONTACTS_BUTTON_X_WIDTH/2+CONTACTS_BUTTON_X,CONTACTS_BUTTON_Y_WIDTH/2+CONTACTS_BUTTON_Y,BLUE); // TODO --> MAKE DEFINES


    tft.setCursor(0, 70);
    tft.setTextColor(RED);
    tft.setTextSize(3);
    tft.println(&currentMessage[2]);

    tft.setCursor(0, 100);
    tft.setTextColor(GREEN);
    tft.setTextSize(2);
    tft.println(&currentMessage[14]);

}

void drawShutdown() {
  tft.fillScreen(BLACK);
   tft.setCursor(0, 140);
    tft.setTextColor(WHITE);
    tft.setTextSize(3);
    tft.println("The switch can be turned off.");

    tft.fillRect(200,50,80,40,RED);
    tft.fillTriangle(280,30,280,110,320,70,RED);
    Serial.write(SHUTDOWN);
}

void drawEnterID() {
for (uint8_t i = 0; i < 114; i++) {
    textBuffer[i] = '\0';
  }
   cursorCords[0] = CURSOR_X;
    cursorCords[1] = CURSOR_Y;
    textBufferLength=0;
    tft.fillScreen(BLACK);

    //MSG WINDOW: SCREEN SI (20 --> 90)
    tft.fillRect(0,30,320,60,WHITE);
    tft.setCursor(40,10);
    tft.setTextColor(WHITE);
    tft.setTextSize(2);
    tft.println("Enter ID: ");
    tft.setCursor(80,10);

    // BACK TO CONTACTS BUTTON //
    tft.fillTriangle(CONTACTS_BUTTON_X+CONTACTS_BUTTON_X_WIDTH/2,CONTACTS_BUTTON_Y,CONTACTS_BUTTON_X,CONTACTS_BUTTON_Y_WIDTH/4+CONTACTS_BUTTON_Y,CONTACTS_BUTTON_X_WIDTH/2+CONTACTS_BUTTON_X,CONTACTS_BUTTON_Y_WIDTH/2+CONTACTS_BUTTON_Y,BLUE); // TODO --> MAKE DEFINES

    //SEND BUTTON //
   tft.fillRect(SEND_BUTTON_X,SEND_BUTTON_Y,SEND_BUTTON_X_WIDTH,SEND_BUTTON_Y_WIDTH,GREEN );
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
    
    tft.drawFastVLine(32,90,50,BLACK);
    tft.drawFastVLine(64,90,50,BLACK);
    tft.drawFastVLine(96,90,50,BLACK);
    tft.drawFastVLine(128,90,50,BLACK);
    tft.drawFastVLine(160,90,50,BLACK);
    tft.drawFastVLine(192,90,50,BLACK);
    tft.drawFastVLine(224,90,50,BLACK);
    tft.drawFastVLine(256,90,50,BLACK);
    tft.drawFastVLine(288,90,50,BLACK);

    // DISPLAYS KEYBOARD CHARACTERS
      for(uint8_t j = 0; j < 10; j++){
        tft.setCursor(j*32+8,90+12);
        tft.setTextSize(3);
        tft.setTextColor(BLACK);
        tft.println(rownum[j]);
      }      
   tft.fillRect(ENTERID_BUTTON_X,ENTERID_BUTTON_Y,ENTERID_BUTTON_X_WIDTH,ENTERID_BUTTON_Y_WIDTH,GREEN );
   tft.setCursor(ENTERID_BUTTON_X + 10,ENTERID_BUTTON_Y + 5);
   tft.setTextColor(WHITE);
   tft.setTextSize(1);
   tft.println("NEXT");
}

void drawEnterName() {
  drawComposeScreen(0);
  
   tft.fillRect(ENTERNAME_BUTTON_X,ENTERNAME_BUTTON_Y,ENTERNAME_BUTTON_X_WIDTH,ENTERNAME_BUTTON_Y_WIDTH,GREEN );
   tft.setCursor(ENTERNAME_BUTTON_X + 10,ENTERNAME_BUTTON_Y + 5);
   tft.setTextColor(WHITE);
   tft.setTextSize(1);
   tft.println("ADD");
}
