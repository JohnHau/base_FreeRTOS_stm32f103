#ifndef __GUI_APP_H__
#define __GUI_APP_H__


#include "stm32f10x.h"
#include "../library/GUI/lvgl/lvgl.h"



extern lv_obj_t * lbl;








extern void OLED_thread(void *para);

#endif /* __GUI_APP_H__ */
