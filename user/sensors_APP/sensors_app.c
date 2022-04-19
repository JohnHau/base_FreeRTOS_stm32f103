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
#include "GUI_APP/gui_app.h"


#include "sensors_APP/sensors_app.h"

#include "../library/GUI/lvgl/lvgl.h"



static uint8_t dht11_str[16]={0};



void sensors_thread(void *para)
{
	
  
	BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
	uint32_t r_queue;	/* ����һ��������Ϣ�ı��� */
	DHT11_Data_TypeDef DHT11_Data;
	DHT11_Init ();
  BASIC_TIM_Init();
	printf("DHT11 tatsk\r\n");
	
	
	
#if 0	
		/* ����Test_Queue */
  uart1_Queue = xQueueCreate((UBaseType_t ) QUEUE_LEN,/* ��Ϣ���еĳ��� */
                            (UBaseType_t ) QUEUE_SIZE);/* ��Ϣ�Ĵ�С */
  if(NULL != uart1_Queue)
    printf("����uart1_Queue��Ϣ���гɹ�!\r\n");
#endif




	
	while(1)
	{	

		
		
#if 0
		 
		//xReturn = xQueueReceive( uart1_Queue,&r_queue,0);
		xReturn = xQueueReceive( uart1_Queue,&r_queue,portMAX_DELAY);
	 if(pdTRUE == xReturn)
      printf("���ν��յ���������%d\n\n",r_queue);		
#endif
	 
	 
	 
	 	//xReturn =xSemaphoreTake(BinarySem_Handle,portMAX_DELAY);
		
#if 0
		if(xReturn == pdPASS)
		{
		
		}	
#endif
		
		
	 
#if 0	 
		
		//portDISABLE_INTERRUPTS();
		if( DHT11_Read_TempAndHumidity ( & DHT11_Data ) == SUCCESS)
			{
				
				printf("\r\n Humidity:%d.%d%% RH,Temperture:%d.%d Cel\r\n",\
				DHT11_Data.humi_int,DHT11_Data.humi_deci,DHT11_Data.temp_int,DHT11_Data.temp_deci);
			}			
			else
			{
				printf("Read DHT11 ERROR!\r\n");
			}
			//portENABLE_INTERRUPTS();
			
#endif
			//DHT11_delay(2000000);
			//vTaskDelay(2000);
			
			
			
			vTaskDelay(2000);
			
			if(DHT11_Read_TempAndHumidity ( & DHT11_Data ) == SUCCESS)
			{
				printf("\r\n��ȡDHT11�ɹ�!\r\n\r\nʪ��Ϊ%d.%d ��RH ���¶�Ϊ %d.%d�� \r\n",\
				DHT11_Data.humi_int,DHT11_Data.humi_deci,DHT11_Data.temp_int,DHT11_Data.temp_deci);
				
				
				
				
				
				dht11_to_string(&DHT11_Data,DHT11_T,dht11_str);
				
				printf("\r\n now T is %s\r\n",(char*)dht11_str);
				
				//lv_label_set_text_fmt(lbl, "%d",DHT11_Data.temp_int);
				lv_label_set_text_fmt(lbl, "%s",dht11_str);
				
				
				
				//lv_label_set_text_fmt(lbl, "%d",26);
			}			
			else
			{
				printf("Read DHT11 ERROR!\r\n");
			}
		
			
			
			
			
			
			
			
			
			
	}
	
	
}
















/*********************************************END OF FILE**********************/