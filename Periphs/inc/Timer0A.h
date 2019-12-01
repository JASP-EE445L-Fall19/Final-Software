//  Timer0A.h
//
//
//



#include <stdint.h>
#include "TM4C1290NCPDT.h"
#include ".\lvgl\lvgl.h"



/**     Timer0A_Init Function
 *  @brief      Initializes Timer0A. Enabled for periodic interrupts
 *  @param[in]  period			Period for timer to interrupt 
 * 
 */
void Timer0A_Init(int period);







