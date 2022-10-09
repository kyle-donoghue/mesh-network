#include "ScreenDefines.h"



TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300); //is the 300 correct?



Elegoo_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);


uint16_t mainScreenButtons[MAIN_SCREEN_BUTTON_COUNT][5] =           [[CONTACTS_SCREEN_CODE,          CONTACTS_BUTTON_X,          CONTACTS_BUTTON_Y,          CONTACTS_BUTTON_X_WIDTH,            CONTACTS_BUTTON_Y_WIDTH],
                                                                    [LOG_SCREEN_CODE,               LOG_BUTTON_X,               LOG_BUTTON_Y,               LOG_BUTTON_X_WIDTH,                 LOG_BUTTON_Y_WIDTH]];
uint16_t contactsScreenButtons[CONTACTS_SCREEN_BUTTON_COUNT][5] =  [[MAIN_SCREEN_CODE,              MAIN_BUTTON_X,              MAIN_BUTTON_Y,              MAIN_BUTTON_X_WIDTH,                MAIN_BUTTON_Y_WIDTH],
                                                                    [ADD_CONTACT_SCREEN_CODE,       ADD_CONTACT_BUTTON_X,       ADD_CONTACT_BUTTON_Y,       ADD_CONTACT_BUTTON_X_WIDTH,         ADD_CONTACT_BUTTON_Y_WIDTH],
                                                                    [DELETE_CONTACT_SCREEN_CODE,    DELETE_CONTACT_BUTTON_X,    DELETE_CONTACT_BUTTON_Y,    DELETE_CONTACT_BUTTON_X_WIDTH,      DELETE_CONTACT_BUTTON_Y_WIDTH],
                                                                    [CONTACTS_PG_DOWN_SCREEN_CODE,  CONTACTS_PG_DOWN_BUTTON_X,  CONTACTS_PG_DOWN_BUTTON_Y,  CONTACTS_PG_DOWN_BUTTON_X_WIDTH,    CONTACTS_PG_DOWN_BUTTON_Y_WIDTH],
                                                                    [CONTACTS_PG_UP_SCREEN_CODE,    CONTACTS_PG_UP_BUTTON_X,    CONTACTS_PG_UP_BUTTON_Y,    CONTACTS_PG_UP_BUTTON_X_WIDTH,      CONTACTS_PG_UP_BUTTON_Y_WIDTH],
                                                                    [CONTACT1_SCREEN_CODE,          CONTACT1_BUTTON_X,          CONTACT1_BUTTON_Y,          CONTACT1_BUTTON_X_WIDTH,            CONTACT1_BUTTON_Y_WIDTH],
                                                                    [CONTACT2_SCREEN_CODE,          CONTACT2_BUTTON_X,          CONTACT2_BUTTON_Y,          CONTACT2_BUTTON_X_WIDTH,            CONTACT2_BUTTON_Y_WIDTH],
                                                                    [CONTACT3_SCREEN_CODE,          CONTACT3_BUTTON_X,          CONTACT3_BUTTON_Y,          CONTACT3_BUTTON_X_WIDTH,            CONTACT3_BUTTON_Y_WIDTH],
                                                                    [CONTACT4_SCREEN_CODE,          CONTACT4_BUTTON_X,          CONTACT4_BUTTON_Y,          CONTACT4_BUTTON_X_WIDTH,            CONTACT4_BUTTON_Y_WIDTH]];
uint16_t logScreenButtons[LOG_SCREEN_BUTTON_COUNT][5] =            [[MAIN_SCREEN_CODE,              MAIN_BUTTON_X,              MAIN_BUTTON_Y,              MAIN_BUTTON_X_WIDTH,                MAIN_BUTTON_Y_WIDTH],
                                                                    [LOG_PG_DOWN_SCREEN_CODE,       LOG_PG_DOWN_BUTTON_X,       LOG_PG_DOWN_BUTTON_Y,       LOG_PG_DOWN_BUTTON_X_WIDTH,         LOG_PG_DOWN_BUTTON_Y_WIDTH],
                                                                    [LOG_PG_UP_SCREEN_CODE,         LOG_PG_UP_BUTTON_X,         LOG_PG_UP_BUTTON_Y,         LOG_PG_UP_BUTTON_X_WIDTH,           LOG_PG_UP_BUTTON_Y_WIDTH]];
uint16_t composeScreenButtons[COMPOSE_SCREEN_BUTTON_COUNT][5] =    [[MAIN_SCREEN_CODE,              MAIN_BUTTON_X,              MAIN_BUTTON_Y,              MAIN_BUTTON_X_WIDTH,                MAIN_BUTTON_Y_WIDTH]
                                                                    [SEND_SCREEN_CODE,              SEND_BUTTON_X,              SEND_BUTTON_Y,              SEND_BUTTON_X_WIDTH,                SEND_BUTTON_Y_WIDTH]];
uint16_t receivedScreen[RECEIVED_SCREEN_BUTTON_COUNT][5] =         [[MAIN_SCREEN_CODE,              MAIN_BUTTON_X,              MAIN_BUTTON_Y,              MAIN_BUTTON_X_WIDTH,                MAIN_BUTTON_Y_WIDTH]
                                                                    [COMPOSE_SCREEN_CODE,           COMPOSE_BUTTON_X,           COMPOSE_BUTTON_Y,           COMPOSE_BUTTON_X_WIDTH,             COMPOSE_BUTTON_Y_WIDTH];

uint16_t keyboardButtons[5] =                                       [];//ARRAY-LIKE WITH RUNTIME DISPERSION INSTEAD OF HARDCODED?

uint8_t handler = READY;
uint8_t messageHandler = MSG_TO_ME;
uint8_t expectedSerial = 1; //1 = waiting for code or ack, 64 for most other transmissions
char messageToReceive[256] = {0};

char textBuffer[12] = {0};
char textBuffer2[240] = {0};
uint8_t textPlaceholder = 0;
uint8_t textPlaceholder2 = 0;

uint32_t currentContactIDs[4] = {0}; //can hold 4 contacts at a time
char currentContactNames[4][12] = {0};
uint16_t currentScreen = MAIN_SCREEN_CODE;

int setupSerial() {
    Serial.begin(BAUD_RATE, SERIAL_8E2); //8 data bits, even parity, 2 stop bits
    //do handshake
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

void setupTimers() {
     /*1. First we reset the control register to amke sure we start with everything disabled.*/
  TCCR1A = 0;                 // Reset entire TCCR1A to 0 
  TCCR1B = 0;                 // Reset entire TCCR1B to 0
 
  /*2. We set the prescalar to the desired value by changing the CS10 CS12 and CS12 bits. */  
  TCCR1B |= B00000100;        //Set CS12 to 1 so we get prescalar 256  
  
  /*3. We enable compare match mode on register A*/
  TIMSK1 |= B00000010;        //Set OCIE1A to 1 so we enable compare match A 
  
  /*4. Set the value of register A to 31250*/
  OCR1A = 31250;             //Finally we set compare register A to this value  
}

int screenPower(bool power) {
    if (power) {
        digitalWrite(A5, HIGH);
        startScreen();
    }
    else
        digitalWrite(A5, LOW);
    return 1;
}

int startScreen() {
        
    delay(100); //turn on screen

    tft.reset(); //reset screen

    uint16_t identifier = tft.readID(); //get screen type, probably should remove below
    if(identifier == 0x9325) {
    Serial.println(F("Found ILI9325 LCD driver"));
    } else if(identifier == 0x9328) {
    Serial.println(F("Found ILI9328 LCD driver"));
    } else if(identifier == 0x4535) {
    Serial.println(F("Found LGDP4535 LCD driver"));
    }else if(identifier == 0x7575) {
    Serial.println(F("Found HX8347G LCD driver"));
    } else if(identifier == 0x9341) {
    Serial.println(F("Found ILI9341 LCD driver"));
    } else if(identifier == 0x8357) {
    Serial.println(F("Found HX8357D LCD driver"));
    } else if(identifier==0x0101)
    {     
        identifier=0x9341;
        Serial.println(F("Found 0x9341 LCD driver"));
    }else {
    Serial.print(F("Unknown LCD driver chip: "));
    Serial.println(identifier, HEX);
    Serial.println(F("If using the Elegoo 2.8\" TFT Arduino shield, the line:"));
    Serial.println(F("  #define USE_Elegoo_SHIELD_PINOUT"));
    Serial.println(F("should appear in the library header (Elegoo_TFT.h)."));
    Serial.println(F("If using the breakout board, it should NOT be #defined!"));
    Serial.println(F("Also if using the breakout, double-check that all wiring"));
    Serial.println(F("matches the tutorial."));
    identifier=0x9341;

    }

    tft.begin(identifier); //initialize tft
    tft.setRotation(2); //make landscape

    tft.fillScreen(BLACK);

    return;
}

