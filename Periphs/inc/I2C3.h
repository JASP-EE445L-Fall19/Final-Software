//  I2C3.h
//      Header for I2C drivers for Lab 11 in EE 445L
//      General Purpose I2C
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

#ifndef __I2C3_H
#define __I2C3_H


#include <stdint.h>


/**     I2C3_Init Function
 *  @brief      Initializes SPI0. Used for LCD Display
 *  @details    Pins Used:
 *                  PK4 - MOSI0
 *                  PK5 - MISO0
 *  @note       Will add params later
 * 
 */
void I2C3_Init(void);





#endif
