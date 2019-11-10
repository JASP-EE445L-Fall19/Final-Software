#ifndef __TOUCHSCREEN_H
#define __TOUCHSCREEN_H

#include <stdint.h>

typedef struct{
	int32_t x;
	int32_t y;
	int32_t z;
} point_t;

void TouchScreen_Init(void);

point_t TouchScreen_GetPoint(void);

int32_t TouchScreen_ReadTouchX(void);

int32_t TouchScreen_ReadTouchY(void);


#endif
