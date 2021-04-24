#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "vector.h"
#include "TExaS.h"

#define RCGC_EEPROM           (*((volatile unsigned long *)0x400FE658))
#define EEPROM_BLOCK          (*((volatile unsigned long *)0x400AF004))
#define EEPROM_OFFSET         (*((volatile unsigned long *)0x400AF008))
#define EEPROM_RDWR           (*((volatile unsigned long *)0x400AF010))
#define EEPROM_DONE           (*((volatile unsigned long *)0x400AF018))
#define EEPROM_ESUPP          (*((volatile unsigned long *)0x400AF01C))
#define SREEPROM                (*((volatile unsigned long *)0x400FE558))
#define SYSCTL_RCGC2_R        (*((volatile unsigned long *)0x400FE108))

unsigned long eedone, eesupp, eeread;

struct vector {
	char *data;
	size_t size, capacity;
};

typedef struct vector vector;

vector *init_vector() {
	vector *vec = (vector *)malloc(sizeof(vector));
	vec->size = 0;
	vec->capacity = 4;
	vec->data = (char *)malloc(vec->capacity);
	
	return vec;
}

void add_char(vector *vec, char c) {
	if (vec->size == vec->capacity) {
		vec->capacity *= 2;
		vec->data = (char *)realloc(vec->data, vec->capacity);
	}
	vec->data[vec->size++] = c;
}

char *get_str(vector *vec) {
	return vec->data;
}

void clear_vector(vector *vec) {
    memset(vec->data, 0, vec->capacity);
    vec->size = 0;
}

void free_vector(vector *vec) {
	free(vec->data);
	free(vec);
}

void EEPROM_Init(void){unsigned long delay;
    RCGC_EEPROM |= 0x1;                 //EEPROM Clock Set
    delay=SYSCTL_RCGC2_R;               //delay System Clock 3 times
    SysTick_Wait_1us(25000);        //25ms Delay
    EEPROM_Done_Check();                //Check EEPROM done flag
    eesupp = EEPROM_ESUPP;          //save eesupp value
    if (eesupp != 0){                       //Check eesupp value if is not 0
    EEPROM_ESUPP = 0x1;                 //Reset flash
    }
    SREEPROM = 0x1;                         //reset eeprom module
    SysTick_Wait_1us(20000);        //delay 20ms
    SREEPROM = 0x0;                         //clear bit to reset module
    SysTick_Wait_1us(25000);        //delay 25ms
    EEPROM_Done_Check();                //Check EEPROM done flag
    eesupp = EEPROM_ESUPP;          //Save eesupp value
        if (eesupp != 0){                   //Check eesupp
    EEPROM_ESUPP = 0x1;                 //Reset Flash
    SysTick_Wait_1us(20000);
    EEPROM_BLOCK = 0x01;                //Set Block to second block
    EEPROM_OFFSET = 0x01;               //Set Offset to second word
    }
}


void EEPROM_Done_Check(void){
            eedone = EEPROM_DONE;   //Save eedone value
    while(eedone != 0)          //check eedone value until it is 0
    {
        printf("\nChecking EEPROM DONE STATUS\n");
        eedone = EEPROM_DONE;   //re-read eedone value
    }
}

unsigned long EEPROM_READ(void) {unsigned long readEEPROM;
    readEEPROM = EEPROM_RDWR;
    printf("\nEEPROM = %lu\n",readEEPROM);
    return readEEPROM;

}

void EEPROM_WRITE(unsigned long write){
    EEPROM_RDWR = write;
    printf("\n Wrote %lu to EEPROM\n",write);
    EEPROM_Done_Check();
    if (EEPROM_RDWR == write){
        printf("EEPROM SUCESSFULLY WRITTEN");
    }
}
