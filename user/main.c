#include <stdio.h>
#include "stm32f10x.h"
#include "FreeRTOS.h"	
#include "task.h"
#include "queue.h"
#include "bsp_led.h"
#include "bsp_usart.h"
void delay(uint32_t n)
{
	while(n)
	{
	  __nop();
		n--;
	}
}

static TaskHandle_t LED1_Task_Handle = NULL;
static TaskHandle_t LED2_Task_Handle = NULL;

int main ( void )
{
	BaseType_t xReturn =pdPASS;
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
	LED_GPIO_Config();
	USART_Config();

	printf("Hello FreeRTOS!\n");
	xReturn = xTaskCreate((TaskFunction_t)LED1_Task,
	                      (const char*)"LED1_Task",
		                    (uint16_t)512,
	                      (void*)NULL,
		                    (UBaseType_t)2,
	                      (TaskHandle_t*)&LED1_Task_Handle);

	xReturn = xTaskCreate((TaskFunction_t)LED2_Task,
	                      (const char*)"LED2_Task",
												(uint16_t)512,
												(void*)NULL,
												(UBaseType_t)2,
												(TaskHandle_t*)&LED1_Task_Handle);
									 
	vTaskStartScheduler();
	while(1)
	{
		//LED1_TOGGLE();
		//delay(1000000);
		//printf("FreeRTOS  error\n");
	}

}
