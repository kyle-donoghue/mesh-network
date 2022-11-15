#ifndef SCREENDEFINES_H_
#define SCREENDEFINES_H_

#define DEVICE_ID 1
#define NUM_CONTS 10

//Touch Screen
#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

#define TS_MINX 120
#define TS_MAXX 900
#define TS_MINY 70
#define TS_MAXY 920


#define MIN_PRESS 10
#define MAX_PRESS 1000

//TFT
#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4 //correct based on PCB layout
//Wowki specific
#define TFT_DC 9
#define TFT_CS 10

//Button Layout
#define MAIN_SCREEN_BUTTON_COUNT 2
#define CONTACTS_SCREEN_BUTTON_COUNT 7
#define LOG_SCREEN_BUTTON_COUNT 3
#define COMPOSE_SCREEN_BUTTON_COUNT 4
#define RECEIVED_SCREEN_BUTTON_COUNT 2
    //TO-DO: need other button defines

//TO-DO: UART defines
#define BAUD_RATE 9600


//// MAIN SCREEN DEFINES ////
// 100 x 80 pixels, fillRect(CONTACTS_BUTTON_X, CONTACTS_BUTTON_Y, CONTACTS_BUTTON_X_WIDTH, CONTACTS_BUTTON_Y_WIDTH)
//#define CONTACTS_SCREEN_CODE 0000000000001110
#define CONTACTS_BUTTON_X    5 //    tft.fillTriangle(0,10,20,0,20,20,BLUE); // TODO --> MAKE DEFINES
#define CONTACTS_BUTTON_Y    5
#define CONTACTS_BUTTON_X_WIDTH 40
#define CONTACTS_BUTTON_Y_WIDTH 40

// 90 x 80 pixels, fillRect(LOG_BUTTON_X, LOG_BUTTON_Y, LOG_BUTTON_X_WIDTH, LOG_BUTTON_Y_WIDTH)
//#define LOG_SCREEN_CODE 0000000000000001
#define LOG_BUTTON_X    0
#define LOG_BUTTON_Y    150
#define LOG_BUTTON_X_WIDTH 90
#define LOG_BUTTON_Y_WIDTH 80
/****************************/


//// CONTACTS SCREEEN DEFINES ////
// 30 x 25 pixels
//#define MAIN_SCREEN_CODE 0000000000000010
#define MAIN_BUTTON_X   0
#define MAIN_BUTTON_Y   0
#define MAIN_BUTTON_X_WIDTH 30
#define MAIN_BUTTON_Y_WIDTH 25

// 80 x 80 pixels
//#define ADD_CONTACT_SCREEN_CODE 0000000000000110
#define ADD_CONTACT_BUTTON_X    20
#define ADD_CONTACT_BUTTON_Y    20
#define ADD_CONTACT_BUTTON_X_WIDTH 20
#define ADD_CONTACT_BUTTON_Y_WIDTH 80

// 80 x 80 pixels
//#define DELETE_CONTACT_SCREEN_CODE  0000000000000111
#define DELETE_CONTACT_BUTTON_X 260
#define DELETE_CONTACT_BUTTON_Y 20
#define DELETE_CONTACT_BUTTON_X_WIDTH 40
#define DELETE_CONTACT_BUTTON_Y_WIDTH 20

// 40 x 80 pixels
//#define CONTACTS_PG_DOWN_SCREEN_CODE   0000000000001000
#define CONTACTS_PG_DOWN_BUTTON_X   290
#define CONTACTS_PG_DOWN_BUTTON_Y   210
#define CONTACTS_PG_DOWN_BUTTON_X_WIDTH 40
#define CONTACTS_PG_DOWN_BUTTON_Y_WIDTH 40

// 40 x 80 pixels
#define CONTACTS_PG_UP_SCREEN_CODE  0000000000001001
#define CONTACTS_PG_UP_BUTTON_X 290
#define CONTACTS_PG_UP_BUTTON_Y 60
#define CONTACTS_PG_UP_BUTTON_X_WIDTH 40
#define CONTACTS_PG_UP_BUTTON_Y_WIDTH 40

// Contact list 1-4 are 260 x 37
//#define CONTACT1_SCREEN_CODE    0000000000001010
#define CONTACT1_BUTTON_X   0
#define CONTACT1_BUTTON_Y   95
#define CONTACT1_BUTTON_X_WIDTH 300
#define CONTACT1_BUTTON_Y_WIDTH 37

//#define CONTACT2_SCREEN_CODE    0000000000001011
#define CONTACT2_BUTTON_X 0
#define CONTACT2_BUTTON_Y 165 
#define CONTACT2_BUTTON_X_WIDTH 300
#define CONTACT2_BUTTON_Y_WIDTH 37


//#define CONTACT3_SCREEN_CODE    0000000000001100
#define CONTACT3_BUTTON_X 0
#define CONTACT3_BUTTON_Y 164
#define CONTACT3_BUTTON_X_WIDTH 260
#define CONTACT3_BUTTON_Y_WIDTH 37


//#define CONTACT4_SCREEN_CODE    0000000000001101
#define CONTACT4_BUTTON_X 0
#define CONTACT4_BUTTON_Y 201
#define CONTACT4_BUTTON_X_WIDTH 260
#define CONTACT4_BUTTON_Y_WIDTH 37
/****************************/

// COMPOSE BUTTONS //

#define CONTACT4_BUTTON_X 0
#define CONTACT4_BUTTON_Y 201
#define CONTACT4_BUTTON_X_WIDTH 260
#define CONTACT4_BUTTON_Y_WIDTH 37

#define CONTACT4_BUTTON_X 0
#define CONTACT4_BUTTON_Y 201
#define CONTACT4_BUTTON_X_WIDTH 260
#define CONTACT4_BUTTON_Y_WIDTH 37





//// LOGS SCREEN DEFINES ////
/* Can center the main button, but have to change variable name. Already defined in CONTACTS DEFINES.
   
    #define MAIN_BUTTON_X
    #define MAIN_BUTTON_Y
    #define MAIN_BUTTON_X_WIDTH
    #define MAIN_BUTTON_Y_WIDTH

*/

// 40 x 80 pixels
#define LOG_PG_DOWN_BUTTON_X    280
#define LOG_PG_DOWN_BUTTON_Y    100
#define LOG_PG_DOWN_BUTTON_X_WIDTH  40
#define LOG_PG_DOWN_BUTTON_Y_WIDTH  80
 
#define LOG_PG_UP_BUTTON_X  280 
#define LOG_PG_UP_BUTTON_Y  90
#define LOG_PG_UP_BUTTON_X_WIDTH 40
#define LOG_PG_UP_BUTTON_Y_WIDTH 80


#define SHUTDOWN_BUTTON_X  10 
#define SHUTDOWN_BUTTON_Y  210
#define SHUTDOWN_BUTTON_X_WIDTH 60
#define SHUTDOWN_BUTTON_Y_WIDTH 20

/****************************/

//// COMPOSE SCREEN BUTTONS DEFINES ////
/* Can center the main button, but have to change variable name. Already defined in CONTACTS DEFINES.
   
    #define MAIN_BUTTON_X
    #define MAIN_BUTTON_Y
    #define MAIN_BUTTON_X_WIDTH
    #define MAIN_BUTTON_Y_WIDTH

*/

// Bottom right, 40 x 80 pixels
#define SEND_BUTTON_X 270
#define SEND_BUTTON_Y 5
#define SEND_BUTTON_X_WIDTH 40
#define SEND_BUTTON_Y_WIDTH 20

#define ENTERID_BUTTON_X 270
#define ENTERID_BUTTON_Y 5
#define ENTERID_BUTTON_X_WIDTH 40
#define ENTERID_BUTTON_Y_WIDTH 20

#define ENTERNAME_BUTTON_X 270
#define ENTERNAME_BUTTON_Y 5
#define ENTERNAME_BUTTON_X_WIDTH 40
#define ENTERNAME_BUTTON_Y_WIDTH 20
#define DELETE_BUTTON_X 225
#define DELETE_BUTTON_Y 5
#define DELETE_BUTTON_X_WIDTH 40
#define DELETE_BUTTON_Y_WIDTH 20

#define KEYBOARD_BUTTON_X   0
#define KEYBOARD_BUTTON_Y   90
#define KEYBOARD_BUTTON_X_WIDTH 320
#define KEYBOARD_BUTTON_Y_WIDTH 150
#define KEYBOARDNUM_BUTTON_X   0
#define KEYBOARDNUM_BUTTON_Y   90
#define KEYBOARDNUM_BUTTON_X_WIDTH 320
#define KEYBOARDNUM_BUTTON_Y_WIDTH 50
#define KEY_XWIDTH 32
#define KEY_YWIDTH 50
#define CURSOR_X 16 // POS/2
#define CURSOR_Y 18
#define CURSOR_XWIDTH 4
#define CURSOR_YWIDTH 7
#define TEXT_BUFFER_MAX 112

/****************************/

//// RECEIVED SCREEN DEFINES ////

#define COMPOSE_BUTTON_X    10
#define COMPOSE_BUTTON_Y    290
#define COMPOSE_BUTTON_X_WIDTH  80
#define COMPOSE_BUTTON_Y_WIDTH  20

/****************************/


//UART Codes
#define READY 1
#define REQ_CONTS 2
#define DEL_CONT 3
#define ADD_CONT 4
#define SEND_N1 5
#define SEND_N2 6
#define SEND_N3 7
#define SEND_N4 8
#define ACK 9
#define REPEAT 10
#define BOOT 11
#define MESSAGE_REC 12
#define MSG_DISP 13
#define MSG_SEND 14
#define MSG_TO_ME 15
#define RECEIVE_N1 16
#define RECEIVE_N2 17
#define RECEIVE_N3 18
#define RECEIVE_N4 19
#define MSG_LOG1 20
#define MSG_LOG2 21
#define MSG_LOG3 22
#define MSG_LOG4 23
#define CONTS_REC_N1 24
#define CONTS_REC_N2 25
#define CONTS_REC_N3 26
#define CONTS_REC_N4 27
#define SHUTDOWN 28
#define BATTERYREQ 30

//Screen Codes
#define LOG_SCREEN_CODE 1
#define MAIN_SCREEN_CODE 2
#define LOG_PG_DOWN_SCREEN_CODE 3
#define LOG_PG_UP_SCREEN_CODE 4
#define SEND_SCREEN_CODE 5
#define ADD_CONTACT_SCREEN_CODE 6
#define DELETE_CONTACT_SCREEN_CODE 7
#define CONTACTS_PG_DOWN_SCREEN_CODE 8
#define CONTACTS_PG_UP_SCREEN_CODE 9
#define CONTACT1_SCREEN_CODE 10
#define CONTACT2_SCREEN_CODE 11
#define CONTACT3_SCREEN_CODE 12
#define CONTACT4_SCREEN_CODE 13
#define CONTACTS_SCREEN_CODE 14
#define COMPOSE_SCREEN_CODE 15
#define RECEIVED_SCREEN_CODE 16
#define KEYBOARD_SCREEN_CODE 17
#define KEYBOARDNUM_SCREEN_CODE 17
#define DELETE_SCREEN_CODE 18
#define SHUTDOWN_SCREEN_CODE 19
#define ENTERID_SCREEN_CODE 20
#define ENTERIDSEND_SCREEN_CODE 20
#define ENTERNAME_SCREEN_CODE 21
#define ENTERNAMESEND_SCREEN_CODE 21

#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#endif
