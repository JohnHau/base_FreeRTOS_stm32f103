#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "bsp_dht11.h"
#include "bsp_usart.h"


DHT11_Data_TypeDef DHT11_Data;


QueueHandle_t uart1_Queue =NULL;

static void                           DHT11_GPIO_Config                       ( void );
static void                           DHT11_Mode_IPU                          ( void );
static void                           DHT11_Mode_Out_PP                       ( void );
static uint8_t                        DHT11_ReadByte                          ( void );

static void BASIC_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // �����ж���Ϊ0
    //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);		
		// �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = BASIC_TIM_IRQ ;	
		// ���������ȼ�Ϊ 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;	 
	  // ������ռ���ȼ�Ϊ3
    //NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

static void BASIC_TIM_Mode_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		
		// ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
    BASIC_TIM_APBxClock_FUN(BASIC_TIM_CLK, ENABLE);
	
		// �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
    TIM_TimeBaseStructure.TIM_Period = BASIC_TIM_Period;	

	  // ʱ��Ԥ��Ƶ��Ϊ
    TIM_TimeBaseStructure.TIM_Prescaler= BASIC_TIM_Prescaler;
	
		// ʱ�ӷ�Ƶ���� ��������ʱ��û�У����ù�
    //TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
		
		// ����������ģʽ��������ʱ��ֻ�����ϼ�����û�м���ģʽ������
    //TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
		
		// �ظ���������ֵ��������ʱ��û�У����ù�
		//TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	
	  // ��ʼ����ʱ��
    TIM_TimeBaseInit(BASIC_TIM, &TIM_TimeBaseStructure);
		
		// ����������жϱ�־λ
    TIM_ClearFlag(BASIC_TIM, TIM_FLAG_Update);
	  
		// �����������ж�
    //TIM_ITConfig(BASIC_TIM,TIM_IT_Update,ENABLE);
		
		// ʹ�ܼ�����
    //TIM_Cmd(BASIC_TIM, ENABLE);	
}

void BASIC_TIM_Init(void)
{
	BASIC_TIM_NVIC_Config();
	BASIC_TIM_Mode_Config();
}








void DHT11_delay_2us(uint32_t nus)
{

	uint32_t us=0;
	//TIM_SetCounter(BASIC_TIM, 0);
	TIM_Cmd(BASIC_TIM, ENABLE);	
	
	while(us < nus)
	{
		
		
		//if ( TIM_GetITStatus( TIM6, TIM_IT_Update) != RESET ) 
		{
			while(TIM_GetFlagStatus(TIM6, TIM_FLAG_Update) == RESET);
			us++;
			//printf("timer 6 - 11!\r\n");
		  TIM_ClearFlag(TIM6, TIM_FLAG_Update);
		  TIM_ClearITPendingBit(TIM6 , TIM_FLAG_Update);  
		}
	 	
  }
	
	TIM_Cmd(BASIC_TIM, DISABLE);	
	
}

void DHT11_delay_30us(uint32_t nus)
{


}

 /**
  * @brief  DHT11 ��ʼ������
  * @param  ��
  * @retval ��
  */
void DHT11_Init ( void )
{
	DHT11_GPIO_Config ();
	
	DHT11_Dout_1;               // ����GPIOB10
}


/*
 * ��������DHT11_GPIO_Config
 * ����  ������DHT11�õ���I/O��
 * ����  ����
 * ���  ����
 */
static void DHT11_GPIO_Config ( void )
{		
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure; 

	
	/*����DHT11_Dout_GPIO_PORT������ʱ��*/
  DHT11_Dout_SCK_APBxClock_FUN ( DHT11_Dout_GPIO_CLK, ENABLE );	
 
	/*ѡ��Ҫ���Ƶ�DHT11_Dout_GPIO_PORT����*/															   
  	GPIO_InitStructure.GPIO_Pin = DHT11_Dout_GPIO_PIN;	

	/*��������ģʽΪͨ���������*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

	/*������������Ϊ50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	/*���ÿ⺯������ʼ��DHT11_Dout_GPIO_PORT*/
  	GPIO_Init ( DHT11_Dout_GPIO_PORT, &GPIO_InitStructure );		  
	
}


/*
 * ��������DHT11_Mode_IPU
 * ����  ��ʹDHT11-DATA���ű�Ϊ��������ģʽ
 * ����  ����
 * ���  ����
 */
static void DHT11_Mode_IPU(void)
{
 	  GPIO_InitTypeDef GPIO_InitStructure;

	  	/*ѡ��Ҫ���Ƶ�DHT11_Dout_GPIO_PORT����*/	
	  GPIO_InitStructure.GPIO_Pin = DHT11_Dout_GPIO_PIN;

	   /*��������ģʽΪ��������ģʽ*/ 
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ; 

	  /*���ÿ⺯������ʼ��DHT11_Dout_GPIO_PORT*/
	  GPIO_Init(DHT11_Dout_GPIO_PORT, &GPIO_InitStructure);	 
	
}


/*
 * ��������DHT11_Mode_Out_PP
 * ����  ��ʹDHT11-DATA���ű�Ϊ�������ģʽ
 * ����  ����
 * ���  ����
 */
static void DHT11_Mode_Out_PP(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;

	 	/*ѡ��Ҫ���Ƶ�DHT11_Dout_GPIO_PORT����*/															   
  	GPIO_InitStructure.GPIO_Pin = DHT11_Dout_GPIO_PIN;	

	/*��������ģʽΪͨ���������*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

	/*������������Ϊ50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	/*���ÿ⺯������ʼ��DHT11_Dout_GPIO_PORT*/
  	GPIO_Init(DHT11_Dout_GPIO_PORT, &GPIO_InitStructure);	 	 
	
}


/* 
 * ��DHT11��ȡһ���ֽڣ�MSB����
 */
static uint8_t DHT11_ReadByte ( void )
{
	uint8_t i, temp=0;
	

	for(i=0;i<8;i++)    
	{	 
		/*ÿbit��50us�͵�ƽ���ÿ�ʼ����ѯֱ���ӻ����� ��50us �͵�ƽ ����*/  
		while(DHT11_Dout_IN()==Bit_RESET);

		/*DHT11 ��26~28us�ĸߵ�ƽ��ʾ��0������70us�ߵ�ƽ��ʾ��1����
		 *ͨ����� x us��ĵ�ƽ��������������״ ��x ���������ʱ 
		 */
		//Delay_us(40); //��ʱx us �����ʱ��Ҫ��������0������ʱ�伴��	   	  
    DHT11_delay_2us(20);
		if(DHT11_Dout_IN()==Bit_SET)/* x us����Ϊ�ߵ�ƽ��ʾ���ݡ�1�� */
		{
			/* �ȴ�����1�ĸߵ�ƽ���� */
			while(DHT11_Dout_IN()==Bit_SET);

			temp|=(uint8_t)(0x01<<(7-i));  //�ѵ�7-iλ��1��MSB���� 
		}
		else	 // x us��Ϊ�͵�ƽ��ʾ���ݡ�0��
		{			   
			temp&=(uint8_t)~(0x01<<(7-i)); //�ѵ�7-iλ��0��MSB����
		}
	}
	
	return temp;
	
}


/*
 * һ�����������ݴ���Ϊ40bit����λ�ȳ�
 * 8bit ʪ������ + 8bit ʪ��С�� + 8bit �¶����� + 8bit �¶�С�� + 8bit У��� 
 */
uint8_t DHT11_Read_TempAndHumidity(DHT11_Data_TypeDef *DHT11_Data)
{ 

	/*���ģʽ*/
	DHT11_Mode_Out_PP();
	/*��������*/
	DHT11_Dout_0;
	/*��ʱ18ms*/
	//Delay_us(18000);
	//portDISABLE_INTERRUPTS();	
  //DHT11_delay_2us(9000);
	vTaskDelay(18);
	//vTaskDelay(20);
  portDISABLE_INTERRUPTS();	
	/*�������� ������ʱ30us*/
	DHT11_Dout_1; 

	//Delay_us(30);   //��ʱ30us
  DHT11_delay_2us(15);
	//DHT11_delay_30us(300);
	/*������Ϊ���� �жϴӻ���Ӧ�ź�*/ 
	DHT11_Mode_IPU();	
	
	/*�жϴӻ��Ƿ��е͵�ƽ��Ӧ�ź� �粻��Ӧ����������Ӧ����������*/   
	if(DHT11_Dout_IN()==Bit_RESET)     
	{
		/*��ѯֱ���ӻ����� ��80us �͵�ƽ ��Ӧ�źŽ���*/  
		while(DHT11_Dout_IN()==Bit_RESET);

		/*��ѯֱ���ӻ������� 80us �ߵ�ƽ �����źŽ���*/
		while(DHT11_Dout_IN()==Bit_SET);

		/*��ʼ��������*/   
		DHT11_Data->humi_int= DHT11_ReadByte();

		DHT11_Data->humi_deci= DHT11_ReadByte();

		DHT11_Data->temp_int= DHT11_ReadByte();

		DHT11_Data->temp_deci= DHT11_ReadByte();

		DHT11_Data->check_sum= DHT11_ReadByte();


		/*��ȡ���������Ÿ�Ϊ���ģʽ*/
		DHT11_Mode_Out_PP();
		/*��������*/
		DHT11_Dout_1;
portENABLE_INTERRUPTS();
		/*����ȡ�������Ƿ���ȷ*/
		if(DHT11_Data->check_sum == DHT11_Data->humi_int + DHT11_Data->humi_deci + DHT11_Data->temp_int+ DHT11_Data->temp_deci)
		{
			return SUCCESS;
		}
		else
		{				
			return ERROR;
		}
		
	}
	
	else
	{
		printf("Low level,no response\r\n");
		return ERROR;
	}
}





uint8_t reverse_string(uint8_t* str)
{

  uint8_t i=0;
	uint8_t temp=0;
	uint8_t len = strlen((char*)str);
	for(i=0;i< len/2;i++)
	{	 
		temp = str[i];
		str[i] = str[len -1 -i];		
		str[len -1 -i] = temp;
	}
	
	
	
	return 0;

}



uint8_t dht11_itoa(uint8_t n,uint8_t*str,uint8_t base)
{
	uint8_t i=0;
	
	
	
	if(n == 0)
	{
	   str[0] = '0';
	   return 0;
	}
	
	
	
	
	while(n)
	{
		str[i++] = n%base + '0';
		n = n/base;

	}

	reverse_string(str);
	
	
	return 0;

}





uint8_t dht11_to_string(DHT11_Data_TypeDef *DHT11_Data,DHT11_RT type,uint8_t* rt_str)
{

	uint8_t istr[4]={0};
	uint8_t fstr[4]={0};
	uint8_t vstr[8]={0};
	
	
	if(type == DHT11_T)//read temperature
	{
		dht11_itoa(DHT11_Data->temp_int, istr,10);
		dht11_itoa(DHT11_Data->temp_deci, fstr,10);
	
	}
	else if(type == DHT11_R)//read humidity
	{
	
		dht11_itoa(DHT11_Data->humi_int, istr,10);
		dht11_itoa(DHT11_Data->humi_deci, fstr,10);
		
	}


	strcat((char*)vstr,(char*)istr);
	strcat((char*)vstr,".");
	strcat((char*)vstr,(char*)fstr);	
	strcpy((char*)rt_str,(char*)vstr);
	
	
	return 0;
}



/*************************************END OF FILE******************************/
