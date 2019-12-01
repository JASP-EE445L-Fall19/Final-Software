/*
 * SSI0.c
 * ----------
 * Inspired by examples in ValvanoWareTM4C123 by Dr. Jonathan Valvano
 * as well as his book Embedded Systems: Real-Time Interfacing to Arm Cortex-M Microcontrollers
 * You can find ValvanoWareTM4C123 at http://edx-org-utaustinx.s3.amazonaws.com/UT601x/ValvanoWareTM4C123.zip?dl=1
 * You can find his book at https://www.amazon.com/gp/product/1463590156/ref=oh_aui_detailpage_o05_s00?ie=UTF8&psc=1
 * You can find more of his work at http://users.ece.utexas.edu/~valvano/
 * ----------
 * @author Zee Livermorium, Sijin Woo
 * @date Oct 30, 2019
 */

#include <stdint.h>
#include <string.h>
#include "SSI0.h"
#include "TM4C1290NCPDT.h"

#define SYSCTL_RCGCSSI_R0			0x00000001
#define SYSCTL_RCGCGPIO_R0			0x00000001
#define SYSCTL_PRGPIO_R0				0x00000001

#define SSI_CR1_EOT						0x00000010  // End of Transmission
#define SSI_CR1_MS							0x00000004  // SSI Master/Slave Select
#define SSI_CR1_SSE						0x00000002  // SSI Synchronous Serial Port
                                            // Enable
#define SSI_CR1_LBM						0x00000001  // SSI Loopback Mode

#define SSI_CC_CS_M						0x0000000F  // SSI Baud Clock Source
#define SSI_CC_CS_SYSPLL				0x00000000  // System clock (based on clock
                                            // source and divisor factor)
#define SSI_CC_CS_PIOSC				0x00000005  // PIOSC
#define SSI_CPSR_CPSDVSR_M			0x000000FF  // SSI Clock Prescale Divisor
#define SSI_CPSR_CPSDVSR_S			0

#define SSI_CR0_SCR_M					0x0000FF00  // SSI Serial Clock Rate
#define SSI_CR0_SPH 						0x00000080  // SSI Serial Clock Phase
#define SSI_CR0_SPO						0x00000040  // SSI Serial Clock Polarity
#define SSI_CR0_FRF_M					0x00000030  // SSI Frame Format Select
#define SSI_CR0_FRF_MOTO				0x00000000  // Freescale SPI Frame Format
#define SSI_CR0_FRF_TI					0x00000010  // Synchronous Serial Frame Format
#define SSI_CR0_FRF_NMW				0x00000020  // MICROWIRE Frame Format
#define SSI_CR0_DSS_M					0x0000000F  // SSI Data Size Select
#define SSI_CR0_DSS_4					0x00000003  // 4-bit data
#define SSI_CR0_DSS_5					0x00000004  // 5-bit data
#define SSI_CR0_DSS_6					0x00000005  // 6-bit data
#define SSI_CR0_DSS_7					0x00000006  // 7-bit data
#define SSI_CR0_DSS_8					0x00000007  // 8-bit data
#define SSI_CR0_DSS_9					0x00000008  // 9-bit data
#define SSI_CR0_DSS_10					0x00000009  // 10-bit data
#define SSI_CR0_DSS_11					0x0000000A  // 11-bit data
#define SSI_CR0_DSS_12					0x0000000B  // 12-bit data
#define SSI_CR0_DSS_13					0x0000000C  // 13-bit data
#define SSI_CR0_DSS_14					0x0000000D  // 14-bit data
#define SSI_CR0_DSS_15					0x0000000E  // 15-bit data
#define SSI_CR0_DSS_16					0x0000000F  // 16-bit data
#define SSI_CR0_SCR_S					8

#define SSI_SR_BSY						0x00000010  // SSI Busy Bit
#define SSI_SR_RFF							0x00000008  // SSI Receive FIFO Full
#define SSI_SR_RNE							0x00000004  // SSI Receive FIFO Not Empty
#define SSI_SR_TNF							0x00000002  // SSI Transmit FIFO Not Full
#define SSI_SR_TFE							0x00000001  // SSI Transmit FIFO Empty

/*
 *  SSI0 A Conncection
 *  ------------------
 *  SCK  --------- PA2
 *  SS   --------- PA3
 *  MOSI --------- PA4
 *  MISO --------- PA5 
 */

/****************************************************
 *                                                  *
 *                 Helper Functions                 *
 *                                                  *
 ****************************************************/

/**
 * reverseBitOrder
 * ----------
 * Discription: to output in the order of LSB first, we need to reverse all bits.
 */
static uint8_t reverseBitOrder(uint8_t byte) {
    return (((byte & 0x01) << 7) +
            ((byte & 0x02) << 5) +
            ((byte & 0x04) << 3) +
            ((byte & 0x08) << 1) +
            ((byte & 0x10) >> 1) +
            ((byte & 0x20) >> 3) +
            ((byte & 0x40) >> 5) +
            ((byte & 0x80) >> 7));
}

/****************************************************
 *                                                  *
 *                   SS Functions                   *
 *                                                  *
 ****************************************************/

void SSI0_SS_HIGH (void) {
	GPIOA_AHB->DATA |= 0x08;
}

void SSI0_SS_LOW (void) {
	GPIOA_AHB->DATA &= ~0x08;
}


/****************************************************
 *                                                  *
 *                   Initializer                    *
 *                                                  *
 ****************************************************/

/**
 * SSI0_Init
 * ----------
 * @brief initialize SSI0 on Port A with corresponding setting parameters.
 */
void SSI0_Init (void) {
	/*-- SSI0 and Port A Activation --*/
	SYSCTL->RCGCSSI |= SYSCTL_RCGCSSI_R0;									// enable SSI Module 0 clock
	SYSCTL->RCGCGPIO |= SYSCTL_RCGCGPIO_R0;								// enable GPIO Port A clock
	while ((SYSCTL->PRGPIO & SYSCTL_PRGPIO_R0) == 0) {};  // allow time for activating

	/*-- Port A Set Up --*/
	GPIOA_AHB->DIR |= 0x08;                             	// make PA3 output
	GPIOA_AHB->AFSEL |= 0x3C;                            	// enable alt funct on PA2, 4, 5
	//GPIOA_AHB->AFSEL &= ~0x08;                           	// disable alt funct on PA3
	GPIOA_AHB->PCTL &= ~((0xF << 8) | (0xF << 12) | (0xF << 16) | (0xF << 20));
	GPIOA_AHB->PCTL |= (0xF << 8) | (0xF << 12) | (0xF << 16) | (0xF << 20);
	GPIOA_AHB->AMSEL &= ~0x3C;                           	// disable analog functionality on PA2-5
	GPIOA_AHB->DEN |= 0x3C;                              	// enable digital I/O on PA2-5

	/*-- SSI0 Set Up --*/
	SSI0->CR1 &= ~SSI_CR1_SSE;                            // disable SSI0 operation
	SSI0->CR1 &= ~SSI_CR1_MS;                             // configure SSI0 as master mode
	SSI0->CC &= ~SSI_CC_CS_M;
	SSI0->CC |= SSI_CC_CS_SYSPLL;
	SSI0->CPSR &= ~SSI_CPSR_CPSDVSR_M;                    // clear bit fields for SSI Clock Prescale Divisor
	SSI0->CPSR += 4;                                     // 120 clock divisor, 1Mhz
	SSI0->CR0 &= ~SSI_CR0_SCR_M;                          // clear bit fields for SSI0 Serial Clock Rate, SCR = 0
	SSI0->CR0 &= ~SSI_CR0_SPO;                            // clear bit fields for SSI0 Serial Clock Polarity, SPO = 0
	SSI0->CR0 &= ~SSI_CR0_SPH;                            // clear bit fields for SSI0 Serial Clock Phase, SPH = 0
	SSI0->CR0 &= ~SSI_CR0_FRF_M;                          // clear bit fields for SSI0 Frame Format Select
	SSI0->CR0 |= SSI_CR0_FRF_MOTO;                        // set frame format to Freescale SPI Frame Format
	SSI0->CR0 &= ~SSI_CR0_DSS_M;                          // clear bit fields for SSI0 Data Size Select
	SSI0->CR0 |= SSI_CR0_DSS_8;                           // set SSI data size to 8
	SSI0->CR1 |= SSI_CR1_SSE;                             // enable SSI operation
}


/****************************************************
 *                                                  *
 *                   I/O Functions                  *
 *                                                  *
 ****************************************************/

/**
 * SSI0_read
 * ----------
 * @return date read from slave device.
 */
uint16_t SSI0_read (void) {
	while((SSI0->SR & SSI_SR_BSY) == SSI_SR_BSY) {};     // wait until SSI0 not busy/transmit FIFO empty
	SSI0->DR = 0x00;                                     // data out, garbage, just for synchronization
	while((SSI0->SR & SSI_SR_RNE) == 0) {};              // wait until response
	return SSI0->DR;                                     // read data
}

/**
 * SSI0_write
 * ----------
 * @param  data  data to be written.
 */
void SSI0_write (uint16_t data){
	while ((SSI0->SR & SSI_SR_BSY) == SSI_SR_BSY) {};   // wait until SSI1 not busy/transmit FIFO empty
	SSI0->DR = data;                                    // write data
	while ((SSI0->SR & SSI_SR_RNE) == 0) {};            // wait until response
	uint16_t sync = SSI0->DR;                           // read byte of data, just for synchronization
}
