#ifndef __DHT11_H
#define	__DHT11_H



#include "stm32f10x.h"


/********************基本定时器TIM参数定义，只限TIM6、7************/
#define BASIC_TIM6 // 如果使用TIM7，注释掉这个宏即可

#ifdef  BASIC_TIM6 // 使用基本定时器TIM6
#define            BASIC_TIM                   TIM6
#define            BASIC_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            BASIC_TIM_CLK               RCC_APB1Periph_TIM6
#define            BASIC_TIM_Period            2-1
#define            BASIC_TIM_Prescaler         71
#define            BASIC_TIM_IRQ               TIM6_IRQn
#define            BASIC_TIM_IRQHandler        TIM6_IRQHandler

#else  // 使用基本定时器TIM7
#define            BASIC_TIM                   TIM7
#define            BASIC_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            BASIC_TIM_CLK               RCC_APB1Periph_TIM7
#define            BASIC_TIM_Period            1000-1
#define            BASIC_TIM_Prescaler         71
#define            BASIC_TIM_IRQ               TIM7_IRQn
#define            BASIC_TIM_IRQHandler        TIM7_IRQHandler

#endif












/************************** DHT11 数据类型定义********************************/
typedef struct
{
	uint8_t  humi_int;		//湿度的整数部分
	uint8_t  humi_deci;	 	//湿度的小数部分
	uint8_t  temp_int;	 	//温度的整数部分
	uint8_t  temp_deci;	 	//温度的小数部分
	uint8_t  check_sum;	 	//校验和
		                 
} DHT11_Data_TypeDef;



/************************** DHT11 连接引脚定义********************************/
#define      DHT11_Dout_SCK_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define      DHT11_Dout_GPIO_CLK                       RCC_APB2Periph_GPIOC

#define      DHT11_Dout_GPIO_PORT                      GPIOC
#define      DHT11_Dout_GPIO_PIN                       GPIO_Pin_0



/************************** DHT11 函数宏定义********************************/
#define      DHT11_Dout_0	                            GPIO_ResetBits ( DHT11_Dout_GPIO_PORT, DHT11_Dout_GPIO_PIN ) 
#define      DHT11_Dout_1	                            GPIO_SetBits ( DHT11_Dout_GPIO_PORT, DHT11_Dout_GPIO_PIN ) 

#define      DHT11_Dout_IN()	                          GPIO_ReadInputDataBit ( DHT11_Dout_GPIO_PORT, DHT11_Dout_GPIO_PIN ) 



/************************** DHT11 函数声明 ********************************/
void                     DHT11_Init                      ( void );
uint8_t                  DHT11_Read_TempAndHumidity      ( DHT11_Data_TypeDef * DHT11_Data );

extern void DHT11_delay_2us(uint32_t nus);
extern void DHT11_Task(void *para);
extern void Delay_us(uint32_t nTime);

extern void BASIC_TIM_Init(void);


extern QueueHandle_t uart1_Queue;

#endif /* __DHT11_H */







