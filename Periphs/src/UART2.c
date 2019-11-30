// UART2.c
//      Source Code for UART2 drivers for Lab 11 in EE 445L
//      General Purpose UART2
//
//  Hardware:
//			PD4	-	U2Rx
//      PD5 -	U2Tx
//
//  Created: 11/28/2019 - 05:00PM
//  Last Updated: 11/28/2019 - 05:00PM
//
//  Developers:
//      Joshua Iwe
//      Paulina Vasquez-Pena
//      Arjun Ramesh
//      Sihyung Woo
//


#include "UART2.h"
#include "TM4C1290NCPDT.h"


// standard ASCII symbols
#define CR   0x0D
#define LF   0x0A
#define BS   0x08
#define ESC  0x1B
#define SP   0x20
#define DEL  0x7F



/**     UART2_Init Function
 *  @brief      Initializes UART2. Used for SIM800H (GSM)
 *  @details    Pins Used:
 *                  PD4 - UART2 Rx
 *                  PD5 - UART2 Tx
 *  @param[in]  baudrate    Baudrate for UART2 
 * 
 */
void UART2_Init(int baudrate) {
	SYSCTL->RCGCUART |= 0x04;				// Port D, UART2
	SYSCTL->RCGCGPIO |= 0x08;
	while ((SYSCTL->PRGPIO & 0x08) == 0) {};
		
	GPIOD_AHB->DIR |= 0x20;
	GPIOD_AHB->DIR &= ~0x10;
	GPIOD_AHB->AFSEL |= 0x30;
	GPIOD_AHB->DEN |= 0x30;
	GPIOD_AHB->PCTL = (GPIOD_AHB->PCTL&0xFF00FFFF) + 0x00110000;
	GPIOD_AHB->AMSEL &= ~0x30;
		
	UART2->CTL &= ~0x01;		// Disable UART by clearing UARTEN in UARTCTL
	UART2->IBRD = 120000000 / (16 * baudrate);		// Write IBRD
	UART2->FBRD = (120000000 % (16 * baudrate)) * 64 + ((16*115200)/2) / (16 * baudrate);		// Write FBRD
	UART2->LCRH = (UART2->LCRH&0xFFFFFF8F) | (0x07<<4);		// Write parameters to UARTLCRH
	// Confgure UART clk src UARTCC - Already reset to 0 on power up
	UART2->CTL |= 0x301;			// Enable UART by setting UARTEN in UARTCTL	
	UART2->IM |= 0x10;		//	Arm RxRIS
	UART2->IFLS = 0x10;		// Half full Rx trigger interurupt
	NVIC_EnableIRQ(UART2_IRQn);
	NVIC_SetPriority(UART2_IRQn, 1);
	UART2->ICR = 0x0;			// Clear flag
	
	Fifo_Init();
	
	SYSCTL->RCGCGPIO |= (1<<12);
	while ((SYSCTL->PRGPIO & (1<<12)) == 0) {};
	GPION->DIR |= 0x2;
	GPION->DEN |= 0x2;
	
}



/**     UART2_InChar Function
 *  @brief      Waits for serial port to receive message.
 *  @return  		8 bit ASCII message received from UART2 serial port 
 * 
 */
char UART2_InChar(void) {
	while((UART2->FR&0x10) != 0) {};
	return (char)(UART2->DR&0xFF);
}



/**     UART2_InString Function
 *  @brief      SGets multiple 8-bit frames from UART2 serial port
 *  @param[in]  *bufPt    ASCII message formatted in NULL terminated array 
 *	@param[in]	size			Size of ASCII buffer
 * 
 */
void UART2_InString(char *bufPt, uint16_t size) {
	int length=0;
	char character = UART2_InChar();
  while(character != CR){
    if(character == BS){
      if(length){
        bufPt--;
        length--;
        UART2_OutChar(BS);
      }
    }
    else if(length < size){
      *bufPt = character;
      bufPt++;
      length++;
      UART2_OutChar(character);
    }
    character = UART2_InChar();
  }
  *bufPt = 0;		

}



/**     UART2_OutChar Function
 *  @brief      8 bits out from UART2 serial port
 *  @param[in]  data		8 bit (ASCII) message to transmit 
 * 
 */
void UART2_OutChar(char data) {
	while((UART2->FR&0x20) != 0) {};
	UART2->DR = data;		
}



/**     UART2_OutString Function
 *  @brief      Sends multiple 8-bit frames from UART2 serial port
 *  @param[in]  *pt    ASCII message formatted in NULL terminated array 
 * 
 */
void UART2_OutString(char *pt) {
	while(*pt) {			// while not NULL
		UART2_OutChar(*pt);
		pt++;	
	}	
}




void UART2_Handler(void) {
	static char Data;
	while((UART2->FR & 0x10) == 0) {
		Data = (char)(UART2->DR & 0x0FF);	// Get Data
		Fifo_Put(Data);
	}
	
	GPION->DATA ^= 0x02;
	UART2->ICR |= 0x10;			// ACK
}



