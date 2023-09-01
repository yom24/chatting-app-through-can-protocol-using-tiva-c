#include "HAL_ring_buff.h"
#include "HAL_can.h"
#include <string.h>
#include "MCAL_uart.h"
#include "MCAL_gpio.h"

uint8_t string[] = "youssef omar";
uint8_t zeros[200] = {' '};
//uint8_t string1[30] = "mohamed abdel samie";
//uint8_t string[length];
uint8_t buf[8];
uint8_t saved[8];
tRingBufObject buffer;

int A = 0;

float h = 0;


_schd S[2];
//int state = 0;

//int flag = 1;

void create_task(_schd s[], int number){
  
  s[number-1].number = number;
  if(number == 1){
  s[number-1].opt = UART0_getString;
  }else{
    s[number-1].opt = systick;
  }
}

void schedeuler(_schd s[], int state){
     
     if(s[0].number == state){
        s[0].opt();
      }else if(s[1].number == state){
        s[1].opt();
      }
     //state = 0;
}

void systick(){
  
  SysTickEnable();
    SysTickPeriodSet(16000000);
    SysTickIntEnable();
    SysTickIntRegister(CAN_send_ISR);
    IntMasterEnable();
  
}

void CAN_send_ISR(){
  
  static int t = 0;
  RingBufInit(&buffer, buf, 8);
  //memset(string,0,sizeof(uint8_t));
  
  /*for(int i =0;i<sizeof(string)/8;i++){
    for(int j=0;j<8;j++){
      
    uint8_t temp[j]=string[j];
      
  RingBufWrite(&buffer, string, 8);
}
}*/
 // RingBufRead(&buffer, saved, 8);
 // RingBufAdvanceRead(&buffer, 4);
 // RingBufRead(&buffer, saved, 8);
 
  
  //bool k = 0;
  uint8_t *p = string1;
  RingBufWrite(&buffer, p+A, 8);
 RingBufRead(&buffer, saved, 8);
 A+=8;
 can_send(saved);
 //}while(strlen(saved)/8 != '\0');
 t++;
 if(strlen(string1)/8 < t){
   SysTickDisable();
   for(int i =0; i<sizeof(string1); i++){
   string1[i] = '\0';
   }
   //state = 0;
   t=0;
   A=0;
 }

}

void save_ring(){
  
  RingBufInit(&buffer, buf, 8);
  //memset(string,0,sizeof(uint8_t));
  
  /*for(int i =0;i<sizeof(string)/8;i++){
    for(int j=0;j<8;j++){
      
    uint8_t temp[j]=string[j];
      
  RingBufWrite(&buffer, string, 8);
}
}*/
 // RingBufRead(&buffer, saved, 8);
 // RingBufAdvanceRead(&buffer, 4);
 // RingBufRead(&buffer, saved, 8);
 
  int x = 0;
  //bool k = 0;
  do{
  uint8_t *p = string;
  RingBufWrite(&buffer, p+x, 8);
 RingBufRead(&buffer, saved, 8);
 x+=8;
 can_send(saved);
 }while(strlen(saved)/8 != '\0');
}
