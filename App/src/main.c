


#include <stdint.h>
#include <stdio.h>
#include "TM4C1290NCPDT.h"
#include "PLL.h"
#include "TouchScreen.h"
#include "HX8357.h"
#include "UART0.h"
#include "SysTick.h"
#include "SIM800H.h"
#include "Timer0A.h"
#include ".\lvgl\lvgl.h"

point_t point;

void my_disp_flush(lv_disp_t * disp, const lv_area_t * area, lv_color_t * color_p)
{
    int32_t x, y;
    for(y = area->y1; y <= area->y2; y++) {
        for(x = area->x1; x <= area->x2; x++) {
            HX8357_DrawPixel(x, y, color_p->full);  /* Put a pixel to the display.*/
            color_p++;
        }
    }

    lv_disp_flush_ready((lv_disp_drv_t *)disp);         /* Indicate you are ready with the flushing*/
}

int main(){
	PLL_Init120MHz();

	TouchScreen_Init();
	HX8357_InitD();
	HX8357_SetCursor(0, 0);
	HX8357_OutString("Hello there.\r\nGeneral Kenobi.");	
	
	lv_init();
	
	static lv_disp_buf_t disp_buf;
	static lv_color_t buf[LV_HOR_RES_MAX * 10];                     /*Declare a buffer for 10 lines*/
	lv_disp_buf_init(&disp_buf, buf, NULL, LV_HOR_RES_MAX * 10);    /*Initialize the display buffer*/	
	
	lv_disp_drv_t disp_drv;               /*Descriptor of a display driver*/
	lv_disp_drv_init(&disp_drv);          /*Basic initialization*/
	disp_drv.flush_cb = my_disp_flush;    /*Set your driver function*/
	disp_drv.buffer = &disp_buf;          /*Assign the buffer to the display*/
	lv_disp_drv_register(&disp_drv);      /*Finally register the driver*/
	
//;
//	
//	
//	lv_obj_t * slider1 = lv_slider_create(lv_scr_act(), NULL);
//	lv_obj_set_x(slider1, 30);
//	lv_obj_set_y(slider1, 10);
//	lv_obj_set_size(slider1, 200, 50);
//	lv_slider_set_value(slider1, 70, LV_ANIM_ON);
	
	static const char * btnm_map[] = {"1", "2", "3", "\n", "4", "5", "6", "\n",
																		 "7", "8", "9", "\n", "#", "0", "*",
                                   ""};
	
	lv_obj_t * btnm1 = lv_btnm_create(lv_scr_act(), NULL);
  lv_btnm_set_map(btnm1, btnm_map);
  //lv_btnm_set_btn_width(btnm1, 10, 2);        /*Make "Action1" twice as wide as "Action2"*/
  lv_obj_align(btnm1, NULL, LV_ALIGN_CENTER, 0, 0);
  //lv_obj_set_event_cb(btnm1, event_handler);
	
	
//	UART0_Init(115200);
//	
//	printf("start\r");
	SysTick_Init(0x00FFFFFF);
//	SIM800H_Init();

	SYSCTL->RCGCGPIO |= (1<<12);
	while ((SYSCTL->PRGPIO & (1<<12)) == 0) {};
	GPION->DIR |= 0x01;
	GPION->DEN |= 0x01;
//	
//	char pt;
//	int num;
//	char str[100];
	while(1){
		lv_task_handler();
		
		GPION->DATA |= 0x01;
		//scanf("%s", &str[0]);
		SysTick_Wait10ms(50);
		GPION->DATA &= ~0x01;
		//printf("Hi %s\r", str);
		SysTick_Wait10ms(50);
		
	}
}
