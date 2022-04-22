#include <stdio.h>
#include <stdlib.h>
#include "stm32f10x.h"
#include "FreeRTOS.h"	
#include "task.h"
#include "queue.h"
#include "semphr.h"	
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_dht11.h"

#include "small_text_protocol/stp.h"
#include "CRC16/CRC16.h"
#include "flash/bsp_spi_flash.h"
#include "nRF24L01/24L01.h"




#include "i2c/bsp_i2c_ee.h"
#include "../library/GUI/lvgl/lvgl.h"
#include "../library/GUI/lv_port_disp.h"
#include "../library/GUI/lv_port_indev.h"


#include "rtc/bsp_rtc.h"

#include "GUI_APP/gui_app.h"
#include "sensors_APP/sensors_app.h"



void delay(uint32_t n)
{
	while(n)
	{
	  __nop();
		n--;
	}
}
void SysTick_Init(void)
{
	/* SystemFrequency / 1000    1ms中断一次
	 * SystemFrequency / 100000	 10us中断一次
	 * SystemFrequency / 1000000 1us中断一次
	 */
//	if (SysTick_Config(SystemFrequency / 100000))	// ST3.0.0库版本
	if (SysTick_Config(SystemCoreClock / 1000000))	// ST3.5.0库版本
	{ 
		/* Capture error */ 
		while (1);
	}
		// 关闭滴答定时器  
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
}



static TaskHandle_t LED1_Task_Handle = NULL;
static TaskHandle_t LED2_Task_Handle = NULL;
static TaskHandle_t DHT11_Task_Handle = NULL;
static TaskHandle_t OLED_thread_Handle = NULL;

static TaskHandle_t sensors_thread_Handle = NULL;


static TaskHandle_t stp_thread_Handle = NULL;


typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;


/* 获取缓冲区的长度 */
#define TxBufferSize1   (countof(TxBuffer1) - 1)
#define RxBufferSize1   (countof(TxBuffer1) - 1)
#define countof(a)      (sizeof(a) / sizeof(*(a)))
#define  BufferSize (countof(Tx_Buffer)-1)


/* 发送缓冲区初始化 */
uint8_t Tx_Buffer[] = "感谢您选用野火stm32开发板\r\n";
uint8_t Rx_Buffer[BufferSize];
__IO uint32_t DeviceID = 0;
__IO uint32_t FlashID = 0;
__IO TestStatus TransferStatus1 = FAILED;

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


void xxxx_delay_ms(uint32_t n)
{
   uint32_t i,j;
	while(n)
	{
   
		for(i=0;i<1000;i++)
         for(j=0;j<1000;j++);
    n--;
	}

}

int main ( void )
{
	
	char *ppp = NULL;
	BaseType_t xReturn =pdPASS;
	

//==========================================
//==========================================
#if 0
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	USART_Config();
	printf("Hello flash123\r");
			/* 8M串行flash W25Q64初始化 */
	SPI_FLASH_Init();
	NRF24L01_Init();    //初始化NRF24L01 
	/* 获取 Flash Device ID */
	

	DeviceID = SPI_FLASH_ReadDeviceID();	
	printf("DeviceID is %x\n",DeviceID);
	
	xxxx_delay_ms(5);
		/* 获取 SPI Flash ID */
	//FlashID = SPI_FLASH_ReadID();	
	//printf("\r\n FlashID is 0x%X,Manufacturer Device ID is 0x%X\r\n", FlashID, DeviceID);
	
	
	SPI_CPOL_CPHA_nRF24L01_Init();
	if(NRF24L01_Check())
	{
		printf("24L01 error\r\n");
	
	}
	else
	{
	  printf("24L01 there\r\n");
	}
	
	
	uint8_t tmp_buf[64]={0}; 

//=====================================================================
#if 0	
	for(uint8_t i=0;i<32;i++)
	{
		tmp_buf[i]=0x55;//调试的时候，可以用此帧数据测试发送与接收情况
	}
	TX_Mode();
	
	while(1)
	{
	
	   if(NRF24L01_TxPacket(tmp_buf)==TX_OK)
		 {
		 
		 }
	 xxxx_delay_ms(20);
	
	}
	
	
#endif
//======================================================================	
	
	
	
	
//======================================================================
#if 0
	RX_Mode();	
	
	while(1)
	{
	
	
			if(NRF24L01_RxPacket(tmp_buf)==0)//一旦接收到信息,则显示出来.
				{
				  tmp_buf[32]=0;//加入字符串结束符

			    printf("rx is %s\r\n",tmp_buf);
					
				}
				
	    xxxx_delay_ms(20);
	}
	
	
#endif
//=======================================================================	
	
	
	
	
	
	
#endif
//=============================================================
//=============================================================
	
	
	
	
	
	
	
	
	DHT11_Init ();
	BASIC_TIM_Init();
	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	LED_GPIO_Config();
	EXTI_Key_Config();
	//SysTick_Init();
	USART_Config();
	
	init_RTC();
	
	
	init_stp_frame(&stp_frame_test,stp_frame_buf);

	ppp = malloc(8);
	if(ppp == NULL)
		printf("malloc error\r\n");
	else
		printf("malloc successfully\r\n");
	
	free(ppp);
	
	
	i2c_CfgGpio();
	
	Initial_LY096BG30();
	
	fill_picture(0x00);
	//fill_picture(0xff);
	
	//while(1);
	
	
	
	lv_init();
	lv_port_disp_init();
	lv_port_indev_init();

	
	//lv_obj_get_disp(const lv_obj_t * obj)
	//lv_theme_mono_init(lv_disp_t * disp, bool dark_bg, const lv_font_t * font);
	
	//lv_theme_mono_init(NULL, true, &lv_font_montserrat_14);
	
	//lv_wtest();
	//lv_mytest();
	

	printf("Hello FreeRTOS!\n");
	
	
	
	//======================================================================
	
	
#if 1
	
	
#define  FLASH_WriteAddress     0x00000
#define  FLASH_ReadAddress      FLASH_WriteAddress
#define  FLASH_SectorToErase    FLASH_WriteAddress
	
	
	SPI_CPOL_CPHA_nRF24L01_Init();
		/* 8M串行flash W25Q64初始化 */
	SPI_FLASH_Init();
	
	/* 获取 Flash Device ID */
	
	
	//u16 xid = SPI_Flash_ReadID();
	//printf("xid is %x\n",xid);
	//while(1);
	
	DeviceID = SPI_FLASH_ReadDeviceID();	
	delay( 200 );
	
	/* 获取 SPI Flash ID */
	FlashID = SPI_FLASH_ReadID();	
	printf("\r\n FlashID is 0x%X,Manufacturer Device ID is 0x%X\r\n", FlashID, DeviceID);

	/* 检验 SPI Flash ID */
	if(1)//(FlashID == sFLASH_ID)
	{	
		printf("\r\n 检测到串行flash W25Q64 !\r\n");
		
		/* 擦除将要写入的 SPI FLASH 扇区，FLASH写入前要先擦除 */
		// 这里擦除4K，即一个扇区，擦除的最小单位是扇区
		SPI_FLASH_SectorErase(FLASH_SectorToErase);	 	 
		
		/* 将发送缓冲区的数据写到flash中 */
		// 这里写一页，一页的大小为256个字节
		SPI_FLASH_BufferWrite(Tx_Buffer, FLASH_WriteAddress, BufferSize);		
		printf("\r\n 写入的数据为：%s \r\t", Tx_Buffer);
		
		/* 将刚刚写入的数据读出来放到接收缓冲区中 */
		SPI_FLASH_BufferRead(Rx_Buffer, FLASH_ReadAddress, BufferSize);
		printf("\r\n 读出的数据为：%s \r\n", Rx_Buffer);
		
		/* 检查写入的数据与读出的数据是否相等 */
		TransferStatus1 = Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);
		
		if( PASSED == TransferStatus1 )
		{ 
			//LED2_ON;
			printf("\r\n 8M串行flash(W25Q64)测试成功!\n\r");
		}
		else
		{        
			//LED1_ON;
			printf("\r\n 8M串行flash(W25Q64)测试失败!\n\r");
		}
	}// if (FlashID == sFLASH_ID)
	else// if (FlashID == sFLASH_ID)
	{ 
		//LED1_ON;
		printf("\r\n 获取不到 W25Q64 ID!\n\r");
	}
	
	
	
	#endif
	
	//=======================================================================
	
	
	
	
	SPI_CPOL_CPHA_nRF24L01_Init();
	NRF24L01_Init();    //初始化NRF24L01 
	if(NRF24L01_Check())
	{
		printf("24L01 error\r\n");
	
	}
	else
	{
	  printf("24L01 there\r\n");
	}
	
	
	
	
	
	
	
	
	
	
	
	
	#if 1
	xReturn = xTaskCreate((TaskFunction_t)LED1_Task,
	                      (const char*)"LED1_Task",
		                    (uint16_t)512,
	                      (void*)NULL,
		                    (UBaseType_t)2,
												(TaskHandle_t*)&LED1_Task_Handle);

												
												
												
												
												
	//xReturn = xTaskCreate((TaskFunction_t)LED2_Task,
	 //                     (const char*)"LED2_Task",
	//											(uint16_t)512,
	//											(void*)NULL,
	//											(UBaseType_t)2,
	//											(TaskHandle_t*)&LED1_Task_Handle);
												
												
					


													
					
				

xReturn = xTaskCreate((TaskFunction_t)OLED_thread,
	                      (const char*)"OLED_thread",
												(uint16_t)512,
												(void*)NULL,
												(UBaseType_t)2,
												(TaskHandle_t*)&OLED_thread_Handle);



	



				
												
xReturn = xTaskCreate((TaskFunction_t)sensors_thread,
	                      (const char*)"sensors_thread",
												(uint16_t)512,
												(void*)NULL,
												(UBaseType_t)2,
												(TaskHandle_t*)&sensors_thread_Handle);										
		


											
xReturn = xTaskCreate((TaskFunction_t)stp_thread,
	                      (const char*)"stp_thread",
												(uint16_t)512,
												(void*)NULL,
												(UBaseType_t)2,
												(TaskHandle_t*)&stp_thread_Handle);				









												
											
#endif	
						

												
												

												
	vTaskStartScheduler();
	while(1)
	{
		//LED1_TOGGLE();
		//delay(1000000);
		//printf("FreeRTOS  error\n");
	}

}
