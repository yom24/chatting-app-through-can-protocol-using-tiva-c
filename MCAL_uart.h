#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/flash.h"
#include "driverlib/adc.h"
#include "driverlib/ssi.h"
#include "utils/uartstdio.h"
#include "driverlib/rom_map.h"


#define UART0_BAUDRATE 115200
#define length 200
#define encryption_key 1

extern int flag;

extern bool Tflag;
extern bool FIFOflag;
extern uint8_t g_UART0_received;
extern char stringrecv[length];
extern uint8_t i;
extern int x,e;
extern int state;
extern char fifo[25][8];
extern uint8_t string1[length];

void UART0_Init(void);
void UART0_SendByte(char byte);
void UART0_ReceiveByte(void);
void UART0_SendString(char *ptr);
char UART0_ReadChar(void);
void UART0_getString();
void Message_Encryption(void);
void UARTSend(const uint8_t *pui8Buffer, uint32_t ui32Count);
void fifo_get();
