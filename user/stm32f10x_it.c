/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTI
  
  AL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f10x_it.h"
#include "FreeRTOS.h"					//FreeRTOS使用		  
#include "task.h" 
#include "queue.h"
#include "semphr.h"	

#include "./dht11/bsp_dht11.h"
#include "small_text_protocol/stp.h"
#include "./usart/bsp_usart.h"
#include "./led/bsp_led.h"


/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
#if 0
void SVC_Handler(void)
{
}
#endif
/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
#if 0
void PendSV_Handler(void)
{
}
#endif
/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */

static uint32_t TimingDelay;

void Delay_us(uint32_t nTime)
{ 
	TimingDelay = nTime;	

	// 使能滴答定时器  
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;

	while(TimingDelay != 0);
}
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
}
extern void xPortSysTickHandler(void);
void SysTick_Handler(void)
{
	
	
	#if 0
	TimingDelay_Decrement();	
	#endif
	
	
	#if 1
	  #if (INCLUDE_xTaskGetSchedulerState  == 1 )
      if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
      {
    #endif  /* INCLUDE_xTaskGetSchedulerState */  
        xPortSysTickHandler();
    #if (INCLUDE_xTaskGetSchedulerState  == 1 )
      }
    #endif  /* INCLUDE_xTaskGetSchedulerState */
	#endif
}




void uart1_DMA_rx_data(void)
{
	BaseType_t pxHigherPriorityTaskWoken;
	//shut DMA for disturbing
	DMA_Cmd(DMA1_Channel5,DISABLE);
	
	//clear DMA flags
	DMA_ClearFlag(DMA1_FLAG_TC5);
	
	DMA1_Channel5->CNDTR = SIZE_BUFFER_UART1;
	//DMA1_Channel5->CNDTR = 5;
	
	//enalble DMA
	DMA_Cmd(DMA1_Channel5,ENABLE);
	
	
	xSemaphoreGiveFromISR(BinarySem_Handle, &pxHigherPriorityTaskWoken);
	
	portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);

}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
void USART1_IRQHandler(void)
{
	
#if 1
	static uint8_t i=0;
	uint8_t ch_rx_uart1=0;
	uint32_t send_data = 0x11111111;
	BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
	BaseType_t  xHigherPriorityTaskWoken;
	BaseType_t pxHigherPriorityTaskWoken;
	//printf("uart1 hi there\r\n");
	
	if(USART_GetITStatus(DEBUG_USARTx , USART_IT_RXNE) != RESET)	 //检查指定的USART中断发生与否
  {
    
		
		ch_rx_uart1=USART_ReceiveData(DEBUG_USARTx ); //读取接收缓冲区数据。    /* Read one byte from the receive data register */
		buffer_rx_uart1[i++] = ch_rx_uart1; 
      
		
		//printf("c is %c\r\n",buffer_rx_uart1[i-1]);
		  //if(i >=64)i=0;
		if(buffer_rx_uart1[i-1] == '\n' || buffer_rx_uart1[i-1] == '\r')
		{
			
			printf("str is %s",buffer_rx_uart1);
			printf("c is %d\n",buffer_rx_uart1[0]);
			printf("c is %d\n",buffer_rx_uart1[1]);
			printf("c is %d\n",buffer_rx_uart1[2]);
			printf("c is %d\n",buffer_rx_uart1[3]);
			printf("c is %d\n",buffer_rx_uart1[4]);
			printf("c is %d\n",buffer_rx_uart1[5]);
			printf("c is %d\n",buffer_rx_uart1[6]);
			
			if(strcmp("hello\r",(char*)buffer_rx_uart1) == 0)
			{
			    printf("world\n");
			}
				if(strcmp("exit\r",(char*)buffer_rx_uart1) == 0)
			{
			    printf("zzzz\n");
			}
			
			memset(buffer_rx_uart1,0,64);
			i=0;
			//printf("%c is received!\r\n",ch_rx_uart1);
		}
		//printf("%c is received!\r\n",ch_rx_uart1);
		//xSemaphoreGiveFromISR(BinarySem_Handle, &pxHigherPriorityTaskWoken);
	
	  //portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
		
#if 0
		//xReturn=xQueueSendFromISR(uart1_Queue,&send_data,&xHigherPriorityTaskWoken);
		xReturn=xQueueSendFromISR(uart1_Queue,&ch_rx_uart1,&xHigherPriorityTaskWoken);
		if(pdPASS == xReturn)
		{
        printf("data sent\r\n");
		}
		else
		{
			printf("Error: send data\r\n");
		}
		
#endif
		
		
		
  }

#endif
	
	
	
#if 0
	uint32_t ulReturn;
	ulReturn = taskENTER_CRITICAL_FROM_ISR();
	printf("usart1 isr:\r\n");
	if(USART_GetITStatus(DEBUG_USARTx,USART_IT_IDLE) != RESET)
	{
		printf("usart1 isr:idle\r\n");
		uart1_DMA_rx_data();// release sema
		USART_ReceiveData(DEBUG_USARTx);//clear flags
		
	}
	
	taskEXIT_CRITICAL_FROM_ISR(ulReturn);
#endif
}



uint32_t timer =0;
void  TIM6_IRQHandler (void)
{
	if ( TIM_GetITStatus( TIM6, TIM_IT_Update) != RESET ) 
	{	
		timer++;
		if(timer > 100000)
		{
			printf("timer\r\n");
			timer = 0;
		}
			TIM_ClearITPendingBit(TIM6 , TIM_FLAG_Update);  		 
	}		 	
}




void USART3_IRQHandler(void)
{
	 uint8_t ucTemp;
	if(USART_GetITStatus(DEBUG_USARTx,USART_IT_RXNE)!=RESET)
	{		
		ucTemp = USART_ReceiveData(DEBUG_USARTx);
    //USART_SendData(DEBUG_USARTx,ucTemp);    
		
		stp_state_machine(&stp_frame_test,ucTemp);

	}	 

}








void KEY1_IRQHandler(void)
{
  //确保是否产生了EXTI Line中断
	if(EXTI_GetITStatus(KEY1_INT_EXTI_LINE) != RESET) 
	{
		// LED1 取反		
		//LED1_TOGGLE;
    //清除中断标志位
		
		printf("key1\r\n");
		EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE);     
	}  
}

void KEY2_IRQHandler(void)
{
  //确保是否产生了EXTI Line中断
	if(EXTI_GetITStatus(KEY2_INT_EXTI_LINE) != RESET) 
	{
		// LED2 取反		
		//LED2_TOGGLE;
    //清除中断标志位
		printf("key2\r\n");
		EXTI_ClearITPendingBit(KEY2_INT_EXTI_LINE);     
	}  
}














/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
