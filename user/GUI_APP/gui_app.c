#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"	
#include "bsp_led.h"   
#include "bsp_usart.h"
#include "bsp_dht11.h"
#include "gui_app.h"
#include "../library/GUI/lvgl/lvgl.h"







 static void btn_event_cb(lv_event_t * e)
 {

     lv_event_code_t code = lv_event_get_code(e);
     lv_obj_t *btn = lv_event_get_target(e);
     if(code == LV_EVENT_CLICKED)
     {
         static uint8_t cnt =0;
         cnt ++;

         lv_obj_t *label = lv_obj_get_child(btn,0);
         lv_label_set_text_fmt(label,"Button: %d",cnt);

     }
 }
 
 
 
 static lv_style_t style_btn;
 static lv_style_t style_lbl;
 lv_obj_t * lbl = NULL;
 void lv_wtest(void)
 {
     
	  lv_style_reset(&style_btn);
	  lv_style_init(&style_btn);
	 
	 
	  lv_style_reset(&style_lbl);
	  lv_style_init(&style_lbl);
	 
	 
	 
//===============================
#if 0	 	 
	 
	 //lv_style_set_bg_opa(&style_btn,LV_OPA_100);
	 //lv_style_set_border_color(&style_btn,lv_color_black());
	 lv_style_set_border_width(&style_btn,2);
	 
	 //lv_style_set_text_color(&style_btn,lv_color_black());
	 

	 
	   lv_obj_t * btn = lv_btn_create(lv_scr_act());
	 
#if 0
	  LV_STATE_DEFAULT     
    LV_STATE_CHECKED     
    LV_STATE_FOCUSED    
    LV_STATE_FOCUS_KEY  
    LV_STATE_EDITED     
    LV_STATE_HOVERED     
    LV_STATE_PRESSED     
    LV_STATE_SCROLLED    
    LV_STATE_DISABLED    
	 
#endif
	 
	 
	 
	 
	 lv_obj_add_style(btn,&style_btn,LV_STATE_DISABLED);
	 
	 
     lv_obj_set_pos(btn,10,10);
     lv_obj_set_size(btn,80,20);

     //lv_obj_add_event_cb(btn,btn_event_cb,LV_EVENT_ALL,NULL);

     lv_obj_t *label = lv_label_create(btn);

	   lv_label_set_text_static(label,"Btn");
     lv_obj_center(label);


#endif
//================================================================

lv_style_set_border_width(&style_lbl,2);
lbl = lv_label_create(lv_scr_act());

  lv_obj_set_pos(lbl,10,10);
     lv_obj_set_size(lbl,40,20);

//void lv_style_set_text_color(lv_style_t * style, lv_color_t value);
lv_color_t tcolor;
tcolor.full =0xff;


//lv_label_set_long_mode(lv_obj_t * obj, lv_label_long_mode_t long_mode);
//LV_LABEL_LONG_SCROLL
//LV_LABEL_LONG_DOT
//LV_LABEL_LONG_WRAP

lv_label_set_long_mode(lbl,LV_LABEL_LONG_SCROLL);


//lv_style_set_bg_opa(lv_style_t * style, lv_opa_t value)
lv_style_set_bg_opa(&style_lbl, LV_OPA_100);

//lv_style_set_border_color(&style_lbl,lv_color_white());
//void lv_style_set_bg_color(lv_style_t * style, lv_color_t value);
lv_style_set_bg_color(&style_lbl, lv_color_black());

lv_style_set_text_color(&style_lbl, lv_color_white());

//LV_STATE_DEFAULT
//LV_STATE_FOCUSED
lv_obj_add_style(lbl,&style_lbl,LV_STATE_DEFAULT);




//lv_label_set_text_static(lbl,"Label987654321");
//lv_label_set_text(lbl,"Label987654321");
   //  lv_obj_center(lbl);
		 
		 
		 
 }
 
 void lv_mytest(void)
 {
     lv_obj_t *label = lv_label_create(lv_scr_act());
     lv_label_set_text(label,"Btn");
     lv_obj_center(label);
 }
 
 



uint8_t dht11_str[16]={0};









#if 1
void OLED_thread(void *para)
{
  BaseType_t xReturn =pdPASS;
	
	
	lv_wtest();
	
	
	while(1)
	{
		
		#if 0
		xReturn = xSemaphoreGive(BinarySem_Handle);
		
		if(xReturn == pdPASS)
		{
		
		     printf("\r\ngive semaphore ok\r\n");
		}
		vTaskDelay(5000);
		
		#endif
		
		
#if 1
		
		
		
	
		//printf("\r\noled thread\r\n");
			

		vTaskDelay(20);		
		lv_tick_inc(20);
		lv_task_handler();
		
		
		
		
		
#endif
	}
	
	
}

#endif

/*********************************************END OF FILE**********************/
