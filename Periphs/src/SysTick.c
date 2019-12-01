//  SysTick.c
//
//
//
//


#include "SysTick.h"



void SysTick_Init(uint32_t delay) {
	SysTick->CTRL = 0;
	SysTick->LOAD = delay - 1;
	SysTick->VAL = 0;
	SysTick->CTRL = 0x05;
}


void SysTick_Wait(uint32_t delay) {
  SysTick->LOAD = delay-1;  // number of counts to wait
  SysTick->VAL = 0;       // any value written to CURRENT clears
  while((SysTick->CTRL&0x00010000)==0){} // wait for count flag
}


void SysTick_Wait1ms(uint32_t ms) {
	for(int i = 0; i < ms; i++){
    SysTick_Wait(120000);  // wait 1ms (assumes 120 MHz clock)
  }
}


void SysTick_Wait10ms(uint32_t ms) {
	for(int i = 0; i < ms; i++) {
		SysTick_Wait(1200000);	// wait 10ms		
	}    
}


