 /**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   华邦 8M串行flash测试，并将测试信息通过串口1在电脑的超级终端中打印出来
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 F103-MINI STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
#include "stm32f10x.h"
#include "./usart/bsp_usart.h"
#include "./led/bsp_led.h"
#include "./i2c/bsp_i2c_ee.h"

#include "./systick/bsp_SysTick.h"
#include "./dht11/bsp_dht11.h"
#include "./rtc/bsp_rtc.h"

#include "./flash/bsp_spi_flash.h"


typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;

/* 获取缓冲区的长度 */
#define TxBufferSize1   (countof(TxBuffer1) - 1)
#define RxBufferSize1   (countof(TxBuffer1) - 1)
#define countof(a)      (sizeof(a) / sizeof(*(a)))
#define  BufferSize (countof(Tx_Buffer)-1)

#define  FLASH_WriteAddress     0x00000
#define  FLASH_ReadAddress      FLASH_WriteAddress
#define  FLASH_SectorToErase    FLASH_WriteAddress

     

/* 发送缓冲区初始化 */
uint8_t Tx_Buffer[] = "感谢您选用野火stm32开发板\r\n";
uint8_t Rx_Buffer[BufferSize];

__IO uint32_t DeviceID = 0;
__IO uint32_t FlashID = 0;
__IO TestStatus TransferStatus1 = FAILED;

// 函数原型声明
void Delay(__IO uint32_t nCount);
TestStatus Buffercmp(uint8_t* pBuffer1,uint8_t* pBuffer2, uint16_t BufferLength);

/*
 * 函数名：main
 * 描述  ：主函数
 * 输入  ：无
 * 输出  ：无
 */
 
 
 int8_t tstr[]="hello";	
 
 
 uint32_t rt_cnt=0;
 
 
 
 
 
 // N = 2^32/365/24/60/60 = 136 年

/*时间结构体，默认时间2000-01-01 00:00:00*/
struct rtc_time systmtime=
{
0,0,0,1,1,2000,0
};

extern __IO uint32_t TimeDisplay ;

 
 
 
 
 
 
 
int main(void)
{ 	
	
		DHT11_Data_TypeDef DHT11_Data;
	LED_GPIO_Config();
	
	/* 配置串口为：115200 8-N-1 */
	USART_Config();
	//printf("\r\n 这是一个8Mbyte串行flash(W25Q64)实验 \r\n");
	printf("\r\nOLED DHT11 USART W25Q64\r\n");
	//=================================================================
#if  1
	i2c_CfgGpio();
	
	
		Initial_LY096BG30();
	
	
	
	fill_picture(0x00);
	oled_096_print(0, 0, tstr);
	oled_096_print(1, 1, tstr);
	oled_096_print(2, 2, tstr);
	
	oled_096_print(3, 3, tstr);
	oled_096_print(4, 4, tstr);
	oled_096_print(5, 5, tstr);
	
	oled_096_print(6, 6, tstr);
	oled_096_print(6, 7, tstr);
	
	
	/* 配置SysTick 为1us中断一次 */
	SysTick_Init();
	
		/*初始化DTT11的引脚*/
	DHT11_Init();
	
	
	

	
	
#endif
//===================================================================================	
	
	
	
			/* 配置RTC秒中断优先级 */
	  RTC_NVIC_Config();
	  RTC_CheckAndConfig(&systmtime);
	
	

	
	
	/* 8M串行flash W25Q64初始化 */
	SPI_FLASH_Init();
	
	/* 获取 Flash Device ID */
	DeviceID = SPI_FLASH_ReadDeviceID();	
	Delay( 200 );
	
	/* 获取 SPI Flash ID */
	FlashID = SPI_FLASH_ReadID();	
	printf("\r\n FlashID is 0x%X,\
	Manufacturer Device ID is 0x%X\r\n", FlashID, DeviceID);
	
	/* 检验 SPI Flash ID */
	if (FlashID == sFLASH_ID)
	{	
		printf("\r\n get W25Q64\r\n");
		
		/* 擦除将要写入的 SPI FLASH 扇区，FLASH写入前要先擦除 */
		// 这里擦除4K，即一个扇区，擦除的最小单位是扇区
		SPI_FLASH_SectorErase(FLASH_SectorToErase);	 	 
		
		/* 将发送缓冲区的数据写到flash中 */
		// 这里写一页，一页的大小为256个字节
		SPI_FLASH_BufferWrite(Tx_Buffer, FLASH_WriteAddress, BufferSize);		
		printf("\r\n write data %s \r\t", Tx_Buffer);
		
		/* 将刚刚写入的数据读出来放到接收缓冲区中 */
		SPI_FLASH_BufferRead(Rx_Buffer, FLASH_ReadAddress, BufferSize);
		printf("\r\n read data %s \r\n", Rx_Buffer);
		
		/* 检查写入的数据与读出的数据是否相等 */
		TransferStatus1 = Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);
		
		if( PASSED == TransferStatus1 )
		{ 
			//LED2_ON;
			printf("\r\n test W25Q64 ok\n\r");
		}
		else
		{        
			//LED1_ON;
			printf("\r\n test W25Q64 error\n\r");
		}
	}// if (FlashID == sFLASH_ID)
	else// if (FlashID == sFLASH_ID)
	{ 
		LED1_ON;
		printf("\r\ncan not get W25Q64 ID!\n\r");
	}
	
	while(1)
	{	
		
		  /* 每过1s 更新一次时间*/
	    if (TimeDisplay == 1)
			//if(1)
	    {
				/* 当前时间 */
	      Time_Display( RTC_GetCounter(),&systmtime); 		  
	      TimeDisplay = 0;
				
				
				
				
				
				rt_cnt++;
				
				if(rt_cnt%5 ==0)
				{
				
							/*调用DHT11_Read_TempAndHumidity读取温湿度，若成功则输出该信息*/
							if( DHT11_Read_TempAndHumidity ( & DHT11_Data ) == SUCCESS)
							{
								printf("\r\nread dht11 ok \r\n\r\nRH is %d.%d ％RH, T is %d.%d℃ \r\n",\
								DHT11_Data.humi_int,DHT11_Data.humi_deci,DHT11_Data.temp_int,DHT11_Data.temp_deci);
							}			
							else
							{
								printf("Read DHT11 ERROR!\r\n");
							}
				
				
				}
				

				
	    }
			
			

			
			
			
			
			
			
			
			
			
			//按下按键，通过串口修改时间
			//if( Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON  )
			if(0)
			{
				struct rtc_time set_time;

				/*使用串口接收设置的时间，输入数字时注意末尾要加回车*/
				Time_Regulate_Get(&set_time);
				/*用接收到的时间设置RTC*/
				Time_Adjust(&set_time);
				
				//向备份寄存器写入标志
				BKP_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);

			} 
		
		
			
			
#if 0
			
	/*调用DHT11_Read_TempAndHumidity读取温湿度，若成功则输出该信息*/
			if( DHT11_Read_TempAndHumidity ( & DHT11_Data ) == SUCCESS)
			{
				printf("\r\nread dht11 ok \r\n\r\nRH is %d.%d ％RH, T is %d.%d℃ \r\n",\
				DHT11_Data.humi_int,DHT11_Data.humi_deci,DHT11_Data.temp_int,DHT11_Data.temp_deci);
			}			
			else
			{
				printf("Read DHT11 ERROR!\r\n");
			}

		 Delay_ms(5000);
			
#endif
			
			
			
	}
}

/*
 * 函数名：Buffercmp
 * 描述  ：比较两个缓冲区中的数据是否相等
 * 输入  ：-pBuffer1     src缓冲区指针
 *         -pBuffer2     dst缓冲区指针
 *         -BufferLength 缓冲区长度
 * 输出  ：无
 * 返回  ：-PASSED pBuffer1 等于   pBuffer2
 *         -FAILED pBuffer1 不同于 pBuffer2
 */
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while(BufferLength--)
  {
    if(*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }

    pBuffer1++;
    pBuffer2++;
  }
  return PASSED;
}

void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}
/*********************************************END OF FILE**********************/
