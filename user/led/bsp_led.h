#ifndef __LED_H
#define __LED_H


#include "stm32f10x.h"

/*
*@mike,2020.01.12
*
*led1:gipoc->pin2
*led1:gipoc->pin3
*/




/* ����LED���ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����Ĵ��뼴�ɸı���Ƶ�LED���� */
#define LED1_GPIO_PORT    	GPIOC		              /* GPIO�˿� */
#define LED1_GPIO_CLK 	    RCC_APB2Periph_GPIOC		/* GPIO�˿�ʱ�� */
#define LED1_GPIO_PIN			GPIO_Pin_2			        

#define LED2_GPIO_PORT    	GPIOC			              /* GPIO�˿� */
#define LED2_GPIO_CLK 	    RCC_APB2Periph_GPIOC		/* GPIO�˿�ʱ�� */
#define LED2_GPIO_PIN		GPIO_Pin_3			        



/** the macro definition to trigger the led on or off 
  * 1 - off
  *0 - on
  */
#define ON  0
#define OFF 1

/* ʹ�ñ�׼�Ĺ̼������IO*/
#define LED1(a)	if (a)	\
					GPIO_SetBits(LED1_GPIO_PORT,LED1_GPIO_PIN);\
					else		\
					GPIO_ResetBits(LED1_GPIO_PORT,LED1_GPIO_PIN)

#define LED2(a)	if (a)	\
					GPIO_SetBits(LED2_GPIO_PORT,LED2_GPIO_PIN);\
					else		\
					GPIO_ResetBits(LED2_GPIO_PORT,LED2_GPIO_PIN)




/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define	digitalHi(p,i)		 {p->BSRR=i;}	 //���Ϊ�ߵ�ƽ		
#define digitalLo(p,i)		 {p->BRR=i;}	 //����͵�ƽ
#define digitalToggle(p,i) {p->ODR ^=i;} //�����ת״̬


/* �������IO�ĺ� */
#define LED1_TOGGLE()		 digitalToggle(LED1_GPIO_PORT,LED1_GPIO_PIN)
#define LED1_OFF()		   digitalHi(LED1_GPIO_PORT,LED1_GPIO_PIN)
#define LED1_ON()			   digitalLo(LED1_GPIO_PORT,LED1_GPIO_PIN)

#define LED2_TOGGLE()		 digitalToggle(LED2_GPIO_PORT,LED2_GPIO_PIN)
#define LED2_OFF()		   digitalHi(LED2_GPIO_PORT,LED2_GPIO_PIN)
#define LED2_ON()			   digitalLo(LED2_GPIO_PORT,LED2_GPIO_PIN)







//���Ŷ���
#define KEY1_INT_GPIO_PORT         GPIOA
#define KEY1_INT_GPIO_CLK          (RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO)
#define KEY1_INT_GPIO_PIN          GPIO_Pin_0
#define KEY1_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOA
#define KEY1_INT_EXTI_PINSOURCE    GPIO_PinSource0
#define KEY1_INT_EXTI_LINE         EXTI_Line0
#define KEY1_INT_EXTI_IRQ          EXTI0_IRQn

#define KEY1_IRQHandler            EXTI0_IRQHandler


#define KEY2_INT_GPIO_PORT         GPIOC
#define KEY2_INT_GPIO_CLK          (RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO)
#define KEY2_INT_GPIO_PIN          GPIO_Pin_13
#define KEY2_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOC
#define KEY2_INT_EXTI_PINSOURCE    GPIO_PinSource13
#define KEY2_INT_EXTI_LINE         EXTI_Line13
#define KEY2_INT_EXTI_IRQ          EXTI15_10_IRQn

#define KEY2_IRQHandler            EXTI15_10_IRQHandler

void EXTI_Key_Config(void);















extern void LED_GPIO_Config(void);
extern void LED1_Task(void *para);
extern void LED2_Task(void *para);
#endif /* __LED_H__ */
