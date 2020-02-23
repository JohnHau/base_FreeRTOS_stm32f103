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


void LED1_Task(void *para)
{

	BaseType_t xReturn =pdPASS;
	while(1)
	{
		
		
		xReturn =xSemaphoreTake(BinarySem_Handle,portMAX_DELAY);
		
		
		if(xReturn == pdPASS)
		{
			printf("got data\r\n");
			
			//printf("data is %d %d %d\r\n",buffer_rx_uart1[0],buffer_rx_uart1[1],buffer_rx_uart1[2]);
			printf("data is %s\r\n",buffer_rx_uart1);
			memset(buffer_rx_uart1, 0, sizeof(buffer_rx_uart1));
		
		}
		LED1(0);//LED1_ON();
		vTaskDelay(500);
		LED1(1);//LED1_OFF();
		vTaskDelay(500);
	}
	
	
}



#if 1
void LED2_Task(void *para)
{

	while(1)
	{
		LED2(0);//LED2_ON();
		vTaskDelay(500);
		LED2(1);//LED2_OFF();
		vTaskDelay(500);
	}
	
	
}

#endif

/*********************************************END OF FILE**********************/
