//  Touch.h
//      Header for Touch Screen for Lab 11 in EE 445L
//      Touch functionality for LCD from Adafruit
//      Uses ADC and Digital I/O for touch screen
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


#include <stdint.h>
#include "ADC.h"




/**     Touch_Init Function
 *  @brief      Initializes SPI0 and display
 *  @details    Pins Used:
 *                  PE0 - Y+ - 
 *                  PE1 - X+ -
 *                  PE2 - Y- -
 *                  PE3 - X- -
 *  
 */
void Touch_Init(void);


