#include "UART1.h"
#include <stdio.h>
#include "tm4c123gh6pm.h"
#include "UARTInterrupt.h"
#include "UniversalFunctions.h"
#include "LED_debug.h"
#include "PLL.h"
//Reciver 1
// U1Rx -  PC4
// U1Tx -  PC5

//Reciver 2
// U2Rx -  PD6
// U2Tx -  PD7

//Reciver 3
// U3Rx -  PC6
// U3Tx -  PC7

//Bluetooth Module
// U5Rx -  PE4
// U5Tx -  PE5

// CTS held high to put host device on pause

#define UART_RIS_OERIS          0x00000400  // UART Overrun Error Raw Interrupt
                                            // Status
#define UART_RIS_BERIS          0x00000200  // UART Break Error Raw Interrupt
                                            // Status

#define UART_RIS_FERIS          0x00000080  // UART Framing Error Raw Interrupt
                                            // Status

#define U1_CTS                   (*((volatile unsigned long *)0x40024008))// // PE1
#define U2_CTS                   (*((volatile unsigned long *)0x40024010)) // PE2
#define U3_CTS                   (*((volatile unsigned long *)0x40024020))  // PE3

extern char data_uart1[1024],data_uart2[1024],data_uart3[1024];
extern int data_uart1_sz,data_uart2_sz,data_uart3_sz;
extern int uart1_done,uart2_done,uart3_done;


//------------UART_Init------------
// Wait for new serial port input
// Initialize the UART for 19200 baud rate (assuming 80 MHz UART clock),
// 8 bit word length, no parity bits, one stop bit, FIFOs enabled
// Input: none
// Output: none



void UART1_Init(void){
  SYSCTL_RCGC1_R |= 0x00000002;  // activate UART 1
  SYSCTL_RCGC2_R |= 0x00000004;  // activate port C
  UART1_CTL_R &= ~0x00000001;    // disable UART
  UART1_IBRD_R = 260;     // IBRD = int(80,000,000/(16*115200)) = int(520.8333)
  UART1_FBRD_R = 26;     // FBRD = round(0.40278 * 64) = 26
  UART1_LCRH_R = 0x00000070;  // 8 bit, no parity bits, one stop, FIFOs
  UART1_IFLS_R &= ~0x3F;                            // Clear TX and RX interrupt FIFO level fields
  UART1_IFLS_R += UART_IFLS_RX6_8 ;                 // RX FIFO interrupt threshold >= 3/4 full
  UART1_IM_R  |= UART_IM_RXIM | UART_IM_RTIM; // setting the interrupt Rx timeout
  UART1_CTL_R |= 0x00000001;     // enable UART
  GPIO_PORTC_AFSEL_R |= 0xF0;    // enable alt funct on PC5-4
  GPIO_PORTC_DEN_R |= 0xF0;      // configure PC5-4 as UART1
  GPIO_PORTC_PCTL_R = (GPIO_PORTC_PCTL_R&0xFF00FFFF)+0x00220000;
  GPIO_PORTC_AMSEL_R &= ~0xF0;   // disable analog on PC5-4
}

void UART2_Init(void){
  SYSCTL_RCGC1_R |= 0x00000004;  // activate UART2
  SYSCTL_RCGC2_R |= 0x00000008;  // activate port D
  UART2_CTL_R &= ~0x00000001;    // disable UART
  UART2_IBRD_R = 260;     // IBRD = int(80,000,000/(16*19200)) = int(260)
  UART2_FBRD_R = 26;     // FBRD = round(0.40278 * 64) = 26
  UART2_LCRH_R = 0x00000070;  // 8 bit, no parity bits, one stop, FIFOs
  UART2_IFLS_R &= ~0x3F;                            // Clear TX and RX interrupt FIFO level fields
  UART2_IFLS_R += UART_IFLS_RX6_8 ;                 // RX FIFO interrupt threshold >= 3/4 full
  UART2_IM_R  |= UART_IM_RXIM | UART_IM_RTIM; // setting the interrupt Rx timeout
  UART2_CTL_R |= 0x00000001;     // enable UART
  GPIO_PORTD_AFSEL_R |= 0xC0;    // enable alt funct on PD6-7
  GPIO_PORTD_DEN_R |= 0xC0;      // configure PD6-7 as UART1
  GPIO_PORTD_PCTL_R = (GPIO_PORTD_PCTL_R&0x00FFFFFF)+0x11000000;
  GPIO_PORTD_AMSEL_R &= ~0xC0;   // disable analog on PD6-7
}

void UART3_Init(void){
  SYSCTL_RCGCUART_R |= 0x00000008;  // activate UART3
  SYSCTL_RCGC2_R |= 0x00000004;  // activate port C
  UART3_CTL_R &= ~0x00000001;    // disable UART
  UART3_IBRD_R = 260;     // IBRD = int(80,000,000/(16*9600)) = int(520.8333)
  UART3_FBRD_R = 26;     // FBRD = round(0.40278 * 64) = 26
  UART3_LCRH_R = 0x00000070;  // 8 bit, no parity bits, one stop, FIFOs
  UART3_IFLS_R &= ~0x3F;                            // Clear TX and RX interrupt FIFO level fields
  UART3_IFLS_R += UART_IFLS_RX6_8 ;                 // RX FIFO interrupt threshold >= 3/4 full
  UART3_IM_R  |= UART_IM_RXIM | UART_IM_RTIM; // setting the interrupt Rx timeout
  UART3_CTL_R |= 0x00000001;     // enable UART
  GPIO_PORTC_AFSEL_R |= 0xC0;    // enable alt funct on PC5-4
  GPIO_PORTC_DEN_R |= 0xC0;      // configure PC5-4 as UART1
  GPIO_PORTC_PCTL_R = (GPIO_PORTC_PCTL_R&0x00FFFFFF)+0x11000000;
  GPIO_PORTC_AMSEL_R &= ~0xC0;   // disable analog on PC5-4
}

void UART5_Init(void){
  SYSCTL_RCGCUART_R |= 0x00000020;  // activate UART5
  SYSCTL_RCGC2_R |= 0x00000010;  // activate port E
  UART5_CTL_R &= ~0x00000001;    // disable UART
  UART5_IBRD_R = 260;     // IBRD = int(80,000,000/(16*19200)) = int(260)
  UART5_FBRD_R = 26;     // FBRD = round(0.40278 * 64) = 26
  UART5_LCRH_R = 0x00000070;  // 8 bit, no parity bits, one stop, FIFOs
  UART5_CTL_R |= 0x00000001;     // enable UART
  GPIO_PORTE_AFSEL_R |= 0x30;    // enable alt funct on PE4-5
  GPIO_PORTE_DEN_R |= 0x30;      // configure PE4-5 as UART5
  GPIO_PORTE_PCTL_R = (GPIO_PORTE_PCTL_R&0xFF00FFFF)+0x00110000;
  GPIO_PORTE_AMSEL_R &= ~0x30;   // disable analog on PE4-5
}

void CTS_Pins_init() {
    // initialise CTS pins and debugging lEDS
  volatile unsigned long delay;

  SYSCTL_RCGC2_R |= 0x00000010;     // 1) E clock
  delay = SYSCTL_RCGC2_R;           // delay
  GPIO_PORTE_CR_R = 0x3E;           // allow changes to PE3-1
  GPIO_PORTE_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTE_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL
  GPIO_PORTE_DIR_R |= 0x3E;          // 5) PE3,PE2,PE1 output
  GPIO_PORTE_AFSEL_R = 0x00;        // 6) no alternate function
  GPIO_PORTE_PUR_R = 0x3E;          // enable pullup resistors on PE3,PE2,PE1
  GPIO_PORTE_DEN_R = 0x3E;          // 7) enable digital pins PE3-PE0
}
//------------UART_InChar------------
// Wait for new serial port input
// Input: none
// Output: ASCII code for key typed


unsigned char UART1_InChar(void){
    while((UART1_FR_R&UART_FR_RXFE) != 0);   // wait until RXFE is 0
  return((unsigned char)(UART1_DR_R&0xFF));
}


unsigned char UART2_InChar(void){
    while((UART2_FR_R&UART_FR_RXFE) != 0);      // wait until RXFE is 0
  return((unsigned char)(UART2_DR_R&0xFF));
}


unsigned char UART3_InChar(void){
    while((UART3_FR_R&UART_FR_RXFE) != 0);     // wait until RXFE is 0
    //printf("uart3 val = %c",t);
  return((unsigned char)(UART3_DR_R&0xFF));
}


unsigned char UART5_InChar(void){
    while((UART5_FR_R&UART_FR_RXFE) != 0);      // wait until RXFE is 0
  return((unsigned char)(UART5_DR_R&0xFF));
}



//------------UART_OutChar------------
// Output 8-bit to serial port
// Input: letter is an 8-bit ASCII character to be transferred
// Output: none


void UART5_OutChar(unsigned char data){
    while((UART5_FR_R&UART_FR_TXFF)){}    // wait until TXFF is 0
  UART5_DR_R = data;
}
void UART1_OutChar(unsigned char data){
    while((UART1_FR_R&UART_FR_TXFF)){}    // wait until TXFF is 0
  UART1_DR_R = data;
}
// Immediately return input or 0 if no input

unsigned char UART5_InCharNonBlocking(void){
    return ((UART5_FR_R & UART_FR_RXFE) == 0)? (unsigned char)(UART5_DR_R & 0xFF): 0;
}
unsigned char UART1_InCharNonBlocking(void){
    return ((UART1_FR_R & UART_FR_RXFE) == 0)? (unsigned char)(UART1_DR_R & 0xFF): 0;
}
unsigned char UART2_InCharNonBlocking(void){
    return ((UART2_FR_R & UART_FR_RXFE) == 0)? (unsigned char)(UART2_DR_R & 0xFF): 0;
}
unsigned char UART3_InCharNonBlocking(void){
    return ((UART3_FR_R & UART_FR_RXFE) == 0)? (unsigned char)(UART3_DR_R & 0xFF): 0;
}

void Bluetooth_Out_String(char *string) {
  while(*string) { //The pointer is assigned to the input. If there is a character it would return 1, so would run for as long the string is
       UART5_OutChar(*string);//output via uart to bluetooth module so that its transmitted to the app
        string++; //The pointer increments to the next character 
  }
}

//
// -----------UART_Handler-----------

/*void check_uart_erros(){

    if ((UART1_RIS_R & UART_RIS_OERIS) || (UART1_RIS_R & UART_RIS_BERIS) || (UART1_RIS_R &  UART_RIS_FERIS)){
        white_on();


            UART1_Init();
            SysTick_Wait1ms(100);
            UART1_EnableRXInterrupt();
            SysTick_Wait1ms(100);

    }

    if ((UART2_RIS_R & UART_RIS_OERIS) || UART2_RIS_R & UART_RIS_BERIS || UART2_RIS_R &  UART_RIS_FERIS ){
        blue_on();
             UART2_Init();
            SysTick_Wait1ms(100);
            UART2_EnableRXInterrupt();
            SysTick_Wait1ms(100);

    }

    if (UART3_RIS_R & UART_RIS_OERIS || UART3_RIS_R & UART_RIS_BERIS || UART3_RIS_R &  UART_RIS_FERIS ){
                red_on();

                UART3_Init();
                SysTick_Wait1ms(100);
                UART3_EnableRXInterrupt();
                SysTick_Wait1ms(100);


    }

    red_off();
    blue_off();
    white_off();
}*/



void inital_bootUp_UARTS_handler(){
 // Enable one UART, receive first
   int cnt = 0;
   UART1_Enable();

   while (cnt != 8) {
       if (((UART1_FR_R & UART_FR_RXFE) == 0)) {
           char c = UART1_DR_R & 0xFF;
           if (c == '\n') {
               cnt++;
           }
       }
   }

   UART1_Disable();

   UART2_Enable();
   cnt= 0 ;

   // high second rst get all character
   while (cnt != 8) {
       if (((UART2_FR_R & UART_FR_RXFE) == 0)) {
           char c = UART2_DR_R & 0xFF;
           if (c == '\n') {
               cnt++;
           }
       }
   }

   UART2_Disable();

   UART3_Enable();
   cnt= 0 ;

   // third second rst get all character
   while (cnt != 8) {
       if (((UART3_FR_R & UART_FR_RXFE) == 0)) {
           char c = UART3_DR_R & 0xFF;
           if (c == '\n') {
               cnt++;
           }
       }
   }

   UART3_Disable();

}
void UART1_Handler(void) {
    static int cnt1;

    if (UART1_RIS_R & UART_RIS_RTRIS  ) {// if its time out - Theres data in FIFO but not half full , no data received over 32 bit  period
        UART1_ICR_R = UART_ICR_RTIC;      // acknowledge interrupt
        blue1_led_Enable();
        while (((UART1_FR_R & UART_FR_RXFE) == 0) ) { // while data is FIFO && data array not full
            char c = UART1_DR_R & 0xFF;
            while(data_uart1_sz==0 && c!='$'){
                  c = UART1_DR_R & 0xFF;
             }
            cnt1 += (c == '\n')? 1: 0;

            if (data_uart1_sz >= 1024) {
                data_uart1_sz =  0;
            }

            data_uart1[data_uart1_sz] = c;
            data_uart1_sz++;


            if (cnt1==3 || data_uart1_sz >= 1024) {
                uart1_done = 1;
                cnt1 = 0;
                UART1_Disable();

                blue1_led_Disable();
                break;
            }
        }


    }

    if (UART1_RIS_R & UART_RIS_RXRIS ) { // If UART Rx FIFO becomes 3/4 full
        UART1_ICR_R = UART_ICR_RXIC;      // acknowledge interrupt to clear it
        white1_led_Enable();
        while ((UART1_FR_R & UART_FR_RXFE) == 0 ) {
            char c = UART1_DR_R & 0xFF;
          while(data_uart1_sz==0 && c!='$'){
                  c = UART1_DR_R & 0xFF;
             }
            cnt1 += (c == '\n')? 1: 0;

            if (c=='\n'){
                cnt1+=1;
            }

            if (data_uart1_sz >= 1024) {
                data_uart1_sz =0;
            }
            data_uart1[data_uart1_sz] = c;
            data_uart1_sz++;
                                                                            //
            if (cnt1==3 || data_uart1_sz >= 1024) {
                uart1_done = 1;
                cnt1 = 0;

                UART1_Disable();
                white1_led_Disable();
                break;
            }
       }
    }
}

void UART2_Handler(void) {
    static int cnt2;
    if (UART2_RIS_R & UART_RIS_RTRIS) {// if its time out - Theres data in FIFO but not half full , no data received over 32 bit  period
        UART2_ICR_R = UART_ICR_RTIC;      // acknowledge interrupt
        //blue2_led_Enable();
        while (((UART2_FR_R & UART_FR_RXFE) == 0) ) {
           char c = UART2_DR_R & 0xFF;

           while(data_uart2_sz==0 && c!='$'){
                  c = UART2_DR_R & 0xFF;
             }

           cnt2 += (c == '\n')? 1: 0;

           if (data_uart2_sz >= 1024) {
               data_uart2_sz = 0;
           }

            data_uart2[data_uart2_sz] = c;
            data_uart2_sz++;                                            //
            if (cnt2==3 || data_uart2_sz >= 1024) {
                uart2_done = 1;
                cnt2 = 0;

                UART2_Disable();
                //blue2_led_Disable();
                break;
            }
       }
    }

    if (UART2_RIS_R & UART_RIS_RXRIS) {// If UART Rx FIFO becomes 3/4 full
        UART2_ICR_R = UART_ICR_RXIC;      // acknowledge interrupt
        white2_led_Enable();
        while ((UART2_FR_R & UART_FR_RXFE) == 0 ) {
            char c = UART2_DR_R & 0xFF;
            while(data_uart2_sz==0 && c!='$'){
                  c = UART2_DR_R & 0xFF;
             }
            cnt2 += (c == '\n')? 1: 0;

            if (data_uart2_sz >= 1024) {
                data_uart2_sz = 0;
            }

            data_uart2[data_uart2_sz] = c;
            data_uart2_sz++;

            if (cnt2==3 || data_uart2_sz >= 1024) {
                uart2_done = 1;
                cnt2 = 0;

                UART2_Disable();
                white2_led_Disable();
                break;
            }
        }
    }
}
void UART3_Handler(void) {
    //check_uart_erros();
    static int cnt3;
    if (UART3_RIS_R & UART_RIS_RTRIS) {// if its time out - Theres data in FIFO but not half full , no data received over 32 bit  period
            UART3_ICR_R = UART_ICR_RTIC;      // acknowledge interrupt
           // blue3_led_Enable();
            while (((UART3_FR_R & UART_FR_RXFE) == 0)) { // while data remains in Rx FIFO
                char c = UART3_DR_R & 0xFF;

                // first check to make sure first character in array is correct

                while(data_uart3_sz==0 && c!='$'){ // while in first component
                       c = UART3_DR_R & 0xFF;
                }

                cnt3 += (c == '\n')? 1: 0;

                if (data_uart3_sz >= 1024) {
                    data_uart3_sz = 0;
                }
                data_uart3[data_uart3_sz] = c;
                data_uart3_sz++;

                if (cnt3==3 || data_uart3_sz >= 1024) {
                    uart3_done = 1;
                    cnt3 = 0;

                    UART3_Disable();
                   // blue3_led_Disable();
                   break;
                }
            }
        }

        if (UART3_RIS_R & UART_RIS_RXRIS) {//If UART Rx FIFO becomes 3/4 full
            UART3_ICR_R = UART_ICR_RXIC;      // acknowledge interrupt
            white3_led_Enable();
            while ((UART3_FR_R & UART_FR_RXFE) == 0 ) {
                char c = UART3_DR_R & 0xFF;
                while(data_uart3_sz==0 && c!='$'){
                       c = UART3_DR_R & 0xFF;
                }

                cnt3 += (c == '\n')? 1: 0;

                if (data_uart3_sz >= 1024) {
                    data_uart3_sz = 0;
                }
                data_uart3[data_uart3_sz] = c;
                data_uart3_sz++;
                if (cnt3==3 || data_uart3_sz >= 1024) {
                    uart3_done = 1;
                    cnt3 = 0;

                    UART3_Disable();
                    white3_led_Disable();
                    break;
                }
            }
        }
}

//--------UART_EnableRXInterrupt--------
// - enables uart rx interrupt
// Inputs: none
// Outputs: none
void UART1_EnableRXInterrupt(void){
  DisableInterrupts();
  NVIC_EN0_R = 1<<6; // Enable (UART1) interrupt 6
  NVIC_PRI1_R &= 0xFF1FFFFF; // setting priority to 0
  EnableInterrupts();
}
void UART2_EnableRXInterrupt(void){
  DisableInterrupts();
  NVIC_EN1_R = 1<<1; // Enable (UART2)interrupt 33
  NVIC_PRI8_R &= 0xFFFF1FFF; // setting priority to 0
  EnableInterrupts();
}
void UART3_EnableRXInterrupt(void){
  DisableInterrupts();
  NVIC_EN1_R = 1<<27; // Enable (UART3 )interrupt 59
  NVIC_PRI14_R &= 0x1FFFFFFF; // setting priority to 0
  EnableInterrupts();
}
//--------UART_DisableRXInterrupt--------
// - disables uart rx interrupt
// Inputs: none
// Outputs: none
void UART1_DisableRXInterrupt(void){
  NVIC_DIS0_R = 1<<6; // interrupt 6
}

void UART2_DisableRXInterrupt(void){
  NVIC_DIS0_R = 1<<1; // interrupt 33
}
void UART3_DisableRXInterrupt(void){
  NVIC_DIS0_R = 1<<27; // interrupt 59
}
// Enable UARTS using GPIO by setting host device CTS to low

void UART1_Enable(){
    U1_CTS&=~0x02;
}
void UART2_Enable(){
    U2_CTS&=~0x04;
}
void UART3_Enable(){
    U3_CTS&=~0x08;
}


// Disable UARTS using GPIO by setting host device CTS to high
void UART1_Disable(){
    U1_CTS|=0x02;
}
void UART2_Disable(){
    U2_CTS|=0x04;
}
void UART3_Disable(){
    U3_CTS|=0x08;

}
