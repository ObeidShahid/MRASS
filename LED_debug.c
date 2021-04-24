/* * LED_debug.c
 *
 *  Created on: 10 Apr 2019
 *      Author: Abdirahman*/

// LEDS for debugging


#include <stdio.h>
#include "tm4c123gh6pm.h"
#include "LED_debug.h"
#include "PLL.h"

#define U1_hrd_buff              (*((volatile unsigned long *)0x40007020)) // PD3
#define U1_sftw_buff             (*((volatile unsigned long *)0x40007004)) // PD0

//#define U2_hrd_buff              (*((volatile unsigned long *)0x40024040)) // PE4
#define U2_sftw_buff             (*((volatile unsigned long *)0x40007008)) // PD1

//#define U3_hrd_buff              (*((volatile unsigned long *)0x40024080)) // PE5
#define U3_sftw_buff             (*((volatile unsigned long *)0x40007010)) // PD2



/*
#define red                      (*((volatile unsigned long *)0x40025008)) // PF1 red
#define blue                     (*((volatile unsigned long *)0x40025010)) // PF2 blue
#define green                    (*((volatile unsigned long *)0x40025020)) // PF3 green
*/


void LEDS_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000020;     // 1) F clock
  delay = SYSCTL_RCGC2_R;           // delay
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock PortF PF0
  GPIO_PORTF_CR_R |= 0x1F;           // allow changes to PF4-0
  GPIO_PORTF_AMSEL_R &= 0x00;        // 3) disable analog function
  GPIO_PORTF_PCTL_R &= 0x00000000;   // 4) GPIO clear bit PCTL
  GPIO_PORTF_DIR_R &= ~0x00;          // 5.1)
  GPIO_PORTF_DIR_R |= 0x0E;          // 5.2) PF3 output
  GPIO_PORTF_AFSEL_R &= 0x00;        // 6) no alternate function
  GPIO_PORTF_PUR_R |= 0x11;          // enable pullup resistors on PF4,PF0
  GPIO_PORTF_DEN_R |= 0x1F;          // 7) enable digital pins PF4-PF0


  SYSCTL_RCGC2_R |= 0x00000008;     // 1) D clock
  delay = SYSCTL_RCGC2_R;           // delay
  GPIO_PORTD_LOCK_R = 0x4C4F434B ;   // 2) unlock PortD PD0
  GPIO_PORTD_CR_R = 0xFF;           // allow changes to PD3-0
  GPIO_PORTD_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTD_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL
  GPIO_PORTD_DIR_R = 0xFF;          // 5) PD3,PD2,PD1,PD0 output
  GPIO_PORTD_AFSEL_R = 0x00;        // 6) no alternate function
  GPIO_PORTD_PUR_R = 0xFF;          // disable pullup resistors for outputs
  GPIO_PORTD_DEN_R = 0xFF;          // 7) enable digital pins PD3-PD0

}

void blue1_led_Disable(){
    U1_hrd_buff&=~0x08;
}
/*void blue2_led_Disable(){
    U2_hrd_buff&=~0x10; // pe4
}
void blue3_led_Disable(){
    U3_hrd_buff&=~0x20; // pe5
}*/
//Disable software debugging leds // white
void white1_led_Disable(){
    U1_sftw_buff&=~0x01;
}
void white2_led_Disable(){
    U2_sftw_buff&=~0x02;
}
void white3_led_Disable(){
    U3_sftw_buff&=~0x04;
}

//Enable hardware debugging leds // blue
void blue1_led_Enable(){
    U1_hrd_buff|=0x08;
}

/*void blue2_led_Enable(){
    U2_hrd_buff|=0x10; // pe4
}
void blue3_led_Enable(){
    U3_hrd_buff|=0x20; // pe5
}*/

//Enable software debugging leds // white
void white1_led_Enable(){
    U1_sftw_buff|=0x01;
}
void white2_led_Enable(){
    U2_sftw_buff|=0x02;
}
void white3_led_Enable(){
    U3_sftw_buff|=0x04;
}

void red_on(){
    GPIO_PORTF_DATA_R |= 0x02;

}

void white_on(){
    GPIO_PORTF_DATA_R |= 0x0E;

}
void white_off(){
    GPIO_PORTF_DATA_R &=~ 0x0E;

}

void red_off(){
    GPIO_PORTF_DATA_R &=~0x02;
}

void blue_on(){
    GPIO_PORTF_DATA_R |= 0x04;
}
void blue_off(){
    GPIO_PORTF_DATA_R &=~ 0x04;
}

void green_on(){
    GPIO_PORTF_DATA_R |= 0x08;
}
void green_off(){
    GPIO_PORTF_DATA_R &=~ 0x08;
}
