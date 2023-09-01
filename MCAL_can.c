#include "MCAL_can.h"
#include "tm4c123gh6pm.h"

int systick_counter = 0;

volatile bool g_bMsgObj3Sent = 0;
volatile bool g_bErrFlag = 0;
volatile uint32_t g_ui32Msg1Count = 0;

void systick_init(){
  
  NVIC_ST_CTRL_R |= 0;
  NVIC_ST_RELOAD_R |= 1599999; //clk delay for 100 msec
  NVIC_ST_CTRL_R |= 5;
  NVIC_ST_CURRENT_R |= 0;
  
}

void systick_polling(){
  
  if(NVIC_ST_CTRL_R & 0x10000){
	systick_counter ++;
        //schedeuler(T, 2);
  }
}

void
CANIntHandler(void)
{
    uint32_t ui32Status;

    // Read the CAN interrupt status to find the cause of the interrupt
    //
    ui32Status = CANIntStatus(CAN0_BASE, CAN_INT_STS_CAUSE);

    // If the cause is a controller status interrupt, then get the status
    //
    if(ui32Status == CAN_INT_INTID_STATUS)
    {
        // Read the controller status.  This will return a field of status
        // error bits that can indicate various errors.  Error processing
        // is not done in this example for simplicity.  Refer to the
        // API documentation for details about the error status bits.
        // The act of reading this status will clear the interrupt.  If the
        // CAN peripheral is not connected to a CAN bus with other CAN devices
        // present, then errors will occur and will be indicated in the
        // controller status.
        //
        ui32Status = CANStatusGet(CAN0_BASE, CAN_STS_CONTROL);

        // Set a flag to indicate some errors may have occurred.
        //
        g_bErrFlag = 1;
    }
    // Check if the cause is message object 1, which is used for sending
    // message 1.
    //
    else if(ui32Status == 1)
    {
        // Getting to this point means that the TX interrupt occurred on
        // message object 1, and the message TX is complete.  Clear the
        // message object interrupt.
        //
        CANIntClear(CAN0_BASE, 1);

        // Increment a counter to keep track of how many messages have been
        // sent.  In a real application this could be used to set flags to
        // indicate when a message is sent.
        //
        g_ui32Msg1Count++;

        // Since the message was sent, clear any error flags.
        //
        g_bErrFlag = 0;
    }
   
    else
    {
        // Spurious interrupt handling can go here.
    }
}


void can_init(void)
{
   SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_16MHZ);
   
   SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
   SysCtlDelay(3);
   
    GPIOPinConfigure(GPIO_PB4_CAN0RX);
    GPIOPinConfigure(GPIO_PB5_CAN0TX);
    
    GPIOPinTypeCAN(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5);
    
    SysCtlPeripheralEnable(SYSCTL_PERIPH_CAN0);
    SysCtlDelay(3);
    
    CANInit(CAN0_BASE);
    
    CANBitRateSet(CAN0_BASE, SysCtlClockGet(), 500000);
    
    CANIntRegister(CAN0_BASE, CANIntHandler); // if using dynamic vectors
    //
    CANIntEnable(CAN0_BASE, CAN_INT_MASTER | CAN_INT_ERROR | CAN_INT_STATUS);
    
    IntEnable(INT_CAN0);

    // Enable the CAN for operation.
    CANEnable(CAN0_BASE);
    
}