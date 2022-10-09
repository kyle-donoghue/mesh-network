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

//Button Layout
#define MAIN_SCREEN_BUTTON_COUNT 2
#define CONTACTS_SCREEN_BUTTON_COUNT 9
#define LOG_SCREEN_BUTTON_COUNT 3
#define COMPOSE_SCREEN_BUTTON_COUNT 2
#define RECEIVED_SCREEN_BUTTON_COUNT 2
    //TO-DO: need other button defines

//TO-DO: UART defines