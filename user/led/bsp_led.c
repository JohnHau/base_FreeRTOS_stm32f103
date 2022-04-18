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

#include "../library/GUI/lvgl/lvgl.h"




DHT11_Data_TypeDef DHT11_Data;



void LED_GPIO_Config(void)
{		
		/*定义一个GPIO_InitTypeDef类型的结构体*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*开启LED相关的GPIO外设时钟*/
		RCC_APB2PeriphClockCmd( LED1_GPIO_CLK | LED2_GPIO_CLK , ENABLE);
		/*选择要控制的GPIO引脚*/
		GPIO_InitStructure.GPIO_Pin = LED1_GPIO_PIN;	

		/*设置引脚模式为通用推挽输出*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		/*设置引脚速率为50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*调用库函数，初始化GPIO*/
		GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);	
		
		/*选择要控制的GPIO引脚*/
		GPIO_InitStructure.GPIO_Pin = LED2_GPIO_PIN;

		/*调用库函数，初始化GPIO*/
		GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStructure);		

		/* 关闭所有led灯	*/
		GPIO_SetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);
		
		/* 关闭所有led灯	*/
		GPIO_SetBits(LED2_GPIO_PORT, LED2_GPIO_PIN);
}












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
 
 
















void LED1_Task(void *para)
{

	BaseType_t xReturn =pdPASS;
	while(1)
	{
		
#if 0
		xReturn =xSemaphoreTake(BinarySem_Handle,portMAX_DELAY);
		
#if 0	
		if(xReturn == pdPASS)
		{
			printf("got data\r\n");
			
			//printf("data is %d %d %d\r\n",buffer_rx_uart1[0],buffer_rx_uart1[1],buffer_rx_uart1[2]);
			printf("data is %s\r\n",buffer_rx_uart1);
			memset(buffer_rx_uart1, 0, sizeof(buffer_rx_uart1));
		
		}
#endif
		
		
#if 0
		if(xReturn == pdPASS)
		{
			vTaskDelay(500);
			printf("\r\ngot sem\r\n");
			#if 1
			LED1(0);//LED1_ON();
			vTaskDelay(500);
			LED1(1);//LED1_OFF();
			vTaskDelay(500);
		
		  #endif
		
		}
		
#endif
		
		
#endif
		
		
		
#if 1
		LED1(0);//LED1_ON();
		vTaskDelay(1000);
		LED1(1);//LED1_OFF();
		vTaskDelay(1000);
		
		

		
		
		if( DHT11_Read_TempAndHumidity ( & DHT11_Data ) == SUCCESS)
			{
				printf("\r\n读取DHT11成功!\r\n\r\n湿度为%d.%d ％RH ，温度为 %d.%d℃ \r\n",\
				DHT11_Data.humi_int,DHT11_Data.humi_deci,DHT11_Data.temp_int,DHT11_Data.temp_deci);
				lv_label_set_text_fmt(lbl, "%d",DHT11_Data.temp_int);
				//lv_label_set_text_fmt(lbl, "%d",26);
			}			
			else
			{
				printf("Read DHT11 ERROR!\r\n");
			}
		
		
		
#endif
	}
	
	
}



#if 1
void LED2_Task(void *para)
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
		
		
		
		#if 0
		LED2(0);//LED2_ON();
		vTaskDelay(500);
		LED2(1);//LED2_OFF();
		vTaskDelay(500);
		#endif
		
		
		vTaskDelay(50);
		//printf("\r\nled2 on off\r\n");
		
		
				
		lv_tick_inc(1);
		lv_task_handler();
		
		
		
		
		
#endif
	}
	
	
}

#endif

/*********************************************END OF FILE**********************/
