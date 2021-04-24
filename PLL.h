#ifndef PLL_h
#define PLL_h

void SysTick_Init(void);
void PLL_Init(void);
void SysTick_Wait(unsigned long delay);
void SysTick_Wait10ms(unsigned long delay);
void SysTick_WaitUs(unsigned long delay);
void SysTick_Wait1ms(unsigned long delay);
void SysTick_Wait01ms(unsigned long delay);
void SysTick_Wait50ns(unsigned long delay);
#endif
