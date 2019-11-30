// UART2.h
//      Source Code for UART2 drivers for Lab 11 in EE 445L
//      General Purpose UART2
//
//  Hardware:
//			PD4	-	U2Rx
//          PD5 -	U2Tx
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



#ifndef __UART2_H
#define __UART2_H


#include <stdint.h>
#include "TM4C1290NCPDT.h"
#include "Fifo.h"



/**     UART2_Init Function
 *  @brief      Initializes UART2. Used for SIM800H (GSM)
 *  @details    Pins Used:
 *                  PD4 - UART2 Rx
 *                  PD5 - UART2 Tx
 *  @param[in]  baudrate    Baudrate for UART2 
 * 
 */
void UART2_Init(int baudrate);



/**     UART2_InChar Function
 *  @brief      Waits for serial port to receive message.
 *  @return  		8 bit ASCII message received from UART2 serial port 
 * 
 */
char UART2_InChar(void);



/**     UART2_InString Function
 *  @brief      Scans multiple 8-bit frames from UART2 serial port
 *  @param[in]  *bufPt    ASCII message formatted in NULL terminated array 
 *	@param[in]	size			Size of ASCII buffer
 * 
 */
void UART2_InString(char *bufPt, uint16_t size);



/**     UART2_OutChar Function
 *  @brief      8 bits out from UART2 serial port
 *  @param[in]  data		8 bit (ASCII) message to transmit 
 * 
 */
void UART2_OutChar(char data);



/**     UART2_OutString Function
 *  @brief      Sends multiple 8-bit frames from UART2 serial port
 *  @param[in]  *pt    ASCII message formatted in NULL terminated array 
 * 
 */
void UART2_OutString(char *pt);




#endif
