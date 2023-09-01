#include "MCAL_uart.h"
#include "uart.h"
#include "tm4c123gh6pm.h"

bool Tflag;
bool FIFOflag;
//uint8_t g_UART0_received;
char stringrecv[length];
uint8_t i;
int x,e;

int state = 0;


uint8_t string1[length];
//char fifo[25][8];

void
UART0_Init(void)
{
 // Enable the GPIO Peripheral used by the UART.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlDelay(3);

    // Enable UART0
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlDelay(3);

    // Configure GPIO Pins for UART mode.
    ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
    ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    // Use the internal 16MHz oscillator as the UART clock source.
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    // Initialize the UART for console I/O.
    UARTStdioConfig(0, UART0_BAUDRATE, 16000000);
}

  void UART0_SendByte(char byte)
{
    /* Send a byte. */
    UARTCharPut(UART0_BASE, byte);
}

void UART0_ReceiveByte(void)
{
        i=0;
        FIFOflag=false;
        /* Return received byte. */
        while (i < length && FIFOflag == false) {
                g_UART0_received = UART0_ReadChar();

                stringrecv[i] = g_UART0_received;
                i++;
                UART0_SendByte(g_UART0_received);
        }

        Tflag=true;

}

void UART0_SendString(char *ptr)
{  
    while(*ptr){
        UARTCharPut(UART0_BASE, *ptr++);
        //ptr++;
    }
}

void UART0_getString()
{
  int i = 0;
  
  while((string1[i-1] != '\r'))
  {
    string1[i] = UART0_ReadChar();
    i++;
  }
  state = 2;
}

void fifo_get(){ 
  static int i, j =0;
  
  //
  if((fifo[j][i-1]) != '\r'){
    for(j =0;j<25;j++){
       //while(fifo[j-1][i-1] != '\r'){
      while(i <= 8){
       
          fifo[j][i] =  UART0_ReadChar();
          i++;
        }
   //i = 0;
      }
  //i = 0;
 //}
    //}
  }
}

char UART0_ReadChar(void) {
  while ((UART0_FR_R & 0x10) != 0){}
    //wait until RXFE is 0
  
    return (UART0_DR_R);
}

void Message_Encryption (void)
{
       for(e = 0; e < length ; e++)
         //the Key is added to THE ASCII value of each character
         stringrecv[e] = stringrecv[e] + encryption_key;

       SysCtlDelay(5);
       UART0_SendString("\n\rEncrypted Message: ");
       for(e = 0; e < length ; e++)
           UART0_SendByte(stringrecv[e]);

}