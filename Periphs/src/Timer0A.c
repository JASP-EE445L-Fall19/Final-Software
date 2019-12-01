// Timer0A.c
//
//
//


#include "Timer0A.h"



/**     Timer0A_Init Function
 *  @brief      Initializes Timer0A. Enabled for periodic interrupts
 *  @param[in]  period			Period for timer to interrupt 
 * 
 */
void Timer0A_Init(int period) {
	SYSCTL->RCGCTIMER |= 0x01;
	while((SYSCTL->PRTIMER & 0x01) == 0) {}
	TIMER0->CTL &= ~0x01;					// Disable Timer0A during init
	TIMER0->CFG = 0x00;						// Set to 32-bit timer
	TIMER0->TAMR = 0x02;					// Periodic, count-down mode
	TIMER0->TAILR = period - 1;		// Set Reload Value
	TIMER0->TAPR = 0x00;					// No prescaler, based off system clock (120MHz => 8.33ns)
	TIMER0->ICR = 0x01;						// Clear interrupt flag for Timer0A
	TIMER0->IMR |= 0x01;					// ARM Timer0A interrupt
	NVIC_EnableIRQ(TIMER0A_IRQn);	// Enable Timer0A IRQ
	NVIC_SetPriority(TIMER0A_IRQn, 2);
	TIMER0->CTL |= 0x01;					// Enable Timer0A, done with initialization
			
}





void TIMER0A_Handler(void) {
	TIMER0->ICR = 0x01;
	
	lv_tick_inc(1);
	
	
}






