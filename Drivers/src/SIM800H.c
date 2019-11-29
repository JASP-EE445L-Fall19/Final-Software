//  SIM800H.c
//      Source code for GSM Module drivers for Lab 11 in EE 445L
//      GSM Module uses the SIM800H chip from SimCom
//
//  Hardware:
//      
//
//  Created: 11/04/2019 - 06:35PM
//  Last Updated: 10/22/2019 - 06:35PM
//
//  Developers:
//      Joshua Iwe
//      Paulina Vasquez-Pena
//      Arjun Ramesh
//      Sihyung Woo
//


#include "SIM800H.h"



void TerminalMenu(void);


/**     SIM800H_Init Function
 *  @brief      Initializes UART2 and GSM module
 *  @details    Pins Used:
 *                  PD0 - Rst
 *                  PD1 - PS
 *                  PD2 - Key
 *                  PD3 - RI
 *                  PD4 - UART2_Rx
 *                  PD5 - UART2_Tx
 *                  PD7 - NS
 *  @note       Might have the baudrate as the input parameter
 *                  but the module automatically tunes baudrate when
 *                  you send first commands to it.
 */
void SIM800H_Init(void) {
	UART2_Init(115200);

	SYSCTL->RCGCGPIO |= 0x08;			// Port D
	while ((SYSCTL->PRGPIO & 0x08) == 0) {};
		
	GPIOD_AHB->DIR |= 0x01;				// RST pin
	GPIOD_AHB->DEN |= 0x01;
	
	// Reset Module (pull rst pin 100ms)
	GPIOD_AHB->DATA |= 0x01;
	SysTick_Wait10ms(1);		// Wait 10ms
	GPIOD_AHB->DATA &= ~0x01;
	SysTick_Wait10ms(10);		// Wait 100ms
	GPIOD_AHB->DATA |= 0x01;
		
	int16_t timeout = 7000;				// 7 seconds
	char read[5];
	while(timeout > 0) {
		UART2_OutString("AT");
		UART2_InString(read, 5);
		if(read[0] == 'O' && read[1] == 'K') {
			break;
		}
		SysTick_Wait10ms(50);			// Wait half a second;
		timeout -= 500;
	}
	
	if(timeout <= 0) {
		printf("SIM800H Timed-out.\r");
	}
		
		
	//TerminalMenu();							// Comment in or out for terminal menu

	
	
	
	
}



void TerminalMenu(void) {
	printf("[?] Print this menu\r");
	printf("[a] read the ADC 2.8V max (FONA800 & 808)\r");
	
}


