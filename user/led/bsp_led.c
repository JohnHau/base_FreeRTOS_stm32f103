#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"	
#include "bsp_led.h"   
#include "bsp_usart.h"
void LED_GPIO_Config(void)
{		
		/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*����LED��ص�GPIO����ʱ��*/
		RCC_APB2PeriphClockCmd( LED1_GPIO_CLK | LED2_GPIO_CLK , ENABLE);
		/*ѡ��Ҫ���Ƶ�GPIO����*/
		GPIO_InitStructure.GPIO_Pin = LED1_GPIO_PIN;	

		/*��������ģʽΪͨ���������*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		/*������������Ϊ50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*���ÿ⺯������ʼ��GPIO*/
		GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);	
		
		/*ѡ��Ҫ���Ƶ�GPIO����*/
		GPIO_InitStructure.GPIO_Pin = LED2_GPIO_PIN;

		/*���ÿ⺯������ʼ��GPIO*/
		GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStructure);		

		/* �ر�����led��	*/
		GPIO_SetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);
		
		/* �ر�����led��	*/
		GPIO_SetBits(LED2_GPIO_PORT, LED2_GPIO_PIN);
}


void LED1_Task(void *para)
{

	BaseType_t xReturn =pdPASS;
	while(1)
	{
		
#if 1
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
		
		
#if 1
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
		
		
		
#if 0
		LED1(0);//LED1_ON();
		vTaskDelay(500);
		LED1(1);//LED1_OFF();
		vTaskDelay(500);
		
#endif
	}
	
	
}



#if 1
void LED2_Task(void *para)
{
  BaseType_t xReturn =pdPASS;
	while(1)
	{
		
		
		xReturn = xSemaphoreGive(BinarySem_Handle);
		
		if(xReturn == pdPASS)
		{
		
		     printf("\r\ngive semaphore ok\r\n");
		}
		vTaskDelay(5000);
#if 0
		LED2(0);//LED2_ON();
		vTaskDelay(500);
		LED2(1);//LED2_OFF();
		vTaskDelay(500);
#endif
	}
	
	
}

#endif

/*********************************************END OF FILE**********************/
