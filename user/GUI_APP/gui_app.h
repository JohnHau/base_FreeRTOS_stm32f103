#ifndef __GUI_APP_H__
#define __GUI_APP_H__


#include "stm32f10x.h"
#include "../library/GUI/lvgl/lvgl.h"



extern lv_obj_t * lbl;

extern lv_obj_t * lbl_R;
extern lv_obj_t * lbl_T;
extern lv_obj_t * lbl_date_time;

extern lv_obj_t* screen_00;
extern lv_obj_t* screen_01;
extern lv_obj_t* screen_02;

void my_screen_01(void);

extern lv_obj_t* scr_rt;
extern lv_obj_t* scr_info;

extern void OLED_thread(void *para);

#endif /* __GUI_APP_H__ */
