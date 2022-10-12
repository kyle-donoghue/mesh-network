#include "ScreenDefines.h"
#include "Adafruit_GFX.h"

void drawMainScreen() { // 2
    tft.fillScreen(ILI9341_BLACK);
    tft.setCursor(60, 40);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.println("ID:120382082948");

    // CONTACTS BUTTON
    tft.fillRect(CONTACTS_BUTTON_X, CONTACTS_BUTTON_Y, CONTACTS_BUTTON_X_WIDTH, CONTACTS_BUTTON_Y_WIDTH,ILI9341_RED);
    tft.setCursor(CONTACTS_BUTTON_X + 10,CONTACTS_BUTTON_Y + 35);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.println("Contacts");
   
    // LOGS BUTTON
    tft.fillRect(LOG_BUTTON_X, LOG_BUTTON_Y, LOG_BUTTON_X_WIDTH, LOG_BUTTON_Y_WIDTH,ILI9341_BLUE);
    tft.setCursor(LOG_BUTTON_X + 20,LOG_BUTTON_Y+35);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.println("Logs");
    //example code above
    //main screen displaying deviceID and buttons (just use radnom hardcoded value for deviceID)
}

void drawContactsScreen() { // 14
    //for individual contacts, just use hardcoded names (no xiao requests)
    tft.fillScreen(ILI9341_BLACK);

    // MAIN/HOME BUTTON
    tft.fillTriangle(MAIN_BUTTON_X,MAIN_BUTTON_Y + 20,MAIN_BUTTON_X + 20,MAIN_BUTTON_Y, MAIN_BUTTON_X + 40,MAIN_BUTTON_Y+20,ILI9341_MAGENTA );
    tft.fillRect(MAIN_BUTTON_X + 5, MAIN_BUTTON_Y + 20, MAIN_BUTTON_X_WIDTH, MAIN_BUTTON_Y_WIDTH,ILI9341_MAGENTA);
    tft.setCursor(MAIN_BUTTON_X + 9,MAIN_BUTTON_Y + 30);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(1);
    tft.println("HOME");

    // ADD CONTACTS BUTTON
    tft.fillCircle(ADD_CONTACT_BUTTON_X,ADD_CONTACT_BUTTON_Y, ADD_CONTACT_BUTTON_X_WIDTH,ILI9341_BLUE);
    tft.setCursor(ADD_CONTACT_BUTTON_X - 8,ADD_CONTACT_BUTTON_Y-2);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(1);
    tft.println("ADD");

    // DELETE CONTACTS BUTTON
    tft.fillRect(DELETE_CONTACT_BUTTON_X,DELETE_CONTACT_BUTTON_Y,DELETE_CONTACT_BUTTON_X_WIDTH,DELETE_CONTACT_BUTTON_Y_WIDTH,ILI9341_RED );
    tft.setCursor(DELETE_CONTACT_BUTTON_X + 10,DELETE_CONTACT_BUTTON_Y + 5);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(1);
    tft.println("DEL");

    // BUTTON-UP
    tft.fillTriangle(CONTACT1_BUTTON_X_WIDTH + 10, CONTACT1_BUTTON_Y - 30, CONTACT1_BUTTON_X_WIDTH, CONTACT1_BUTTON_Y - 5, CONTACT1_BUTTON_X_WIDTH + 20, CONTACT1_BUTTON_Y - 5, ILI9341_WHITE);

    // BUTTON DOWN 
     tft.fillTriangle(CONTACT1_BUTTON_X_WIDTH + 10, CONTACTS_PG_DOWN_BUTTON_Y, CONTACT1_BUTTON_X_WIDTH, CONTACTS_PG_DOWN_BUTTON_Y_WIDTH, CONTACT1_BUTTON_X_WIDTH + 20, CONTACTS_PG_DOWN_BUTTON_Y_WIDTH, ILI9341_WHITE);


    tft.setCursor(0, 80);
    tft.setTextColor(ILI9341_RED);
    tft.setTextSize(2);
    tft.println("James");

    tft.setCursor(5, 100);
    tft.setTextColor(ILI9341_GREEN);
    tft.setTextSize(1);
    tft.drawRect(CONTACT1_BUTTON_X,CONTACT1_BUTTON_Y, CONTACT1_BUTTON_X_WIDTH, CONTACT1_BUTTON_Y_WIDTH, ILI9341_WHITE);
    tft.println("Character Counter is a 100% free online Character\n Counter is a 100% free online character\n count calculator that's simple to use.");

    tft.setCursor(0, 150);
    tft.setTextColor(ILI9341_RED);
    tft.setTextSize(2);
    tft.println("Chase");

    tft.setCursor(5, 170);
    tft.setTextColor(ILI9341_GREEN);
    tft.setTextSize(1);
    tft.drawRect(CONTACT2_BUTTON_X,CONTACT2_BUTTON_Y, CONTACT1_BUTTON_X_WIDTH, CONTACT2_BUTTON_Y_WIDTH, ILI9341_WHITE);
    tft.println("Character Counter is a 100% free online Character\n Counter is a 100% free online character\n count calculator that's simple to use.");
    }// end draw Contacts

void drawLogScreen() { // 1

    tft.fillScreen(ILI9341_BLACK);
    tft.setCursor(60, 40);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.println("welcome to LOGS");

    // MAIN/HOME BUTTON
    tft.fillTriangle(MAIN_BUTTON_X,MAIN_BUTTON_Y + 20,MAIN_BUTTON_X + 20,MAIN_BUTTON_Y, MAIN_BUTTON_X + 40,MAIN_BUTTON_Y+20,ILI9341_MAGENTA );
    tft.fillRect(MAIN_BUTTON_X + 5, MAIN_BUTTON_Y + 20, MAIN_BUTTON_X_WIDTH, MAIN_BUTTON_Y_WIDTH,ILI9341_MAGENTA);
    tft.setCursor(MAIN_BUTTON_X + 9,MAIN_BUTTON_Y + 30);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(1);
    tft.println("HOME");

    // BUTTON-UP
    tft.fillTriangle(CONTACT1_BUTTON_X_WIDTH + 10, CONTACT1_BUTTON_Y - 30, CONTACT1_BUTTON_X_WIDTH, CONTACT1_BUTTON_Y - 5, CONTACT1_BUTTON_X_WIDTH + 20, CONTACT1_BUTTON_Y - 5, ILI9341_WHITE);

    // BUTTON DOWN 
     tft.fillTriangle(CONTACT1_BUTTON_X_WIDTH + 10, CONTACTS_PG_DOWN_BUTTON_Y, CONTACT1_BUTTON_X_WIDTH, CONTACTS_PG_DOWN_BUTTON_Y_WIDTH, CONTACT1_BUTTON_X_WIDTH + 20, CONTACTS_PG_DOWN_BUTTON_Y_WIDTH, ILI9341_WHITE);

    //if you want to try out new layout with contacts and logs combined, then skip this
}

void drawComposeScreen() { //
    //let me know when you get here and we can discuss the keypad, but should also have a placeholder text box and send button
    
    tft.fillScreen(ILI9341_BLACK);

    //MSG WINDOW
    tft.fillRect(0,0,320,100,ILI9341_WHITE);
    tft.setCursor(10,10);
    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(2);
    tft.println("To: James \n     Msg's Here");

    //MAIN/HOME BUTTON
    tft.fillTriangle(MAIN_BUTTON_X + 300,MAIN_BUTTON_Y,MAIN_BUTTON_X + 280,MAIN_BUTTON_Y + 20, MAIN_BUTTON_X + 320,MAIN_BUTTON_Y+20,ILI9341_MAGENTA );
    tft.fillRect(MAIN_BUTTON_X + 285, MAIN_BUTTON_Y + 20, MAIN_BUTTON_X_WIDTH, MAIN_BUTTON_Y_WIDTH,ILI9341_MAGENTA);
    tft.setCursor(MAIN_BUTTON_X + 290,MAIN_BUTTON_Y + 30);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(1);
    tft.println("HOME");

    //SEND BUTTON 
    tft.fillRect(SEND_BUTTON_X,SEND_BUTTON_Y, SEND_BUTTON_X_WIDTH,SEND_BUTTON_Y_WIDTH,ILI9341_CYAN);
    tft.setCursor(SEND_BUTTON_X,SEND_BUTTON_Y);
    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(2);
    tft.println("Send");


    // KEYBOARD SECTION
    tft.fillRect(0, 110, 320, 240,ILI9341_YELLOW );
    tft.fillRect(10, 120, 300, 190,ILI9341_BLACK );
    tft.setCursor(100,150);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(3);
    tft.println("KEYBOARD");

}

void drawReceivedScreen() { // 
    //new message screen with reply button
    tft.fillScreen(ILI9341_BLACK);
    tft.setCursor(60, 40);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.println("Msg's Received");

     // MAIN/HOME BUTTON
    tft.fillTriangle(MAIN_BUTTON_X,MAIN_BUTTON_Y + 20,MAIN_BUTTON_X + 20,MAIN_BUTTON_Y, MAIN_BUTTON_X + 40,MAIN_BUTTON_Y+20,ILI9341_MAGENTA );
    tft.fillRect(MAIN_BUTTON_X + 5, MAIN_BUTTON_Y + 20, MAIN_BUTTON_X_WIDTH, MAIN_BUTTON_Y_WIDTH,ILI9341_MAGENTA);
    tft.setCursor(MAIN_BUTTON_X + 9,MAIN_BUTTON_Y + 30);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(1);
    tft.println("HOME");

    // BUTTON-UP
    tft.fillTriangle(CONTACT1_BUTTON_X_WIDTH + 10, CONTACT1_BUTTON_Y - 30, CONTACT1_BUTTON_X_WIDTH, CONTACT1_BUTTON_Y - 5, CONTACT1_BUTTON_X_WIDTH + 20, CONTACT1_BUTTON_Y - 5, ILI9341_WHITE);

    // BUTTON DOWN 
     tft.fillTriangle(CONTACT1_BUTTON_X_WIDTH + 10, CONTACTS_PG_DOWN_BUTTON_Y, CONTACT1_BUTTON_X_WIDTH, CONTACTS_PG_DOWN_BUTTON_Y_WIDTH, CONTACT1_BUTTON_X_WIDTH + 20, CONTACTS_PG_DOWN_BUTTON_Y_WIDTH, ILI9341_WHITE);
}
