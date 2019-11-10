#include <stdint.h>
#include "TouchScreen.h"
#include "TM4C1290NCPDT.h"
#include "ADC0.h"
#include "ADC1.h"

#define NUMSAMPLES		2

#define INPUT		0
#define OUTPUT		1

#define PE3		0x08
#define PE2		0x04
#define PE1		0x02
#define PE0		0x01

#define YP			PE3
#define XP			PE2
#define YM			PE1
#define XM			PE0

#define RXPLATE	300

long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value

// Y+ : PE3 : ADC0 Ch0
// X- : PE0 : ADC1 Ch3
// X+ : PE2 : GPIO out
// Y- : PE1 : GPIO out

/** digitalWrite
 * @param pinNum : 1 or 2 for PE1 or PE2
 * @param state : 0 for low, 1 for high
 */
static void digitalWrite(uint32_t pinNum, int8_t state){
	switch(pinNum){
		case PE0:
			if(state == 0)
				GPIOE_AHB->DATA &= ~(PE0);
			else
				GPIOE_AHB->DATA |= (PE0);
			break;
		
		case PE1:
			if(state == 0)
				GPIOE_AHB->DATA &= ~(PE1);
			else
				GPIOE_AHB->DATA |= (PE1);
			break;
		
		case PE2:
			if(state == 0)
				GPIOE_AHB->DATA &= ~(PE2);
			else
				GPIOE_AHB->DATA |= (PE2);
			break;
			
		case PE3:
			if(state == 0)
				GPIOE_AHB->DATA &= ~(PE3);
			else
				GPIOE_AHB->DATA |= (PE3);
			break;
			
		default:
			break;
	}
}

static void pinMode(uint32_t pinNum, int8_t direction){
	switch(pinNum){
		case PE0:
			if(direction == INPUT)
				ADC0_InitTimer0ATriggerSeq3(0, 1200000);		// Ch0, 100Hz
			
			else{
				ADC0_Disable();
				long sr = StartCritical();
				SYSCTL->RCGCGPIO |= 0x10;
				while((SYSCTL->PRGPIO & 0x10) == 0);
				GPIOE_AHB->DIR |= PE0;				// PE1 and PE2 as output
				GPIOE_AHB->AFSEL &= ~(PE0);		// No alternate function
				GPIOE_AHB->AMSEL &= ~(PE0);		// No analog
				GPIOE_AHB->DEN |= 0x06;
				EndCritical(sr);
			}
			break;
		
		case PE1:
		case PE2:
			break;
			
		case PE3:
			if(direction == INPUT)
				ADC1_InitTimer0ATriggerSeq3(3, 1200000);		// Ch3, 100Hz
			
			else{
				ADC1_Disable();
				long sr = StartCritical();
				SYSCTL->RCGCGPIO |= 0x10;
				while((SYSCTL->PRGPIO & 0x10) == 0);
				GPIOE_AHB->DIR |= PE3;				// PE1 and PE2 as output
				GPIOE_AHB->AFSEL &= ~(PE3);		// No alternate function
				GPIOE_AHB->AMSEL &= ~(PE3);		// No analog
				GPIOE_AHB->DEN |= PE3;
				EndCritical(sr);
			}
			break;
			
		default:
			break;		
	}
}


void TouchScreen_Init(void){
	// Y+
	ADC0_InitTimer0ATriggerSeq3(0, 1200000);		// Ch0, 100Hz
	
	// X-
	ADC1_InitTimer0ATriggerSeq3(3, 1200000);		// Ch3, 100Hz
	
	// X+ and Y-
	SYSCTL->RCGCGPIO |= 0x10;
	while((SYSCTL->PRGPIO & 0x10) == 0);
	GPIOE_AHB->DIR |= 0x06;				// PE1 and PE2 as output
	GPIOE_AHB->AFSEL &= ~(0x06);		// No alternate function
	GPIOE_AHB->AMSEL &= ~(0x06);		// No analog
	GPIOE_AHB->DEN |= 0x06;
}

point_t TouchScreen_GetPoint(void){
	point_t point;
	int samples[NUMSAMPLES];
	uint8_t i, valid;

	valid = 1;

	pinMode(YP, INPUT);
  pinMode(YM, INPUT);
  pinMode(XP, OUTPUT);
  pinMode(XM, OUTPUT);
	
	digitalWrite(XP, 1);
	digitalWrite(XM, 0);

	for (i=0; i<NUMSAMPLES; i++) {
		samples[i] = ADC0_GetValue();
	}

#if NUMSAMPLES == 2
	// Allow small amount of measurement noise, because capacitive
	// coupling to a TFT display's signals can induce some noise.
	if (samples[0] - samples[1] < -4 || samples[0] - samples[1] > 4) {
		valid = 0;
	} else {
		samples[1] = (samples[0] + samples[1]) >> 1; // average 2 samples
	}
#endif

	point.x = (4095-samples[NUMSAMPLES/2]);

	pinMode(XP, INPUT);
	pinMode(XM, INPUT);
	pinMode(YP, OUTPUT);
	pinMode(YM, OUTPUT);

	digitalWrite(YM, 0);
	digitalWrite(YP, 1);

	for (i=0; i<NUMSAMPLES; i++) {
	 samples[i] = ADC1_GetValue();
	}

#if NUMSAMPLES == 2
	// Allow small amount of measurement noise, because capacitive
	// coupling to a TFT display's signals can induce some noise.
	if (samples[0] - samples[1] < -4 || samples[0] - samples[1] > 4) {
	 valid = 0;
	} else {
	 samples[1] = (samples[0] + samples[1]) >> 1; // average 2 samples
	}
#endif

	point.y = (4095-samples[NUMSAMPLES/2]);

	pinMode(XP, OUTPUT);
	pinMode(YP, INPUT);
	
	digitalWrite(XP, 0);
	digitalWrite(YM, 1); 

	int z1 = ADC1_GetValue(); 
	int z2 = ADC0_GetValue();

#if RXPLATE != 0
	// now read the x 
	float rtouch;
	rtouch = z2;
	rtouch /= z1;
	rtouch -= 1;
	rtouch *= point.x;
	rtouch *= RXPLATE;
	rtouch /= 4096;

	point.z = rtouch;		 	 
#else
	point.z = (4095-(z2-z1));
#endif

	if (! valid) {
		point.z = 0;
	}

	return point;
}

int32_t TouchScreen_ReadTouchX(void){
	return 0;
}

int32_t TouchScreen_ReadTouchY(void){
	return 0;
}

