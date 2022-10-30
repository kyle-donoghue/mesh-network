//UART CODES
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


//Device Properties
#define DEVICE_ID 54321; //chosen ID for this device
#define NETWORK_ID 12345;

//Lora Parameters
#define CS_PIN D3;
#define RESET_PIN D2;
#define IRQ_PIN D1;

#define BAUD_RATE 250000
#define CARRIER_FREQ 915E6

//Memory Allocation
#define NUM_MSG 100 
#define NUM_LOG 100
#define NUM_CONTS 100