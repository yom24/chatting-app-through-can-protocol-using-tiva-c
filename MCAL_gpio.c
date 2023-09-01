#include "MCAL_gpio.h"
#include "HAL_gpio.h"

int q = 0;

void gpio_init(){
  
    //SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|
     //              SYSCTL_XTAL_16MHZ);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)){}
    
    // Unlock the Port F Pin 0 GPIO as it defaults as a locked NMI pin.
    //
    MAP_GPIOUnlockPin(GPIO_PORTF_BASE, GPIO_PIN_0);
    MAP_GPIOUnlockPin(GPIO_PORTF_BASE, GPIO_PIN_4);
    
              
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, LEDS);
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, PINS);
        
	GPIOPadConfigSet(GPIO_PORTF_BASE, PINS, GPIO_STRENGTH_2MA,
                         GPIO_PIN_TYPE_STD_WPU);
	GPIOIntEnable(GPIO_PORTF_BASE, PINS);
	GPIOIntTypeSet(GPIO_PORTF_BASE, PINS, GPIO_RISING_EDGE);
	IntPrioritySet(INT_GPIOF, 0);
	IntRegister(INT_GPIOF, GPIOIntHandler);
	IntEnable(INT_GPIOF);
	IntMasterEnable();
}

void GPIOIntHandler(void){
  //handler function to do here
  
  GPIOIntClear(GPIO_PORTF_BASE, PINS);
  int state = GPIOIntStatus(GPIO_PORTF_BASE, true);
  //debounce();
  
  //
  
 //
  switch(state){
  
  case 1:  
    //data collection state
      LED_toggle(RED);
      break;
      
  case 16:
    LED_toggle(BLUE);
    break;
    
  case 0:
    break;
  }
}

//
int button_state(){

     int volatile btn1 = 0;
     int volatile btn2 = 0;
      int state1 = 0;
     
     btn1 = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4);
     btn2 = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0);
    
  if(btn1 == 0){
    SysCtlDelay(250);
    if((GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4) == 0)){
      SysCtlDelay(250);
      state1 = 1;
    }
  }
  else if(btn2 == 0){
      SysCtlDelay(250);
      if((GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0)) ==0){
        state1 = 2;
      }
  }
    
  return state1;
}


void LED_toggle(uint8_t led_number){
  
  GPIOPinWrite(GPIO_PORTF_BASE, LEDS, led_number);
}