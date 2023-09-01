#include "HAL_can.h"

volatile uint32_t g_ui32Msg1Count = 0;

volatile uint32_t g_ui32MsgCount = 0;
volatile bool g_bRXFlag1 = 0;


volatile bool g_bErrFlag = 0;

tCANMsgObject g_sCANMsgObject1;
uint8_t g_pui8Msg1[8];

tCANMsgObject sCANMessage;
uint8_t pui8MsgData[8];


void can_send();
void can_enable();

void
CANIntHandlerREC(void)
{
   uint32_t ui32Status;

    //
    // Read the CAN interrupt status to find the cause of the interrupt
    //
    ui32Status = CANIntStatus(CAN0_BASE, CAN_INT_STS_CAUSE);

    //
    // If the cause is a controller status interrupt, then get the status
    //
    if(ui32Status == CAN_INT_INTID_STATUS)
    {
        //
        // Read the controller status.  This will return a field of status
        // error bits that can indicate various errors.  Error processing
        // is not done in this example for simplicity.  Refer to the
        // API documentation for details about the error status bits.
        // The act of reading this status will clear the interrupt.
        //
        ui32Status = CANStatusGet(CAN0_BASE, CAN_STS_CONTROL);

        //
        // Set a flag to indicate some errors may have occurred.
        //
        g_bErrFlag = 1;
    }

    //
    // Check if the cause is message object 1.
    //
    else if(ui32Status == 2)
    {
        //
        // Getting to this point means that the RX interrupt occurred on
        // message object 1, and the message reception is complete.  Clear the
        // message object interrupt.
        //
        CANIntClear(CAN0_BASE, 2);

        //
        // Increment a counter to keep track of how many messages have been
        // received.  In a real application this could be used to set flags to
        // indicate when a message is received.
        //
        g_ui32MsgCount++;

        //
        // Set flag to indicate received message is pending for this message
        // object.
        //
        g_bRXFlag1 = 1;
        
        //receive message here
        sCANMessage.pui8MsgData = pui8MsgData;

            //
            // Read the message from the CAN.  Message object number 1 is used
            // (which is not the same thing as CAN ID).  The interrupt clearing
            // flag is not set because this interrupt was already cleared in
            // the interrupt handler.
            //
            CANMessageGet(CAN0_BASE, 2, &sCANMessage, 0);

            //
            // Clear the pending message flag so that the interrupt handler can
            // set it again when the next message arrives.
            //
            g_bRXFlag1 = 0;
            
            PrintCANMessageRECInfo(&sCANMessage, 2);

        //
        // Since a message was received, clear any error flags.
        //
        g_bErrFlag = 0;
       // ui32Status =0;
       // IntMasterDisable();
      //IntMasterEnable();
    }
    //
    // Otherwise, something unexpected caused the interrupt.  This should
    // never happen.
    //
    else
    {
        CANIntClear(CAN0_BASE,2);
        IntMasterDisable();
        CANIntUnregister(CAN0_BASE);
        IntMasterEnable();
        
        // Spurious interrupt handling can go here.
        //
    }
}

void
CANIntHandler(void)
{
    uint32_t ui32Status;

    //
    // Read the CAN interrupt status to find the cause of the interrupt
    //
    ui32Status = CANIntStatus(CAN0_BASE, CAN_INT_STS_CAUSE);

    //
    // If the cause is a controller status interrupt, then get the status
    //
    if(ui32Status == CAN_INT_INTID_STATUS)
    {
        //
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

        //
        // Set a flag to indicate some errors may have occurred.
        //
        g_bErrFlag = 1;
    }

    //
    // Check if the cause is message object 1, which is used for sending
    // message 1.
    //
    else if(ui32Status == 1)
    {
        //
        // Getting to this point means that the TX interrupt occurred on
        // message object 1, and the message TX is complete.  Clear the
        // message object interrupt.
        //
        CANIntClear(CAN0_BASE, 1);

        //
        // Increment a counter to keep track of how many messages have been
        // sent.  In a real application this could be used to set flags to
        // indicate when a message is sent.
        //
        g_ui32Msg1Count++;
        
        //
        // Since the message was sent, clear any error flags.
        //
        g_bErrFlag = 0;
    }

    
    else
    {
        //
        // Spurious interrupt handling can go here.
        //
    }
}

void
InitConsole(void)
{
    //
    // Enable GPIO port A which is used for UART0 pins.
    // TODO: change this to whichever GPIO port you are using.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlDelay(3);

    //
    // Configure the pin muxing for UART0 functions on port A0 and A1.
    // This step is not necessary if your part does not support pin muxing.
    // TODO: change this to select the port/pin you are using.
    //
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);

    //
    // Enable UART0 so that we can configure the clock.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlDelay(3);

    //
    // Use the internal 16MHz oscillator as the UART clock source.
    //
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    //
    // Select the alternate (UART) function for these pins.
    // TODO: change this to select the port/pin you are using.
    //
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Initialize the UART for console I/O.
    //
    UARTStdioConfig(0, 115200, 16000000);
}


void
PrintCANMessageInfo(tCANMsgObject *psCANMsg, uint32_t ui32MsgObj)
{
    unsigned int uIdx;

    UARTprintf("Sending msg: obj=%d ID=0x%04X msg= ", ui32MsgObj,
               psCANMsg->ui32MsgID);
    for(uIdx = 0; uIdx < psCANMsg->ui32MsgLen; uIdx++)
    {
        UARTprintf("%c", psCANMsg->pui8MsgData[uIdx]);
    }
    UARTprintf("\n");
}

void
PrintCANMessageRECInfo(tCANMsgObject *psCANMsg, uint32_t ui32MsgObj)
{
    unsigned int uIdx;

    //
    // Check to see if there is an indication that some messages were
    // lost.
    //
    if(psCANMsg->ui32Flags & MSG_OBJ_DATA_LOST)
    {
        UARTprintf("CAN message loss detected on message object %d\n",
                   ui32MsgObj);
    }

    //
    // Print out the contents of the message that was received.
    //
    UARTprintf("Msg Obj=%u ID=0x%05X len=%u data= ", ui32MsgObj,
               psCANMsg->ui32MsgID, psCANMsg->ui32MsgLen);
    for(uIdx = 0; uIdx < psCANMsg->ui32MsgLen; uIdx++)
    {
        UARTprintf("%c", psCANMsg->pui8MsgData[uIdx]);
    }
    UARTprintf("\n");
}

void can_enable(){
  
 // CANDisable(CAN0_BASE);
  SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_16MHZ);
  
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
  
  GPIOPinConfigure(GPIO_PB4_CAN0RX);
    GPIOPinConfigure(GPIO_PB5_CAN0TX);
    
    GPIOPinTypeCAN(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    //
    // The GPIO port and pins have been set up for CAN.  The CAN peripheral
    // must be enabled.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_CAN0);
    
  CANInit(CAN0_BASE);
  
  CANBitRateSet(CAN0_BASE, SysCtlClockGet(), 500000);
  
  IntMasterDisable();
  CANIntRegister(CAN0_BASE, CANIntHandlerREC);
  
  CANIntEnable(CAN0_BASE, CAN_INT_MASTER | CAN_INT_ERROR | CAN_INT_STATUS);

    //
    // Enable the CAN interrupt on the processor (NVIC).
    //
    IntEnable(INT_CAN0);
    IntMasterEnable();
  
  CANEnable(CAN0_BASE);
  
  g_sCANMsgObject1.ui32MsgID = 0x1001;
    g_sCANMsgObject1.ui32MsgIDMask = 0;
    g_sCANMsgObject1.ui32Flags = MSG_OBJ_TX_INT_ENABLE;
    g_sCANMsgObject1.ui32MsgLen = sizeof(g_pui8Msg1);
    
     sCANMessage.ui32MsgID = 0x2001;
    sCANMessage.ui32MsgIDMask = 0xfffff;
    sCANMessage.ui32Flags = (MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER |
                             MSG_OBJ_EXTENDED_ID);
    sCANMessage.ui32MsgLen = 8;
    
    CANMessageSet(CAN0_BASE, 2, &sCANMessage, MSG_OBJ_TYPE_RX);
}

void can_rec_enable(){
  
  //CANDisable(CAN0_BASE);
  SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_16MHZ);
  //InitConsole();
  
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
  
  GPIOPinConfigure(GPIO_PB4_CAN0RX);
    GPIOPinConfigure(GPIO_PB5_CAN0TX);
    
    GPIOPinTypeCAN(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5);
    
    SysCtlPeripheralEnable(SYSCTL_PERIPH_CAN0);
    
    CANInit(CAN0_BASE);
    
    CANBitRateSet(CAN0_BASE, SysCtlClockGet(), 500000);
    
    CANIntRegister(CAN0_BASE, CANIntHandlerREC);
    
    CANIntEnable(CAN0_BASE, CAN_INT_MASTER | CAN_INT_ERROR | CAN_INT_STATUS);
    
    IntEnable(INT_CAN0);

    //
    // Enable the CAN for operation.
    //
    CANEnable(CAN0_BASE);
    
  sCANMessage.ui32MsgID = 0x2001;
    sCANMessage.ui32MsgIDMask = 0xfffff;
    sCANMessage.ui32Flags = (MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER |
                             MSG_OBJ_EXTENDED_ID);
    sCANMessage.ui32MsgLen = 8;
    
    CANMessageSet(CAN0_BASE, 2, &sCANMessage, MSG_OBJ_TYPE_RX);
}

void can_send(uint8_t x[]){
  //can_enable();
  PrintCANMessageInfo(&g_sCANMsgObject1, 1);
  g_sCANMsgObject1.pui8MsgData = x;
        CANMessageSet(CAN0_BASE, 1, &g_sCANMsgObject1, MSG_OBJ_TYPE_TX);
  
}

void can_rec(){
  if(g_bRXFlag1 ==1){
 sCANMessage.pui8MsgData = pui8MsgData;

            //
            // Read the message from the CAN.  Message object number 1 is used
            // (which is not the same thing as CAN ID).  The interrupt clearing
            // flag is not set because this interrupt was already cleared in
            // the interrupt handler.
            //
            CANMessageGet(CAN0_BASE, 2, &sCANMessage, 0);

            //
            // Clear the pending message flag so that the interrupt handler can
            // set it again when the next message arrives.
            //
            g_bRXFlag1 = 0;
            
            PrintCANMessageRECInfo(&sCANMessage, 2);
  }
}

void can_reg(){
  IntMasterEnable();
  CANIntRegister(CAN0_BASE, CANIntHandlerREC);
}

void can_unreg(){
  IntMasterDisable();
  CANIntUnregister(CAN0_BASE);
}