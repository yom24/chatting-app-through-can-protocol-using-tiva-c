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
#include "driverlib/interrupt.h"
#include "driverlib/can.h"
#include "inc/hw_can.h"
#include "inc/hw_ints.h"

extern tCANMsgObject g_sCANMsgObject1;

void systick_polling();
void systick_init();
void can_init(void);