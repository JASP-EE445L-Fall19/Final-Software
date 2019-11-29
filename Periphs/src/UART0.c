//  UART0.c
//      Source Code for UART0 drivers for Lab 11 in EE 445L
//      General Purpose UART0
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



#include "UART0.h"
#include "TM4C1290NCPDT.h"

// standard ASCII symbols
#define CR   0x0D
#define LF   0x0A
#define BS   0x08
#define ESC  0x1B
#define SP   0x20
#define DEL  0x7F


// This is required to use printf
struct __FILE{
	int file;
};

// Global variables
FILE __stdout;
FILE __stdin;





/**     UART0_Init Function
 *  @brief      Initializes UART0. Used to retarget printf and scanf
 *  @details    Pins Used:
 *                  PA0 - UART0 Rx
 *                  PA1 - UART0 Tx
 *  @param[in]  baudrate    Baudrate for UART0 
 * 
 */
void UART0_Init(int baudrate) {
	SYSCTL->RCGCUART |= 0x01;				// Port A, UART0
	SYSCTL->RCGCGPIO |= 0x01;
	while ((SYSCTL->PRGPIO & 0x01) == 0) {};
	
	GPIOA_AHB->DIR |= 0x02;
	GPIOA_AHB->DIR &= ~0x01;
	GPIOA_AHB->AFSEL |= 0x03;
	GPIOA_AHB->DEN |= 0x03;
	GPIOA_AHB->PCTL = (GPIOA_AHB->PCTL&0xFFFFFF00) + 0x00000011;
	GPIOA_AHB->AMSEL &= ~0x03;
		
	UART0->CTL &= ~0x01;		// Disable UART by clearing UARTEN in UARTCTL
	UART0->IBRD = 120000000 / (16 * baudrate);		// Write IBRD
	UART0->FBRD = (120000000 % (16 * baudrate)) * 64 + ((16*115200)/2) / (16 * baudrate);		// Write FBRD
	UART0->LCRH = (UART0->LCRH&0xFFFFFF8F) | (0x07<<4);		// Write parameters to UARTLCRH
	// Confgure UART clk src UARTCC - Already reset to 0 on power up
	UART0->CTL |= 0x01;			// Enable UART by setting UARTEN in UARTCTL
				
}



/**     UART0_InChar Function
 *  @brief      Waits for serial port to receive message.
 *  @return  		8 bit ASCII message received from UART0 serial port 
 * 
 */
char UART0_InChar(void) {
	while((UART0->FR&0x10) != 0) {};
	return (char)(UART0->DR&0xFF);
}



/**     UART0_InString Function
 *  @brief      Scans multiple 8-bit frames from UART0 serial port
 *  @param[in]  *bufPt    ASCII message formatted in NULL terminated array 
 *	@param[in]	size			Size of ASCII buffer
 * 
 */
void UART0_InString(char *bufPt, uint16_t size) {
	int length=0;
	char character = UART0_InChar();
  while(character != CR){
    if(character == BS){
      if(length){
        bufPt--;
        length--;
        UART0_OutChar(BS);
      }
    }
    else if(length < size){
      *bufPt = character;
      bufPt++;
      length++;
      UART0_OutChar(character);
    }
    character = UART0_InChar();
  }
  *bufPt = 0;		
	
}




/**     UART0_OutChar Function
 *  @brief      8 bits out from UART0 serial port
 *  @param[in]  data		8 bit (ASCII) message to transmit 
 * 
 */
void UART0_OutChar(char data) {
	while((UART0->FR&0x20) != 0) {};
	UART0->DR = data;	
}



/**     UART0_OutString Function
 *  @brief      Sends multiple 8-bit frames from UART0 serial port
 *  @param[in]  *pt    ASCII message formatted in NULL terminated array 
 * 
 */
void UART0_OutString(char *pt) {
	while(*pt) {			// while not NULL
		UART0_OutChar(*pt);
		pt++;	
	}
}



/**     fputc Function
 *  @brief      Used to retarget printf wuth UART0
 *  @param[in]  ch    ASCII character to send 
 *	@param[in]	*f		FILE type for STD IO 
 *
 */
int fputc(int ch, FILE *f){
  if((ch == 10) || (ch == 13) || (ch == 27)){
    UART0_OutChar(13);
    UART0_OutChar(10);
    return 1;
  }
  UART0_OutChar(ch);
  return 1;
}



/**     fgetc Function
 *  @brief      Used to retarget scanf wuth UART0 
 *	@param[in]	*f		FILE type for STD IO 
 *
 */
int fgetc (FILE *f){
  char ch = UART0_InChar();  // receive from keyboard
  UART0_OutChar(ch);            // echo
  return (int)ch;
}

