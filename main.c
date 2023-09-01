#include "HAL_can.h"
#include "HAL_ring_buff.h"
#include "MCAL_uart.h"
#include "MCAL_gpio.h"
#include "HAL_gpio.h"


    void main(){
    
    InitConsole();
    can_enable();
    
    create_task(S, 1);
    create_task(S, 2);
    gpio_init();
    
    while(1){
      
      int switch1 = 0;
      switch1 = button_state();
      switch(switch1){
      case 0:
        //idle
        can_reg();
        LED_toggle(BLUE);
        //can_enable();
        break;
      
      case 1:
        can_unreg();
        LED_toggle(GREEN);
        schedeuler(S, 1);
        while(!(button_state() == 2));
        //CANDisable(CAN0_BASE);
        
        schedeuler(S, 2);
          switch1 = 0;
          can_reg();
        break;
        
      //case 2:
        //LED_toggle(GREEN);
        //schedeuler(S ,switch1);
        //break;
        
      }
        

      
    }
    //UART0_getString();
    //save_ring();
    
     // can_send();
}




/*

//schedeuler(S);
      
     // IntMasterDisable();
     // IntMasterEnable();
      LED_toggle(BLUE);
     // int c = 0;
      //can_rec_enable();
      int c = 0;
        c = button_state();
      //ile(button_state()!=1);
     // IntMasterDisable();
      //IntMasterEnable();
     // CANDisable(CAN0_BASE);
      c = button_state();
      if(c == 1){
        LED_toggle(GREEN);
        schedeuler(S, c);
        
       //hile(!(button_state() == 2));
      }else if(c==2){
        LED_toggle(GREEN);
        UART0_Init();
        //IntMasterDisable();
       // IntMasterEnable();
       // can_enable(); 
        schedeuler(S, 2);
       // CANDisable(CAN0_BASE);
      }else{
       can_rec();
        
      }

int switch1 = 0;
      switch1 = button_state();
      switch(switch1){
      case 0:
        //idle
        LED_toggle(BLUE);
        break;
      
      case 1:
        LED_toggle(GREEN);
        schedeuler(S, switch1);
        break;
        
      //case 2:
        //LED_toggle(GREEN);
        //schedeuler(S ,switch1);
        //break;
        
      }
*/