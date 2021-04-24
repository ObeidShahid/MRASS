#include <stdio.h>
#include <string.h>
#include <math.h>
#include "TExaS.h"
#include "uart.h"
#include "PLL.h"
#include "UniversalFunctions.h"
#include "UniversalFunctions2.h"
#include "UniversalFunctions3.h"
//#include "updatedNmeaFunctions.h"
#include "calculations.h"
#include "vector.h"
#include "UART1.h"
#include "UARTInterrupt.h"
#include "LED_debug.h"
#include "VincentyDistance.h"
#include "heading.h"
#include "Rotation.h"
extern double p_ypr_1[3],p_ypr_2[3],p_ypr_3[3];
char data_uart1[1024],data_uart2[1024],data_uart3[1024];
int data_uart1_sz,data_uart2_sz,data_uart3_sz;
int uart1_done,uart2_done,uart3_done;
int i;
char splitarray1[100], splitarray2[100], splitarray3[100],splitarray21[100],splitarray22[100],splitarray23[100],splitarray31[100], splitarray32[100], splitarray33[100];
char *danno, *danno2, *danno3;

char nmeafromsystem4[85] = {"$GNGGA,164208.00,5348.50968,N,00133.44528,W,5,12,0.87,82.4,M,47.8,M,22.0,0000*7C"};
char nmeafromsystem5[85] = {"$GNGGA,164202.00,5348.50944,N,00133.44801,W,5,12,0.79,81.7,M,47.8,M,43.0,0000*78"};
char nmeafromsystem6[85] = {"$GNGGA,164208.00,5348.50946,N,00133.44792,W,5,12,0.94,81.9,M,47.8,M,49.0,0000*72"};

/* Issue when receivers first turn on , it transmits 10+ init sentences in bursts to each host device
 * Throwing off the UART Handlers either causing a framing error or break error in any of the UARTS
 * Needs 1-4 resets of the MCU on first turn on before code works as should
*/

/* Need to:
     * solve Above issue
     * create constant naming conventions for all classes
     * modularise the UART1 class to the following classes
           *   init_UART
           *   UART_data_handlers class*/

void Disable_leds(){

    white1_led_Disable();
    white2_led_Disable();
    white3_led_Disable();

    blue1_led_Disable();
  //  blue2_led_Disable();
  //  blue3_led_Disable();

}

void enable_interrupts(){
    UART1_EnableRXInterrupt();
    SysTick_Wait1ms(20);
    UART2_EnableRXInterrupt();
    SysTick_Wait1ms(20);
    UART3_EnableRXInterrupt();
    SysTick_Wait1ms(20);
}

void init_uarts(){
    UART1_Init();
    SysTick_Wait1ms(20);
    UART2_Init();
    SysTick_Wait1ms(20);
    UART3_Init();
    SysTick_Wait1ms(20);
/*    UART5_Init();
    SysTick_Wait1ms(20);*/
}

void clear_data_arrays(int data_sz){
    for (i=0; i <=data_sz; ++i){
        data_uart1[i]=0x00;
        data_uart2[i]=0x00;
        data_uart3[i]=0x00;
    }
}

void split_incoming_data(char datainpuit[]){
    danno = datainpuit;
    int x=0, y=0,z=0,arraycounter=0;
    while(*danno != '\n'){
        splitarray1[x] = datainpuit[arraycounter];
        x=x+1; arraycounter=arraycounter+1; danno++;
    }

    splitarray1[x] = '\0';

    danno++;
    arraycounter++;

    while(*danno != '\n'){
        splitarray2[y] = datainpuit[arraycounter];
        y=y+1; arraycounter=arraycounter+1; danno++;
    }

    splitarray2[y] = '\0';

    danno++;
    arraycounter++;

    while(*danno != '\n'){
        splitarray3[z] = datainpuit[arraycounter];
        z=z+1; arraycounter=arraycounter+1; danno++;
    }

    splitarray3[z] = '\0';

   // printf("\n\nUart 1 Split data stream 1 = ");
    for(i=0; i<sizeof(splitarray1); i++){
     //   printf("%c", splitarray1[i]);
    }

   // printf("\n\nUart 1 Split data stream 2 = ");
    for(i=0; i<sizeof(splitarray2); i++){
      //  printf("%c", splitarray2[i]);
    }

    //printf("\n\nUart 1 Split data stream 3 = ");
    for(i=0; i<sizeof(splitarray3); i++){
      //  printf("%c", splitarray3[i]);
    }
}

void split_incoming_data2(char datainpuit[]){
    danno2 = datainpuit;
    int x=0, y=0,z=0,arraycounter=0;
    while(*danno2 != '\n'){
        splitarray21[x] = datainpuit[arraycounter];
        x=x+1; arraycounter=arraycounter+1; danno2++;
    }

    splitarray21[x] = '\0';

    danno2++;
    arraycounter++;

    while(*danno2 != '\n'){
        splitarray22[y] = datainpuit[arraycounter];
        y=y+1; arraycounter=arraycounter+1; danno2++;
    }

    splitarray22[y] = '\0';

    danno2++;
    arraycounter++;

    while(*danno != '\n'){
        splitarray23[z] = datainpuit[arraycounter];
        z=z+1; arraycounter=arraycounter+1; danno2++;
    }

    splitarray23[z] = '\0';


   // printf("\n\nUart 2 Split data stream 1 = ");
    for(i=0; i<sizeof(splitarray21); i++){
    //    printf("%c", splitarray1[i]);
    }

   // printf("\n\nUart 2 Split data stream 2 = ");
    for(i=0; i<sizeof(splitarray22); i++){
     //   printf("%c", splitarray22[i]);
    }

   // printf("\n\nUart 2 Split data stream 3 = ");
    for(i=0; i<sizeof(splitarray23); i++){
     //   printf("%c", splitarray23[i]);
    }
}

void split_incoming_data3(char datainpuit[]){
    danno3 = datainpuit;
    int x=0, y=0,z=0,arraycounter=0;
    while(*danno3 != '\n'){
        splitarray31[x] = datainpuit[arraycounter];
        x=x+1; arraycounter=arraycounter+1; danno3++;
    }

    splitarray31[x] = '\0';


    danno3++;
    arraycounter++;

    while(*danno3 != '\n'){
        splitarray32[y] = datainpuit[arraycounter];
        y=y+1; arraycounter=arraycounter+1; danno3++;
    }

    splitarray32[y] = '\0';

    danno3++;
    arraycounter++;

    while(*danno3 != '\n'){
        splitarray33[z] = datainpuit[arraycounter];
        z=z+1; arraycounter=arraycounter+1; danno3++;
    }

    splitarray33[z] = '\0';


   // printf("\n\nUart 3 Split data stream 1 = ");
    for(i=0; i<sizeof(splitarray31); i++){
       // printf("%c", splitarray31[i]);
    }

   // printf("\n\nUart 3 Split data stream 2 = ");
    for(i=0; i<sizeof(splitarray32); i++){
      //  printf("%c", splitarray32[i]);
    }

   // printf("\n\nUart 3 Split data stream 3 = ");
    for(i=0; i<sizeof(splitarray33); i++){
       // printf("%c", splitarray33[i]);
    }
}

int main() {
    PLL_Init();      //Initialise the PLL
    SysTick_Init();  //Initialise the Systick
    UART_Init();     //Initialise UART
    printf("Program Start\n");
    LEDS_Init();
    red_on();
    CTS_Pins_init();
    //UART5_Init();

    data_uart1_sz = 0;
    data_uart2_sz = 0;
    data_uart3_sz = 0;

    uart1_done = 0;
    uart2_done = 0;
    uart3_done = 0;

    int is_init = 0;
    int cnt = 0;
    double i_lat2, i_lng2, n_lat2, n_lng2;

    init_uarts();
    inital_bootUp_UARTS_handler();

    clear_data_arrays(1024);

    enable_interrupts();


// Initialisation complete

     while (1) {

         //printf("\n\n");
              UART1_Enable();

              green_off();
              // MONITOR THE STATUS OF THE FLAGS USING LEDS
              // after enable uart handlers are excuted //!uart2_done || !uart3_done
              while (!uart1_done || !uart2_done || !uart3_done  ) {

                  InterruptWait();

                 if (uart1_done) {
                      UART2_Enable();
                  }
                  if (uart2_done) {
                       UART3_Enable();

                   }
                   /*printf("uart 1 done %d \n",uart1_done);
                   printf("uart 2 done %d \n",uart2_done);
                   printf("uart 3 done %d \n",uart3_done);
                   printf("\n");*/
              }

              red_off();
              green_on();

/*              //printf("\n\nUART1   %d", data_uart1_sz);
             // printf("\nUART1 array = ");
              for (i = 0; i < data_uart1_sz; ++i) {
                 //printf("%c", data_uart1[i]);
              }*/

              split_incoming_data(data_uart1);

              /*printf("\nsplitstream2[0] = %c", splitarray2[0]);
              printf("\nsplitstream2[1] = %c", splitarray2[1]);
              printf("\nsplitstream2[2] = %c", splitarray2[2]);
              printf("\nsplitstream2[3] = %c", splitarray2[3]);
              printf("\nsplitstream2[4] = %c", splitarray2[4]);
              printf("\nsplitstream2[5] = %c", splitarray2[5]);
              printf("\nsplitstream2[6] = %c", splitarray2[6]);*/

              findStream(splitarray1,100);
              findStream(splitarray2,100);
              findStream(splitarray3,100);
              cartesian1(latitudedegrees, longitudedegrees, altitudemeters);

              data_uart1_sz = 0;
              uart1_done = 0;

/*
              //printf("\nUART2   %d\n", data_uart2_sz);
              for (i = 0; i < data_uart2_sz; ++i) {
                //printf("%c", data_uart2[i]);
              }
*/

              split_incoming_data2(data_uart2);
              findStream2(splitarray21);
              findStream2(splitarray22);
              findStream2(splitarray23);
              cartesian2(latitudedegrees2, longitudedegrees2, altitudemeters2);

              data_uart2_sz = 0;
              uart2_done = 0;

/*              //printf("\nUART3   %d\n", data_uart3_sz);
              for (i = 0; i < data_uart3_sz; ++i) {
               //printf("%c", data_uart3[i]);
              }*/

              split_incoming_data3(data_uart3);
              findStream3(splitarray31);
              findStream3(splitarray32);
              findStream3(splitarray33);
              cartesian3(latitudedegrees3, longitudedegrees3, altitudemeters3);

              combinePositions(latitudedegrees, longitudedegrees, altitudemeters, latitudedegrees2, longitudedegrees2, altitudemeters2, latitudedegrees3, longitudedegrees3, altitudemeters3);

              data_uart3_sz = 0;
              uart3_done = 0;

              struct euler_angles ypr;

    /*          double initp1[3]={3773063.046210,-102559.576920,5124285.710345};
              double initp2[3]={3773062.159873,-102559.036603,5124284.518825};
              double initp3[3]={3773057.235531,-102565.756446,5124284.902608};*/

              float heading_angle;

              if (!is_init) {
                  set_init_ref_frame(p_ypr_1,p_ypr_2,p_ypr_3);
                 // set_init_ref_frame(initp1,initp2,initp3);
                  i_lat2 = latitudedegreescombined;
                  i_lng2 = longitudedegreescombined;
                  is_init = 1;
                  cnt=0;
              } else {
                   ypr = Rotation(p_ypr_1, p_ypr_2, p_ypr_3);

                   n_lat2 = latitudedegreescombined;
                   n_lng2 = longitudedegreescombined;
                   heading_angle = Bearing_calc(i_lat2,i_lng2,n_lat2,n_lng2);
                   i_lat2 = n_lat2;
                   i_lng2 = n_lng2;
                   //cnt++;
              }



              //encode
              encodeYPR(ypr.yaw, ypr.pitch, ypr.roll, heading_angle);
              encodeGNS(latitudedegreescombined,longitudedegreescombined,altitudemeterscombined);
              encodeGST();

          }
}

