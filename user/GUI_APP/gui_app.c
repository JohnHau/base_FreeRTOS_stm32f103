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


 
 static lv_style_t style_btn;
 static lv_style_t style_lbl;
 
 
 lv_obj_t * lbl = NULL;
 lv_obj_t * lbl_01 = NULL;
 lv_obj_t * lbl_02 = NULL;
 lv_obj_t * lbl_03 = NULL;
 
 
 lv_obj_t * lbl_R = NULL;
 lv_obj_t * lbl_T = NULL;
 lv_obj_t * lbl_info = NULL;
 
 lv_obj_t * lbl_date_time = NULL;
 
 lv_obj_t* screen_00 = NULL;
 lv_obj_t* screen_01 = NULL;
 lv_obj_t* screen_02 = NULL;
 
 lv_obj_t* scr_rt = NULL;
 lv_obj_t* scr_info = NULL;

lv_obj_t * btn = NULL;
lv_obj_t *label =NULL;

 static void btn_event_cb(lv_event_t * e)
 {

     lv_event_code_t code = lv_event_get_code(e);
     lv_obj_t *bbtn = lv_event_get_target(e);

	// printf("btn_event_cb\r\n");
	 
	 
  //   if(code == LV_EVENT_CLICKED)
	// printf("code is %d\r\n",(uint8_t)code);
	 
	 
	 //uint32_t mk = lv_indev_get_key(lv_indev_get_act());
	 
	 //printf("mk is %d\r\n",mk);
	 
	 
	 
	 
	 
	 if(bbtn == btn)
	 {
	 
			// if(code == LV_EVENT_KEY)
			 if(code == LV_EVENT_PRESSED)
				//	if(code == LV_EVENT_RELEASED)
			 //if(code == LV_EVENT_LONG_PRESSED)
				//  if(code == LV_EVENT_LONG_PRESSED_REPEAT)
				 {
						 static uint8_t cnt = 50;
						 				 				 
					  uint32_t mk = lv_indev_get_key(lv_indev_get_act());
	 
	         // printf("================mk is %d ====================\r\n",mk);
					 
					 
					 if(mk == LV_KEY_PREV)
					 {
					    cnt --;
					 }
					 else if(mk == LV_KEY_NEXT) 
					 {
					    cnt ++;
					 }
					 else if(mk == LV_KEY_ENTER) 
					 {
					 
					    cnt =0;
					 }
					 else if(mk == LV_USR_KEY_UP) 
					 {
					    cnt ++;
					 }
						 lv_obj_t *label = lv_obj_get_child(btn,0);
						 lv_label_set_text_fmt(label,"B%d",cnt);

				 }
				 
		 
		 
		 
	 }
		 
		 
		 
		 
 }
 
 

 
extern lv_indev_t * indev_keypad;
 
 void lv_btest(void)
 {
     
	 // lv_style_reset(&style_btn);
	 // lv_style_init(&style_btn);
	 
	 
	 // lv_style_reset(&style_lbl);
	 // lv_style_init(&style_lbl);
	 
	 
	 
//===============================
#if 1
	 
	     /*Later you should create group(s) with `lv_group_t * group = lv_group_create()`,
     *add objects to the group with `lv_group_add_obj(group, obj)`
     *and assign this input device to group to navigate in it:
     *`lv_indev_set_group(indev_keypad, group);`*/
	 

	 
	 
	 lv_style_set_border_width(&style_btn,2);
	  
	 btn = lv_btn_create(lv_scr_act());
	
   
	 //lv_obj_add_style(btn,&style_btn,LV_STATE_DISABLED);
	 lv_obj_add_style(btn,&style_btn,LV_STATE_DEFAULT);
	 
	 
   lv_obj_set_pos(btn,10,10);
   lv_obj_set_size(btn,60,20);

   

  // lv_group_t * g = lv_group_get_default();
	 lv_group_t * g = lv_group_create();/////////////////////////
	 //lv_group_set_default(g);
	 lv_indev_set_group(indev_keypad, g);
	 lv_group_add_obj(g, btn);
	 
   lv_obj_add_event_cb(btn,btn_event_cb,LV_EVENT_ALL,NULL);

 
 
     label = lv_label_create(btn);

	   lv_label_set_text_static(label,"Btn");
     lv_obj_center(label);


#endif
//==================================

		 	 	 
 }
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 //===========================================
 
 
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
     lv_obj_set_size(btn,40,20);

     //lv_obj_add_event_cb(btn,btn_event_cb,LV_EVENT_ALL,NULL);

     lv_obj_t *label = lv_label_create(btn);

	   lv_label_set_text_static(label,"Btn");
     lv_obj_center(label);


#endif
//================================================================

lv_style_set_border_width(&style_lbl,2);

  screen_00 = lv_scr_act();
	
	screen_01 = lv_obj_create(NULL);
	
	screen_02 = lv_obj_create(NULL);
	
	
  lbl = lv_label_create(screen_00);
  lv_obj_set_pos(lbl,10,10);
  lv_obj_set_size(lbl,40,20);
		 
	lbl_01 = lv_label_create(lv_scr_act());
  lv_obj_set_pos(lbl_01,10,30);
  lv_obj_set_size(lbl_01,40,20);	 
		 
		 
		 

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
		 
		 	 
		 my_screen_01();
		 
		 	 	 
 }
 
 
 
 
 
 
 
 
 
 
 
  void my_screen_01(void)
 {
     
	  lv_style_reset(&style_btn);
	  lv_style_init(&style_btn);
	 
	 
	  lv_style_reset(&style_lbl);
	  lv_style_init(&style_lbl);
	 
	 
	

   lv_style_set_border_width(&style_lbl,2);


  lbl_02 = lv_label_create(screen_01);
  lv_obj_set_pos(lbl_02,10,10);
  lv_obj_set_size(lbl_02,40,20);
		 
	lbl_03 = lv_label_create(screen_01);
  lv_obj_set_pos(lbl_03,10,30);
  lv_obj_set_size(lbl_03,40,20);	 
		 
		 


//lv_label_set_long_mode(lbl,LV_LABEL_LONG_SCROLL);


//lv_style_set_bg_opa(&style_lbl, LV_OPA_100);


//lv_style_set_bg_color(&style_lbl, lv_color_black());

//lv_style_set_text_color(&style_lbl, lv_color_white());

lv_obj_add_style(lbl,&style_lbl,LV_STATE_DEFAULT);




lv_label_set_text_static(lbl_02,"321");
lv_label_set_text_static(lbl_03,"987");
//lv_label_set_text(lbl,"Label987654321");
   //  lv_obj_center(lbl);
		 
		 
		 
 }
 
 
 
 
 
 static void lbl_T_event_cb(lv_event_t * e)
 {

     lv_event_code_t code = lv_event_get_code(e);
     lv_obj_t *llbl_T = lv_event_get_target(e);

	// printf("btn_event_cb\r\n");
	 
	 
  //   if(code == LV_EVENT_CLICKED)
	// printf("code is %d\r\n",(uint8_t)code);
	 
	 
	 //uint32_t mk = lv_indev_get_key(lv_indev_get_act());
	 
	 //printf("mk is %d\r\n",mk);
	 
	 
	 
	 
	 
	 if(llbl_T == lbl_T)
	 {
	 
			// if(code == LV_EVENT_KEY)
			 if(code == LV_EVENT_PRESSED)
				//	if(code == LV_EVENT_RELEASED)
			 //if(code == LV_EVENT_LONG_PRESSED)
				//  if(code == LV_EVENT_LONG_PRESSED_REPEAT)
				 {
						 static uint8_t cnt = 50;
						 				 				 
					  uint32_t mk = lv_indev_get_key(lv_indev_get_act());
	 
	         // printf("================mk is %d ====================\r\n",mk);
					 
					 
					 if(mk == LV_KEY_PREV)
					 {
					    cnt --;
					 }
					 else if(mk == LV_KEY_NEXT) 
					 {
					    cnt ++;
					 }
					 else if(mk == LV_KEY_ENTER) 
					 {
					    printf("lbl callback KEY_ENTER\r\n");
					    cnt =0;
					 }
					 else if(mk == LV_USR_KEY_UP) 
					 {
					   printf("lbl callback KEY_UP\r\n"); 
						 cnt ++;
					 }
						 //lv_obj_t *label = lv_obj_get_child(btn,0);
						 //lv_label_set_text_fmt(label,"B%d",cnt);
					 
					 printf("lbl callback %d\r\n",cnt);
					 lv_scr_load(scr_info);
		
					 

				 }
				 
		 
		 
		 
	 }
		 
		 
		 
		 
 }
  void screen_INFO(void);
 
 void screen_RT(void)
 {
  
	  lv_style_reset(&style_lbl);
	  lv_style_init(&style_lbl);
	 
    lv_style_set_border_width(&style_lbl,2);

    scr_rt = lv_scr_act();
			 
	  lbl_T = lv_label_create(scr_rt);
    lv_obj_set_pos(lbl_T,0,0);
    lv_obj_set_size(lbl_T,60,20);	 
		 
	  lbl_R = lv_label_create(scr_rt);
    lv_obj_set_pos(lbl_R,0,18);
    lv_obj_set_size(lbl_R,70,20);

	 
	  lbl_date_time = lv_label_create(scr_rt);
    lv_obj_set_pos(lbl_date_time,0,36);
    lv_obj_set_size(lbl_date_time,125,20);
	 
	 
	   // lv_group_t * g = lv_group_get_default();
	 lv_group_t * g = lv_group_create();/////////////////////////
	 //lv_group_set_default(g);
	 lv_indev_set_group(indev_keypad, g);
	 lv_group_add_obj(g, lbl_T);
	 
	 lv_obj_add_event_cb(lbl_T,lbl_T_event_cb,LV_EVENT_ALL,NULL);
	 
	 
   lv_obj_add_style(lbl_R,&style_lbl,LV_STATE_DEFAULT);
   lv_obj_add_style(lbl_T,&style_lbl,LV_STATE_DEFAULT);
	 lv_obj_add_style(lbl_date_time,&style_lbl,LV_STATE_DEFAULT);
	 
	 
	  screen_INFO();
	 
	 
	 
	 
	 
	 
	 
	 

 }
 
 
 void screen_INFO(void)
 {
  
	  lv_style_reset(&style_lbl);
	  lv_style_init(&style_lbl);
	 
    lv_style_set_border_width(&style_lbl,2);

    scr_info = lv_obj_create(NULL);
			 
	  lbl_info = lv_label_create(scr_info);
    lv_obj_set_pos(lbl_info,0,0);
    lv_obj_set_size(lbl_info,100,20);	 
		 

    lv_obj_add_style(lbl_info,&style_lbl,LV_STATE_DEFAULT);
	 
	 lv_obj_add_style(lbl_info,&style_lbl,LV_STATE_DEFAULT);
	 lv_label_set_text(lbl_info,"info:dth11 routine");
 }
 
 
 
 
 
 void lv_mytest(void)
 {
     lv_obj_t *label = lv_label_create(lv_scr_act());
     lv_label_set_text(label,"Btn");
     lv_obj_center(label);
 }
 
 

 
 
 
 
 
 
 
 
 
 
 
 
 
  void screen_test(void)
 {
  
	  lv_style_reset(&style_lbl);
	  lv_style_init(&style_lbl);
	 
    lv_style_set_border_width(&style_lbl,2);

    scr_rt = lv_scr_act();
			 
	  lbl_T = lv_label_create(scr_rt);
    lv_obj_set_pos(lbl_T,0,0);
    lv_obj_set_size(lbl_T,60,20);	 

   lv_obj_add_style(lbl_T,&style_lbl,LV_STATE_DEFAULT);
	 


 }
 
 
 


uint8_t dht11_str[16]={0};



#if 1
void OLED_thread(void *para)
{
  BaseType_t xReturn =pdPASS;
	static uint32_t cnt =1;
	
	//lv_wtest();
	//lv_btest();
	
	screen_RT();
	
	
	//screen_test();
	
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
		
		//uint32_t mk = lv_indev_get_key(lv_indev_get_act());
	  //printf("\r\nmk is %d\r\n",mk);
			
		//printf("\r\noled thread\r\n");
			

		vTaskDelay(1);		
		lv_tick_inc(1);
		lv_task_handler();
		
		
		
		if(lv_scr_act() == scr_info)
		{
		  cnt ++;
			//printf("\r\nswitch cnt %dr\n",cnt);
			if(cnt %5000 ==0)
			{
					lv_scr_load(scr_rt);
				  cnt =1;
			}
			
		}
		
		
#endif
	}
	
	
}

#endif

/*********************************************END OF FILE**********************/
