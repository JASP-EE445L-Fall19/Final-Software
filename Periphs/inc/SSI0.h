/*
 * SSI0.h
 * ----------
 * Inspired by examples in ValvanoWareTM4C123 by Dr. Jonathan Valvano
 * as well as his book Embedded Systems: Real-Time Interfacing to Arm Cortex-M Microcontrollers
 * You can find ValvanoWareTM4C123 at http://edx-org-utaustinx.s3.amazonaws.com/UT601x/ValvanoWareTM4C123.zip?dl=1
 * You can find his book at https://www.amazon.com/gp/product/1463590156/ref=oh_aui_detailpage_o05_s00?ie=UTF8&psc=1
 * You can find more of his work at http://users.ece.utexas.edu/~valvano/
 * ----------
 * Zee Livermorium, Sijin Woo
 * Oct 30, 2019
 */
 
#ifndef __SSI0_H
#define __SSI0_H

#include <stdint.h>

/**
 * SSI0_A_Init
 * ----------
 * @brief initialize SSI0 on Port A with corresponding setting parameters.
 */
void SSI0_Init (void);

// for bit banging
void SSI0_SS_HIGH (void);
void SSI0_SS_LOW (void);

/**
 * SSI0_Read
 * ----------
 * @return: date read from another device.
 */
uint16_t SSI0_read (void);

/**
 * SSI0_write
 * ----------
 * @param  data  byte of data to be written.
 */
void SSI0_write(uint16_t data);

#endif
