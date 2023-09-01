#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_can.h"
#include "inc/hw_ints.h"
#include "inc/hw_nvic.h"
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "driverlib/can.h"
#include "driverlib/debug.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "utils/ringbuf.h"
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"

typedef void (*operation)();


typedef struct  {
  int number;
  //uint32 periodicty;
  operation opt;
  
} _schd;

extern _schd S[2];

extern tRingBufObject buffer;
void CAN_send_ISR();
void save_ring();
void systick();
void create_task(_schd s[], int number);
void schedeuler(_schd s[], int state);
extern int flag;