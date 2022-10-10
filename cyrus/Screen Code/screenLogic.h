/*
 * screenBox.h
 *
 *  Created on: Oct 7, 2022
 *      Author: cyrus
 */

#include "Elegoo_GFX.h"
#include "Elegoo_TFTLCD.h"
#include "Eleego_Touchscreen.h"



#ifndef SCREENBOX_H_
#define SCREENBOX_H_



#define NUM_MSG 10 //change to 100 later
#define BAUD_RATE 250000
#define NUM_LOG 100

//Screen Codes from google sheet
#define Log_Screen_Code 0000000000000001
#define Main_Screen_Code 0000000000000010
#define Log_Page_Down 0000000000000011
#define Log_Page_Up 0000000000000100
#define Send_Screen_Code 0000000000000101
#define Add_Contact 0000000000000110
#define Delete_Contact 0000000000000111
#define Contacts_Page_Down 0000000000001000
#define Contacts_Page_Up 0000000000001001
#define Contact_1 0000000000001010
#define Contact_2 0000000000001011
#define Contact_3 0000000000001100
#define Contact_4 0000000000001101
#define Contacts_Screen 0000000000001110


// DEFINE COLORS IF NEEDED
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define LCD_CS A3		//Chip select Analog 3
#define LCD_CD A2 		//Command/Data goes to Analog 2
#define LCD_WR A1		// LCD WRITE goes to Analog 1
#define LCD_RD A0		// LCD READ goes to Analog 0
#define LCD_RESET A4 	// Reset pin

// Calibrate TouchScreen data to the screen coordinates
#define TS_MINX 150
#define TS_MINY 130
#define TS_MAXX 3800
#define TS_MAXY 4000

Elegoo_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LOC_RD,LCD_RESET);

/*
	BIG NOTE: THE SCREEN PIXELS ARE 240 X 320 PIXELS
	USE tft.width() and tft.height() to adjust for where the user is in the screen
*/


void setup(){
	tft.reset();			// reset LCD
	tft.begin(0x9341);		// initialize LCD
	tft.fillScreen(BLACK);	// set background black
}//end setup

/*
 * Has contacts button, we got rid of the Logs button
 * Should have device ID
 */
void mainScreen(){
	tft.fillscreen(BLACK);		// black background
	tft.setCursor(0,0);			// Center's the cursor
    tft.setTextSize(1);
    tft.setTextColor(WHITE);
    tft.println("ID: 159327548715");

}//end mainScreen


/*
 * Has home button and left/right button
 * Should display 2 contacts, separated by a line
 */
void contactScreen(){

}// end contactScreen


/*
 * change screen state, if necessary (i.e. page left/right doesn't need change screen)
 * update global var currentScreen
 * draw screen
    --> request relevant data from xiao; i.e. contacts list, messages
 */
void handleScreen(uint16_t screenCode) {

	uint16_t state = screenCode;
	
	switch(state){
	case Main_Screen_Code:
			mainScreen();
		break;


	case Contacts_Screen:
		    contactScreen();// call the contact screen
		break;//end contacts

	default: state = Main_Screen_Code;
        break;

	}


}// end handleScreen



#endif /* SCREENBOX_H_ */
