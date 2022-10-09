void displayMessage(char message[256],uint8_t num) {
    //screen should already be on a screen that displays a list of messages
    
}

void displayContacts() {//use global variable currentContacts
    //screen should already be on a screen that can display contacts
}

void messageReceived(char message[256]) {
    //change state to received message screen
    //display new message with buttons described in array
    uint32_t senderID = 0;
    for (uint8_t i = 0; i < 4; i++){
        senderID += LoRa.read();
        if (i < 3)
            senderID = senderID<<8;
    }
    char messageText[240] = {0};
    for (uint8_t i = 4; i < 244; i++){
        messageText[i] = message[i];
    }
    //draw receive screen
    //draw senderID
    //TO-DO: reference senderID with name
    //draw text
    //TO-DO: actually draw this
}

//all other screen drawings i.e. drawContactsScreen()

//use requestMessage and requestContacts when drawing to get data

void requestMessage(uint8_t numMessage, uint8_t num) { //get the nth message
    Serial.Write(MSG_DISP);
    expectedSerial = 64;
    handler = RECEIVE_N1;
    switch(num) {
        case 1:
            messageHandler = MSG_LOG1;
            break;
        case 2:
            messageHandler = MSG_LOG2;
            break;
        case 3:
            messageHandler = MSG_LOG3;
            break;
        case 4:
            messageHandler = MSG_LOG4;
            break;
        default: ;
    }
    return;
}

void requestContacts(uint8_t firstContact) { //get 4 contacts at once
    Serial.Write(REQ_CONTS);
    expectedSerial = 64;
    handler = CONTS_REC;
}