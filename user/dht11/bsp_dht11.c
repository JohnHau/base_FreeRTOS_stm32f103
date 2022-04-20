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
    // 设置中断组为0
    //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);		
		// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = BASIC_TIM_IRQ ;	
		// 设置主优先级为 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;	 
	  // 设置抢占优先级为3
    //NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

static void BASIC_TIM_Mode_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		
		// 开启定时器时钟,即内部时钟CK_INT=72M
    BASIC_TIM_APBxClock_FUN(BASIC_TIM_CLK, ENABLE);
	
		// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
    TIM_TimeBaseStructure.TIM_Period = BASIC_TIM_Period;	

	  // 时钟预分频数为
    TIM_TimeBaseStructure.TIM_Prescaler= BASIC_TIM_Prescaler;
	
		// 时钟分频因子 ，基本定时器没有，不用管
    //TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
		
		// 计数器计数模式，基本定时器只能向上计数，没有计数模式的设置
    //TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
		
		// 重复计数器的值，基本定时器没有，不用管
		//TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	
	  // 初始化定时器
    TIM_TimeBaseInit(BASIC_TIM, &TIM_TimeBaseStructure);
		
		// 清除计数器中断标志位
    TIM_ClearFlag(BASIC_TIM, TIM_FLAG_Update);
	  
		// 开启计数器中断
    //TIM_ITConfig(BASIC_TIM,TIM_IT_Update,ENABLE);
		
		// 使能计数器
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
  * @brief  DHT11 初始化函数
  * @param  无
  * @retval 无
  */
void DHT11_Init ( void )
{
	DHT11_GPIO_Config ();
	
	DHT11_Dout_1;               // 拉高GPIOB10
}


/*
 * 函数名：DHT11_GPIO_Config
 * 描述  ：配置DHT11用到的I/O口
 * 输入  ：无
 * 输出  ：无
 */
static void DHT11_GPIO_Config ( void )
{		
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure; 

	
	/*开启DHT11_Dout_GPIO_PORT的外设时钟*/
  DHT11_Dout_SCK_APBxClock_FUN ( DHT11_Dout_GPIO_CLK, ENABLE );	
 
	/*选择要控制的DHT11_Dout_GPIO_PORT引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = DHT11_Dout_GPIO_PIN;	

	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	/*调用库函数，初始化DHT11_Dout_GPIO_PORT*/
  	GPIO_Init ( DHT11_Dout_GPIO_PORT, &GPIO_InitStructure );		  
	
}


/*
 * 函数名：DHT11_Mode_IPU
 * 描述  ：使DHT11-DATA引脚变为上拉输入模式
 * 输入  ：无
 * 输出  ：无
 */
static void DHT11_Mode_IPU(void)
{
 	  GPIO_InitTypeDef GPIO_InitStructure;

	  	/*选择要控制的DHT11_Dout_GPIO_PORT引脚*/	
	  GPIO_InitStructure.GPIO_Pin = DHT11_Dout_GPIO_PIN;

	   /*设置引脚模式为浮空输入模式*/ 
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ; 

	  /*调用库函数，初始化DHT11_Dout_GPIO_PORT*/
	  GPIO_Init(DHT11_Dout_GPIO_PORT, &GPIO_InitStructure);	 
	
}


/*
 * 函数名：DHT11_Mode_Out_PP
 * 描述  ：使DHT11-DATA引脚变为推挽输出模式
 * 输入  ：无
 * 输出  ：无
 */
static void DHT11_Mode_Out_PP(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;

	 	/*选择要控制的DHT11_Dout_GPIO_PORT引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = DHT11_Dout_GPIO_PIN;	

	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	/*调用库函数，初始化DHT11_Dout_GPIO_PORT*/
  	GPIO_Init(DHT11_Dout_GPIO_PORT, &GPIO_InitStructure);	 	 
	
}


/* 
 * 从DHT11读取一个字节，MSB先行
 */
static uint8_t DHT11_ReadByte ( void )
{
	uint8_t i, temp=0;
	

	for(i=0;i<8;i++)    
	{	 
		/*每bit以50us低电平标置开始，轮询直到从机发出 的50us 低电平 结束*/  
		while(DHT11_Dout_IN()==Bit_RESET);

		/*DHT11 以26~28us的高电平表示“0”，以70us高电平表示“1”，
		 *通过检测 x us后的电平即可区别这两个状 ，x 即下面的延时 
		 */
		//Delay_us(40); //延时x us 这个延时需要大于数据0持续的时间即可	   	  
    DHT11_delay_2us(20);
		if(DHT11_Dout_IN()==Bit_SET)/* x us后仍为高电平表示数据“1” */
		{
			/* 等待数据1的高电平结束 */
			while(DHT11_Dout_IN()==Bit_SET);

			temp|=(uint8_t)(0x01<<(7-i));  //把第7-i位置1，MSB先行 
		}
		else	 // x us后为低电平表示数据“0”
		{			   
			temp&=(uint8_t)~(0x01<<(7-i)); //把第7-i位置0，MSB先行
		}
	}
	
	return temp;
	
}


/*
 * 一次完整的数据传输为40bit，高位先出
 * 8bit 湿度整数 + 8bit 湿度小数 + 8bit 温度整数 + 8bit 温度小数 + 8bit 校验和 
 */
uint8_t DHT11_Read_TempAndHumidity(DHT11_Data_TypeDef *DHT11_Data)
{ 

	/*输出模式*/
	DHT11_Mode_Out_PP();
	/*主机拉低*/
	DHT11_Dout_0;
	/*延时18ms*/
	//Delay_us(18000);
	//portDISABLE_INTERRUPTS();	
  //DHT11_delay_2us(9000);
	vTaskDelay(18);
	//vTaskDelay(20);
  portDISABLE_INTERRUPTS();	
	/*总线拉高 主机延时30us*/
	DHT11_Dout_1; 

	//Delay_us(30);   //延时30us
  DHT11_delay_2us(15);
	//DHT11_delay_30us(300);
	/*主机设为输入 判断从机响应信号*/ 
	DHT11_Mode_IPU();	
	
	/*判断从机是否有低电平响应信号 如不响应则跳出，响应则向下运行*/   
	if(DHT11_Dout_IN()==Bit_RESET)     
	{
		/*轮询直到从机发出 的80us 低电平 响应信号结束*/  
		while(DHT11_Dout_IN()==Bit_RESET);

		/*轮询直到从机发出的 80us 高电平 标置信号结束*/
		while(DHT11_Dout_IN()==Bit_SET);

		/*开始接收数据*/   
		DHT11_Data->humi_int= DHT11_ReadByte();

		DHT11_Data->humi_deci= DHT11_ReadByte();

		DHT11_Data->temp_int= DHT11_ReadByte();

		DHT11_Data->temp_deci= DHT11_ReadByte();

		DHT11_Data->check_sum= DHT11_ReadByte();


		/*读取结束，引脚改为输出模式*/
		DHT11_Mode_Out_PP();
		/*主机拉高*/
		DHT11_Dout_1;
portENABLE_INTERRUPTS();
		/*检查读取的数据是否正确*/
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
