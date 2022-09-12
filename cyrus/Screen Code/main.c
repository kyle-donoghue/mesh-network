#include screen.library
#include UART.h

#define MAINSCREENBUTTONCOUNT = 2

#define CONTACTSSCREEN = 12
#define CONTACTSBUTTONx = 40
#define CONTACTSBUTTONy = 40
#define CONTACTSBUTTONxwidth = 40
#define CONTACTSBUTTONywidth = 40

global int mainScreenButtons[2][5] = [CONTACTSSCREEN, CONTACTSBUTTONx, CONTACTSBUTTONy, CONTACTSBUTTONxwidth, CONTACTSBUTTONywidth], [same for log button];



setup() {

    screenPower(1); //1 = om, 0 = off
    sleep(100); //ms
    setupScreen(); // turn on screen

    setupSerialComm(); //establish serial comms with Xiao

    sendReady(); // Tell Xiao that screen is ready

    setupInterrupts();

}

loop() {
    int pos[3] = getTouch();
    if (isPressed(pos[2])) {
        int buttonPressed = findButton(pos[0],pos[1]);
        sendButtonPress(buttonPressed); // sends button press to Xiao
    }
}

int findButton(int x, int y) {

    if(currentScreen = MAINSCREEN) {

        for(int i = 0; i < MAINSCREENBUTTONCOUNT; i++){

            if( !((x >= mainScreenButtons[i][1]) && (x <= mainScreenButtons[i][1]+mainScreenButtons[i][3])) ){ // if x , then go to contacts/logs
                continue;
            }
            if( !((y >= mainScreenButtons[i][2]) && (y <= mainScreenButtons[i][2]+mainScreenButtons[i][4])) ){ // if y matches, then go to contacts/logs
                continue;
            }
            return mainScreenButtons[i][0];
        }
        return 0;
    }
}

INTERRUPT getUART {
    int screenCode = uart.get();
    int screenData = uart.get(); // dynamic value passed to screen set to determine which screen page to present
    screenSet(screenCode,screenData);
}

// Picks from the screenStorage file and display the screen 
int screenSet(screenCode,screenData) {
    case screenCode {
        screenCode = 12 {
            setContactsScreen(screenData);
        }
        screenCode = 11 {
            ...
        }
    }
}

