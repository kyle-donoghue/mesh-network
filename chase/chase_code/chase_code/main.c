#include lora.library
#include SPI.h
#include RxTx.h

global devideID = ArduinoUniqueID;
global NetworkID = sameStaticIDAcrossDevices;
global IDpointer = sklfashg;;

#define MAINSCREEN = 1

int setscreen(screenNum, screenData)

setup() {

    setupLora()

    setupSerialComm()

    IDpointer = setupMessageIDLog()

    setupMessageContentsLog()

    while(!screenReady()){}

    setScreen(MAINSCREEN,null)
}
//maybe enable power save in loop
loop() {

}

INTERRUPT messageReceived {

    stopInterrupts()

    newMEssage = lora.getMessage()

    handleMessage(newMessage,NetworkID,devideID);

    startInterrupts()
}

INTERRUPT UARTReceived {
    if (bits[1:4] == buttonpressed)
        buttonthatwaspressed = bits[5:8]
}