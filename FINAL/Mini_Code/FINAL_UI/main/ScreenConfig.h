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
                                                                    {DEV_SCREEN_CODE,    DEV_BUTTON_X,    DEV_BUTTON_Y,    DEV_BUTTON_X_WIDTH,      DEV_BUTTON_Y_WIDTH},
                                                                    {SHUTDOWN_SCREEN_CODE,          SHUTDOWN_BUTTON_X,          SHUTDOWN_BUTTON_Y,          SHUTDOWN_BUTTON_X_WIDTH,            SHUTDOWN_BUTTON_Y_WIDTH},
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
uint16_t enterIDScreenButtons[COMPOSE_SCREEN_BUTTON_COUNT][5] =    {{CONTACTS_SCREEN_CODE,              CONTACTS_BUTTON_X,              CONTACTS_BUTTON_Y,              CONTACTS_BUTTON_X_WIDTH,                CONTACTS_BUTTON_Y_WIDTH},
                                                                    {ENTERIDSEND_SCREEN_CODE,              SEND_BUTTON_X,              SEND_BUTTON_Y,              SEND_BUTTON_X_WIDTH,                SEND_BUTTON_Y_WIDTH},
                                                                    {DELETE_SCREEN_CODE,              DELETE_BUTTON_X,              DELETE_BUTTON_Y,              DELETE_BUTTON_X_WIDTH,                DELETE_BUTTON_Y_WIDTH},
                                                                    {KEYBOARDNUM_SCREEN_CODE,              KEYBOARDNUM_BUTTON_X,              KEYBOARDNUM_BUTTON_Y,              KEYBOARDNUM_BUTTON_X_WIDTH,                KEYBOARDNUM_BUTTON_Y_WIDTH}};
uint16_t enterNameScreenButtons[COMPOSE_SCREEN_BUTTON_COUNT][5] =    {{CONTACTS_SCREEN_CODE,              CONTACTS_BUTTON_X,              CONTACTS_BUTTON_Y,              CONTACTS_BUTTON_X_WIDTH,                CONTACTS_BUTTON_Y_WIDTH},
                                                                    {ENTERNAMESEND_SCREEN_CODE,              SEND_BUTTON_X,              SEND_BUTTON_Y,              SEND_BUTTON_X_WIDTH,                SEND_BUTTON_Y_WIDTH},
                                                                    {DELETE_SCREEN_CODE,              DELETE_BUTTON_X,              DELETE_BUTTON_Y,              DELETE_BUTTON_X_WIDTH,                DELETE_BUTTON_Y_WIDTH},
                                                                    {KEYBOARD_SCREEN_CODE,              KEYBOARD_BUTTON_X,              KEYBOARD_BUTTON_Y,              KEYBOARD_BUTTON_X_WIDTH,                KEYBOARD_BUTTON_Y_WIDTH}};


uint16_t receivedScreenButtons[RECEIVED_SCREEN_BUTTON_COUNT][5] =         {{CONTACTS_SCREEN_CODE,          CONTACTS_BUTTON_X,          CONTACTS_BUTTON_Y,          CONTACTS_BUTTON_X_WIDTH,            CONTACTS_BUTTON_Y_WIDTH}};


uint16_t devScreenButtons[DEV_SCREEN_BUTTON_COUNT][5] =         {{BW125_SCREEN_CODE,          BW125_BUTTON_X,          BW125_BUTTON_Y,          BW125_BUTTON_X_WIDTH,            BW125_BUTTON_Y_WIDTH},
                                                                  {BW250_SCREEN_CODE,          BW250_BUTTON_X,          BW250_BUTTON_Y,          BW250_BUTTON_X_WIDTH,            BW250_BUTTON_Y_WIDTH},
                                                                  {BW500_SCREEN_CODE,          BW500_BUTTON_X,          BW500_BUTTON_Y,          BW500_BUTTON_X_WIDTH,            BW500_BUTTON_Y_WIDTH},
                                                                  {SF7_SCREEN_CODE,          SF7_BUTTON_X,          SF7_BUTTON_Y,          SF7_BUTTON_X_WIDTH,            SF7_BUTTON_Y_WIDTH},
                                                                  {SF9_SCREEN_CODE,          SF9_BUTTON_X,          SF9_BUTTON_Y,          SF9_BUTTON_X_WIDTH,            SF9_BUTTON_Y_WIDTH},
                                                                  {SF11_SCREEN_CODE,          SF11_BUTTON_X,          SF11_BUTTON_Y,          SF11_BUTTON_X_WIDTH,            SF11_BUTTON_Y_WIDTH},
                                                                  {CRCON_SCREEN_CODE,          CRCON_BUTTON_X,          CRCON_BUTTON_Y,          CRCON_BUTTON_X_WIDTH,            CRCON_BUTTON_Y_WIDTH},
                                                                  {CRCOFF_SCREEN_CODE,          CRCOFF_BUTTON_X,          CRCOFF_BUTTON_Y,          CRCOFF_BUTTON_X_WIDTH,            CRCOFF_BUTTON_Y_WIDTH},
                                                                  {AUTG_SCREEN_CODE,          AUTG_BUTTON_X,          AUTG_BUTTON_Y,          AUTG_BUTTON_X_WIDTH,            AUTG_BUTTON_Y_WIDTH},
                                                                  {SEND10_SCREEN_CODE,          SEND10_BUTTON_X,          SEND10_BUTTON_Y,          SEND10_BUTTON_X_WIDTH,            SEND10_BUTTON_Y_WIDTH},
                                                                  {MAXG_SCREEN_CODE,          MAXG_BUTTON_X,          MAXG_BUTTON_Y,          MAXG_BUTTON_X_WIDTH,            MAXG_BUTTON_Y_WIDTH}};
                                                                  

uint16_t keyboardButtons[5] =                                       {}; //runtime dispersion
char row[3][10] = {{'Q','W','E','R','T','Y','U','I','O','P'},
                   {'A','S','D','F','G','H','J','K','L','_'},
                   {'Z','X','C','V','B','N','M',',','.','\''}}; 
char rownum[10] = {'0','1','2','3','4','5','6','7','8','9'};
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

char currentContacts[2][128] = {0};
uint8_t reqContactsInd = 0;
char currentMessage[128] = {'\0'};
uint8_t currentScreen = MAIN_SCREEN_CODE;

uint8_t serialPipe[64] = {0};
uint8_t serialCounter = 0;

bool deleting = false;
bool booting = true;

uint16_t addID = 0;
char addName[12] = {'\0'};

bool buttonState;            
bool lastButtonState = 0;   

unsigned long lastDebounceTime = 0;  
unsigned long debounceDelay = 50; 


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
    Serial.write(BOOT);
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
    Serial.write(BOOT);
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
    else {
        tft.fillScreen(BLACK);
        //delay(500);
        digitalWrite(A5, LOW);
    }
    return 1;
}
