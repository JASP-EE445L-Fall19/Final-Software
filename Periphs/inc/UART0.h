//  UART0.h
//      Header for UART drivers for Lab 11 in EE 445L
//      General Purpose UART
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



#ifndef __UART0_H
#define __UART0_H



#include <stdint.h>
#include <stdio.h>


/**     UART0_Init Function
 *  @brief      Initializes UART0. Used to retarget printf and scanf
 *  @details    Pins Used:
 *                  PA0 - UART0 Rx
 *                  PA1 - UART0 Tx
 *  @param[in]  baudrate    Baudrate for UART0 
 * 
 */
void UART0_Init(int baudrate);




/**     UART0_InChar Function
 *  @brief      Waits for serial port to receive message.
 *  @return  		8 bit ASCII message received from UART0 serial port 
 * 
 */
char UART0_InChar(void);



/**     UART0_InString Function
 *  @brief      Scans multiple 8-bit frames from UART0 serial port
 *  @param[in]  *bufPt    ASCII message formatted in NULL terminated array 
 *	@param[in]	size			Size of ASCII buffer
 * 
 */
void UART0_InString(char *bufPt, uint16_t size);



/**     UART0_OutChar Function
 *  @brief      8 bits out from UART0 serial port
 *  @param[in]  data		8 bit (ASCII) message to transmit 
 * 
 */
void UART0_OutChar(char data);



/**     UART0_OutString Function
 *  @brief      Sends multiple 8-bit frames from UART0 serial port
 *  @param[in]  *pt    ASCII message formatted in NULL terminated array 
 * 
 */
void UART0_OutString(char *pt);



/**     fputc Function
 *  @brief      Used to retarget printf wuth UART0
 *  @param[in]  ch    ASCII character to send 
 *	@param[in]	*f		FILE type for STD IO 
 *
 */
int fputc(int ch, FILE *f);



/**     fgetc Function
 *  @brief      Used to retarget scanf wuth UART0 
 *	@param[in]	*f		FILE type for STD IO 
 *
 */
int fgetc (FILE *f);




#endif

