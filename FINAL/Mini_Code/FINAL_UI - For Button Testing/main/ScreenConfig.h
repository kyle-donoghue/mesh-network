#include "ScreenDefines.h"

TSPoint button;
int screenCode = 0;
bool awake = 1;
bool prevAwake = 1;
bool prevPress = 0;


TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

Elegoo_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

//TO-DO: change to {}
uint16_t mainScreenButtons[MAIN_SCREEN_BUTTON_COUNT][5] =          {{CONTACTS_SCREEN_CODE,          CONTACTS_BUTTON_X,          CONTACTS_BUTTON_Y,          CONTACTS_BUTTON_X_WIDTH,            CONTACTS_BUTTON_Y_WIDTH},
                                                                    {LOG_SCREEN_CODE,               LOG_BUTTON_X,               LOG_BUTTON_Y,               LOG_BUTTON_X_WIDTH,                 LOG_BUTTON_Y_WIDTH}};
uint16_t contactsScreenButtons[CONTACTS_SCREEN_BUTTON_COUNT][5] =  {/*{MAIN_SCREEN_CODE,              MAIN_BUTTON_X,              MAIN_BUTTON_Y,              MAIN_BUTTON_X_WIDTH,                MAIN_BUTTON_Y_WIDTH},*/
                                                                    {ADD_CONTACT_SCREEN_CODE,       ADD_CONTACT_BUTTON_X,       ADD_CONTACT_BUTTON_Y,       ADD_CONTACT_BUTTON_X_WIDTH,         ADD_CONTACT_BUTTON_Y_WIDTH},
                                                                    {DELETE_CONTACT_SCREEN_CODE,    DELETE_CONTACT_BUTTON_X,    DELETE_CONTACT_BUTTON_Y,    DELETE_CONTACT_BUTTON_X_WIDTH,      DELETE_CONTACT_BUTTON_Y_WIDTH},
                                                                    {CONTACTS_PG_DOWN_SCREEN_CODE,  CONTACTS_PG_DOWN_BUTTON_X,  CONTACTS_PG_DOWN_BUTTON_Y,  CONTACTS_PG_DOWN_BUTTON_X_WIDTH,    CONTACTS_PG_DOWN_BUTTON_Y_WIDTH},
                                                                    {CONTACTS_PG_UP_SCREEN_CODE,    CONTACTS_PG_UP_BUTTON_X,    CONTACTS_PG_UP_BUTTON_Y,    CONTACTS_PG_UP_BUTTON_X_WIDTH,      CONTACTS_PG_UP_BUTTON_Y_WIDTH},
                                                                    {CONTACT1_SCREEN_CODE,          CONTACT1_BUTTON_X,          CONTACT1_BUTTON_Y,          CONTACT1_BUTTON_X_WIDTH,            CONTACT1_BUTTON_Y_WIDTH},
                                                                    {CONTACT2_SCREEN_CODE,          CONTACT2_BUTTON_X,          CONTACT2_BUTTON_Y,          CONTACT2_BUTTON_X_WIDTH,            CONTACT2_BUTTON_Y_WIDTH}/*,
                                                                    {CONTACT3_SCREEN_CODE,          CONTACT3_BUTTON_X,          CONTACT3_BUTTON_Y,          CONTACT3_BUTTON_X_WIDTH,            CONTACT3_BUTTON_Y_WIDTH},
                                                                    {CONTACT4_SCREEN_CODE,          CONTACT4_BUTTON_X,          CONTACT4_BUTTON_Y,          CONTACT4_BUTTON_X_WIDTH,            CONTACT4_BUTTON_Y_WIDTH}*/};
uint16_t logScreenButtons[LOG_SCREEN_BUTTON_COUNT][5] =            {{MAIN_SCREEN_CODE,              MAIN_BUTTON_X,              MAIN_BUTTON_Y,              MAIN_BUTTON_X_WIDTH,                MAIN_BUTTON_Y_WIDTH},
                                                                    {LOG_PG_DOWN_SCREEN_CODE,       LOG_PG_DOWN_BUTTON_X,       LOG_PG_DOWN_BUTTON_Y,       LOG_PG_DOWN_BUTTON_X_WIDTH,         LOG_PG_DOWN_BUTTON_Y_WIDTH},
                                                                    {LOG_PG_UP_SCREEN_CODE,         LOG_PG_UP_BUTTON_X,         LOG_PG_UP_BUTTON_Y,         LOG_PG_UP_BUTTON_X_WIDTH,           LOG_PG_UP_BUTTON_Y_WIDTH}};
uint16_t composeScreenButtons[COMPOSE_SCREEN_BUTTON_COUNT][5] =    {{CONTACTS_SCREEN_CODE,              CONTACTS_BUTTON_X,              CONTACTS_BUTTON_Y,              CONTACTS_BUTTON_X_WIDTH,                CONTACTS_BUTTON_Y_WIDTH},
                                                                    {SEND_SCREEN_CODE,              SEND_BUTTON_X,              SEND_BUTTON_Y,              SEND_BUTTON_X_WIDTH,                SEND_BUTTON_Y_WIDTH},
                                                                    {DELETE_SCREEN_CODE,              DELETE_BUTTON_X,              DELETE_BUTTON_Y,              DELETE_BUTTON_X_WIDTH,                DELETE_BUTTON_Y_WIDTH},
                                                                    {KEYBOARD_SCREEN_CODE,              KEYBOARD_BUTTON_X,              KEYBOARD_BUTTON_Y,              KEYBOARD_BUTTON_X_WIDTH,                KEYBOARD_BUTTON_Y_WIDTH}};
uint16_t receivedScreenButtons[RECEIVED_SCREEN_BUTTON_COUNT][5] =         {{CONTACTS_SCREEN_CODE,          CONTACTS_BUTTON_X,          CONTACTS_BUTTON_Y,          CONTACTS_BUTTON_X_WIDTH,            CONTACTS_BUTTON_Y_WIDTH}};

uint16_t keyboardButtons[5] =                                       {}; //runtime dispersion
char row[3][10] = {{'Q','W','E','R','T','Y','U','I','O','P'},
                   {'A','S','D','F','G','H','J','K','L','_'},
                   {'Z','X','C','V','B','N','M',',','.','\''}}; 

uint8_t handler = READY;
uint8_t messageHandler = MSG_TO_ME;
uint8_t expectedSerial = 1; //1 = waiting for code or ack, 64 for most other transmissions
char messageToReceive[144] = {0};

bool pressing = false;
uint8_t holdCount = 0;
uint8_t currContactIndex = 0;
char textBuffer[114] = {'\0'};
uint8_t textBufferLength = 0;

uint8_t cursorCords[2] = {CURSOR_X,CURSOR_Y};

uint16_t currentContactIDs[4] = {0}; //can hold 4 contacts at a time
char currentContactNames[4][14] = {0};
char currentContacts[2][128] = {0};
char currentMessage[128] = {'\0'};
uint8_t currentScreen = MAIN_SCREEN_CODE;

uint8_t serialPipe[64] = {0};
uint8_t serialCounter = 0;

bool deleting = false;
bool booting = true;


void waitForAck() {
  while (true) { //wait for handshake
        if (Serial.available()) {
            uint8_t ack = Serial.read();
            break;
        }
    }
}


int setupSerial() {
    //Serial.begin(BAUD_RATE, SERIAL_8E2); //8 data bits, even parity, 2 stop bits
    Serial.begin(BAUD_RATE);
    //do handshake
    /*Serial.write(BOOT);
    while (true) { //wait for handshake
        if (Serial.available()) {
            uint8_t ack = Serial.read();
            uint8_t boot = BOOT;
            if (ack == boot) {
              break;
            }
            else {
              Serial.write(BOOT);
            }
        }
    }
    Serial.write(BOOT);*/
    while(!Serial);
    Serial.println("hello");
    return 1;
}

void setupTimers() {
     
  TCCR1A = 0;                 
  TCCR1B = 0;                
  TCCR1B |= B00000100;       
  TIMSK1 |= B00000010;      
  OCR1A = 15625;//31250>>1;          
}
uint8_t startScreen() {
        
    delay(100); //turn on screen

    tft.reset(); //reset screen

    tft.begin(0x9341); //initialize tft
    tft.setRotation(1); //make landscape

    tft.fillScreen(BLACK);

    return 1;
}
uint8_t screenPower(bool power) {
    if (power) {
        digitalWrite(A5, HIGH);
        startScreen();
    }
    else
        digitalWrite(A5, LOW);
    return 1;
}
