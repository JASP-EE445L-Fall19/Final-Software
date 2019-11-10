/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2015

 Copyright 2015 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

#include <stdint.h>
#include "TM4C1290NCPDT.h"


#define NVIC_EN0_INT17          0x00020000  // Interrupt 17 enable

#define TIMER_CFG_16_BIT        0x00000004  // 16-bit timer configuration,
                                            // function is controlled by bits
                                            // 1:0 of GPTMTAMR and GPTMTBMR
#define TIMER_TAMR_TACDIR       0x00000010  // GPTM Timer A Count Direction
#define TIMER_TAMR_TAMR_PERIOD  0x00000002  // Periodic Timer mode
#define TIMER_CTL_TAOTE         0x00000020  // GPTM TimerA Output Trigger
                                            // Enable
#define TIMER_CTL_TAEN          0x00000001  // GPTM TimerA Enable
#define TIMER_IMR_TATOIM        0x00000001  // GPTM TimerA Time-Out Interrupt
                                            // Mask
#define TIMER_TAILR_TAILRL_M    0x0000FFFF  // GPTM TimerA Interval Load
                                            // Register Low

#define ADC_ACTSS_ASEN3         0x00000008  // ADC SS3 Enable
#define ADC_RIS_INR3            0x00000008  // SS3 Raw Interrupt Status
#define ADC_IM_MASK3            0x00000008  // SS3 Interrupt Mask
#define ADC_ISC_IN3             0x00000008  // SS3 Interrupt Status and Clear
#define ADC_EMUX_EM3_M          0x0000F000  // SS3 Trigger Select mask
#define ADC_EMUX_EM3_TIMER      0x00005000  // Timer
#define ADC_SSPRI_SS3_4TH       0x00003000  // fourth priority
#define ADC_SSPRI_SS2_3RD       0x00000200  // third priority
#define ADC_SSPRI_SS1_2ND       0x00000010  // second priority
#define ADC_SSPRI_SS0_1ST       0x00000000  // first priority
#define ADC_PSSI_SS3            0x00000008  // SS3 Initiate
#define ADC_SSCTL3_TS0          0x00000008  // 1st Sample Temp Sensor Select
#define ADC_SSCTL3_IE0          0x00000004  // 1st Sample Interrupt Enable
#define ADC_SSCTL3_END0         0x00000002  // 1st Sample is End of Sequence
#define ADC_SSCTL3_D0           0x00000001  // 1st Sample Diff Input Select
#define ADC_SSFIFO3_DATA_M      0x00000FFF  // Conversion Result Data mask
#define ADC_PC_SR_M             0x0000000F  // ADC Sample Rate
#define ADC_PC_SR_125K          0x00000001  // 125 ksps
#define SYSCTL_RCGCGPIO_R4      0x00000010  // GPIO Port E Run Mode Clock
                                            // Gating Control
#define SYSCTL_RCGCGPIO_R3      0x00000008  // GPIO Port D Run Mode Clock
                                            // Gating Control
#define SYSCTL_RCGCGPIO_R1      0x00000002  // GPIO Port B Run Mode Clock
                                            // Gating Control
																						
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value

// There are many choices to make when using the ADC, and many
// different combinations of settings will all do basically the
// same thing.  For simplicity, this function makes some choices
// for you.  When calling this function, be sure that it does
// not conflict with any other software that may be running on
// the microcontroller.  Particularly, ADC0 sample sequencer 3
// is used here because it only takes one sample, and only one
// sample is absolutely needed.  Sample sequencer 3 generates a
// raw interrupt when the conversion is complete, and it is then
// promoted to an ADC0 controller interrupt.  Hardware Timer0A
// triggers the ADC0 conversion at the programmed interval, and
// software handles the interrupt to process the measurement
// when it is complete.
//
// A simpler approach would be to use software to trigger the
// ADC0 conversion, wait for it to complete, and then process the
// measurement.
//
// This initialization function sets up the ADC according to the
// following parameters.  Any parameters not explicitly listed
// below are not modified:
// Timer0A: enabled
// Mode: 32-bit, down counting
// One-shot or periodic: periodic
// Interval value: programmable using 32-bit period
// Sample time is busPeriod*period
// Max sample rate: <=125,000 samples/second
// Sequencer 0 priority: 1st (highest)
// Sequencer 1 priority: 2nd
// Sequencer 2 priority: 3rd
// Sequencer 3 priority: 4th (lowest)
// SS3 triggering event: Timer0A
// SS3 1st sample source: programmable using variable 'channelNum' [0:11]
// SS3 interrupts: enabled and promoted to controller
void ADC0_InitTimer0ATriggerSeq3(uint8_t channelNum, uint32_t period){
  volatile uint32_t delay;
  // **** GPIO pin initialization ****
  switch(channelNum){             // 1) activate clock
    case 0:
    case 1:
    case 2:
    case 3:
    case 8:
    case 9:                       //    these are on GPIO_PORTE
			SYSCTL->RCGCGPIO |= SYSCTL_RCGCGPIO_R4;	break;
    case 4:
    case 5:
    case 6:
    case 7:                       //    these are on GPIO_PORTD
      SYSCTL->RCGCGPIO |= SYSCTL_RCGCGPIO_R3; break;
    case 10:
    case 11:                      //    these are on GPIO_PORTB
      SYSCTL->RCGCGPIO |= SYSCTL_RCGCGPIO_R1; break;
    default: return;              //    0 to 11 are valid channels on the LM4F120
  }
  delay = SYSCTL->RCGCGPIO ;      // 2) allow time for clock to stabilize
  delay = SYSCTL->RCGCGPIO ;
  switch(channelNum){
    case 0:                       //      Ain0 is on PE3
      GPIOE_AHB->DIR &= ~0x08;  // 3.0) make PE3 input
      GPIOE_AHB->AFSEL |= 0x08; // 4.0) enable alternate function on PE3
      GPIOE_AHB->DEN &= ~0x08;  // 5.0) disable digital I/O on PE3
      GPIOE_AHB->AMSEL |= 0x08; // 6.0) enable analog functionality on PE3
      break;
    case 1:                       //      Ain1 is on PE2
      GPIOE_AHB->DIR &= ~0x04;  // 3.1) make PE2 input
      GPIOE_AHB->AFSEL |= 0x04; // 4.1) enable alternate function on PE2
      GPIOE_AHB->DEN &= ~0x04;  // 5.1) disable digital I/O on PE2
      GPIOE_AHB->AMSEL |= 0x04; // 6.1) enable analog functionality on PE2
      break;
    case 2:                       //      Ain2 is on PE1
      GPIOE_AHB->DIR &= ~0x02;  // 3.2) make PE1 input
      GPIOE_AHB->AFSEL |= 0x02; // 4.2) enable alternate function on PE1
      GPIOE_AHB->DEN &= ~0x02;  // 5.2) disable digital I/O on PE1
      GPIOE_AHB->AMSEL |= 0x02; // 6.2) enable analog functionality on PE1
      break;
    case 3:                       //      Ain3 is on PE0
      GPIOE_AHB->DIR &= ~0x01;  // 3.3) make PE0 input
      GPIOE_AHB->AFSEL |= 0x01; // 4.3) enable alternate function on PE0
      GPIOE_AHB->DEN &= ~0x01;  // 5.3) disable digital I/O on PE0
      GPIOE_AHB->AMSEL |= 0x01; // 6.3) enable analog functionality on PE0
      break;
    case 4:                       //      Ain4 is on PD3
      GPIOD_AHB->DIR &= ~0x08;  // 3.4) make PD3 input
      GPIOD_AHB->AFSEL |= 0x08; // 4.4) enable alternate function on PD3
      GPIOD_AHB->DEN &= ~0x08;  // 5.4) disable digital I/O on PD3
      GPIOD_AHB->AMSEL |= 0x08; // 6.4) enable analog functionality on PD3
      break;
    case 5:                       //      Ain5 is on PD2
      GPIOD_AHB->DIR &= ~0x04;  // 3.5) make PD2 input
      GPIOD_AHB->AFSEL |= 0x04; // 4.5) enable alternate function on PD2
      GPIOD_AHB->DEN &= ~0x04;  // 5.5) disable digital I/O on PD2
      GPIOD_AHB->AMSEL |= 0x04; // 6.5) enable analog functionality on PD2
      break;
    case 6:                       //      Ain6 is on PD1
      GPIOD_AHB->DIR &= ~0x02;  // 3.6) make PD1 input
      GPIOD_AHB->AFSEL |= 0x02; // 4.6) enable alternate function on PD1
      GPIOD_AHB->DEN &= ~0x02;  // 5.6) disable digital I/O on PD1
      GPIOD_AHB->AMSEL |= 0x02; // 6.6) enable analog functionality on PD1
      break;
    case 7:                       //      Ain7 is on PD0
      GPIOD_AHB->DIR &= ~0x01;  // 3.7) make PD0 input
      GPIOD_AHB->AFSEL |= 0x01; // 4.7) enable alternate function on PD0
      GPIOD_AHB->DEN &= ~0x01;  // 5.7) disable digital I/O on PD0
      GPIOD_AHB->AMSEL |= 0x01; // 6.7) enable analog functionality on PD0
      break;
    case 8:                       //      Ain8 is on PE5
      GPIOE_AHB->DIR &= ~0x20;  // 3.8) make PE5 input
      GPIOE_AHB->AFSEL |= 0x20; // 4.8) enable alternate function on PE5
      GPIOE_AHB->DEN &= ~0x20;  // 5.8) disable digital I/O on PE5
      GPIOE_AHB->AMSEL |= 0x20; // 6.8) enable analog functionality on PE5
      break;
    case 9:                       //      Ain9 is on PE4
      GPIOE_AHB->DIR &= ~0x10;  // 3.9) make PE4 input
      GPIOE_AHB->AFSEL |= 0x10; // 4.9) enable alternate function on PE4
      GPIOE_AHB->DEN &= ~0x10;  // 5.9) disable digital I/O on PE4
      GPIOE_AHB->AMSEL |= 0x10; // 6.9) enable analog functionality on PE4
      break;
    case 10:                      //       Ain10 is on PB4
      GPIOB_AHB->DIR &= ~0x10;  // 3.10) make PB4 input
      GPIOB_AHB->AFSEL |= 0x10; // 4.10) enable alternate function on PB4
      GPIOB_AHB->DEN &= ~0x10;  // 5.10) disable digital I/O on PB4
      GPIOB_AHB->AMSEL |= 0x10; // 6.10) enable analog functionality on PB4
      break;
    case 11:                      //       Ain11 is on PB5
      GPIOB_AHB->DIR &= ~0x20;  // 3.11) make PB5 input
      GPIOB_AHB->AFSEL |= 0x20; // 4.11) enable alternate function on PB5
      GPIOB_AHB->DEN &= ~0x20;  // 5.11) disable digital I/O on PB5
      GPIOB_AHB->AMSEL |= 0x20; // 6.11) enable analog functionality on PB5
      break;
  }
  long sr = StartCritical();
  SYSCTL->RCGCADC |= 0x01;     // activate ADC0 
  SYSCTL->RCGCTIMER |= 0x01;   // activate timer0 
  delay = SYSCTL->RCGCTIMER;   // allow time to finish activating
  TIMER0->CTL = 0x00000000;    // disable timer0A during setup
  TIMER0->CTL |= 0x00000020;   // enable timer0A trigger to ADC
  TIMER0->CFG = 0;             // configure for 32-bit timer mode
  TIMER0->TAMR = 0x00000002;   // configure for periodic mode, default down-count settings
  TIMER0->TAPR = 0;            // prescale value for trigger
  TIMER0->TAILR = period-1;    // start value for trigger
  TIMER0->IMR = 0x00000000;    // disable all interrupts
	TIMER0->ADCEV |= (1 << 0);
  TIMER0->CTL |= 0x00000001;   // enable timer0A 32-b, periodic, no interrupts
  ADC0->PC = 0x01;         // configure for 125K samples/sec
  ADC0->SSPRI = 0x3210;    // sequencer 0 is highest, sequencer 3 is lowest
  ADC0->ACTSS &= ~0x08;    // disable sample sequencer 3
  ADC0->EMUX = (ADC0->EMUX&0xFFFF0FFF)+0x5000; // timer trigger event
  ADC0->SSMUX3 = channelNum;
  ADC0->SSCTL3 = 0x06;          // set flag and end                       
  ADC0->IM |= 0x08;             // enable SS3 interrupts
  ADC0->ACTSS |= 0x08;          // enable sample sequencer 3
	NVIC_SetPriority(ADC0SS3_IRQn, 2);
  NVIC_EnableIRQ(ADC0SS3_IRQn);
	EndCritical(sr);

}

static volatile uint32_t ADCvalue;
void ADC0SS3_Handler(void){
  ADC0->ISC = 0x08;          // acknowledge ADC sequence 3 completion
  ADCvalue = ADC0->SSFIFO3;  // 12-bit result
}

uint32_t ADC0_GetValue(void){
	uint32_t temp = 0;
	long sr = StartCritical();
	temp = ADCvalue;	
	EndCritical(sr);
	return temp;
}

void ADC0_Disable(void){
	NVIC_DisableIRQ(ADC0SS3_IRQn);
	ADC0->ACTSS &= ~0x08;    // disable sample sequencer 3
}

