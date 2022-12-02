//Device Properties
#define DEVICE_ID = 594385720; //randomly-chosen ID for this device
#define NETWORK_ID = 11122333; //networkID of our intials: K,L,W,C,C
//TEST TEST
//Lora Parameters
#define CS_PIN = 7;
#define RESET_PIN = 6;
#define IRQ_PIN = 1;  //THESE NEED TO BE CHANGED

#define BAUD_RATE 250000
#define CARRIER_FREQ 915E6

//Memory Allocation
#define NUM_MSG 10 //change to 100 later
#define NUM_LOG 100
#define NUM_CONTS 100

char messages[NUM_MSG][244] = {""}; // \0 //messages[0][0] = message1;
int logs[NUM_LOG] = {0};
char contacts[NUM_CONTS][16] = {0};
char messageToDisplay[256] = {0};
uint32_t messagePlaceholder = 0;
uint8_t handler = READY;
uint8_t expectedSerial = 1; //1 for waiting for code or ack; 64 for most other cases
char messageToSend[256] = {0};