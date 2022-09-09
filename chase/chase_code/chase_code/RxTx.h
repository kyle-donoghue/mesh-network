#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define _10KB 10240

int handleMessage(message,nID,uID) {
    //byte referral
    networkID = messages[252:255];
    if (networkID !== nID) {
        return 1;
    }
    messageID = messages[248:251];
    if (messageExists(messageID)) {
        return 1;
    }
    
    logMessageID(messageID);

    userID = message[244:247];
    if (userID == uID) {
        senderID = message[240:243];
        messageContents = message[0:239];
        storeMessage(messageContents,senderID);
    }
    else {
        transmitMessage(message);
    }
}

int messageExists(mID)

int setupMessageIDLog() {

    //found this which might help: https://stackoverflow.com/questions/51986342/allocating-10kb-of-memory-and-printing-addresses-and-content

    int *ptr;//this will hold the address of the first byte
    ptr = (int*) malloc (_10KB * (sizeof(int)*1000));

    if(ptr==NULL)
    {
      printf("Memory not allocated. \n");
    }
    else
    {
        printf("Memory allocated succesfully. \n");
    }
    return ptr
}
int getMessageID(messageNum) {
    ...
}
//messageIDn = *(pointer+(n-1)*4)