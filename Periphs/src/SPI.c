//  SPI.c
//      Source Code for SPI drivers for Lab 11 in EE 445L
//      General Purpose SPI
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


#include "SPI.h"



/**     SPI0_Init Function
 *  @brief      Initializes SPI0. Used for LCD Display
 *  @details    Pins Used:
 *                  PA2 - Sck0
 *                  PA3 - CS0/Fss0
 *                  PA4 - MOSI0
 *                  PA5 - MISO0
 *  @param[in]  freq        Speed of SPI0 
 *  @param[in]  polarity    Polarity of Clock
 *  @param[in]  phase       Phase of Clock
 * 
 */
void SPI0_Init(int freq, int polarity, int phase) {


}




/**     SPI1_Init Function
 *  @brief      Initializes SPI1. Used for DAC
 *  @details    Pins Used:
 *                  PB4 - Fss1
 *                  PB5 - Sck1
 *                  PE4 - MOSI1
 *                  PE5 - CS1
 *  @param[in]  freq        Speed of SPI1 
 *  @param[in]  polarity    Polarity of Clock
 *  @param[in]  phase       Phase of Clock
 * 
 */
void SPI1_Init(int freq, int polarity, int phase) {

    
}



