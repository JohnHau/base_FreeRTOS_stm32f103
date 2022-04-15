#include <stdio.h>
#include "stm32f10x.h"
#include "FreeRTOS.h"	
#include "task.h"
#include "queue.h"
#include "semphr.h"	
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_dht11.h"


#include "./CRC16/CRC16.h"
#include "./flash/bsp_spi_flash.h"
#include "./i2c/bsp_i2c_ee.h"
#include "../library/GUI/lvgl/lvgl.h"
#include "../library/GUI/lv_port_disp.h"
#include "../library/GUI/lv_port_indev.h"







 static void btn_event_cb(lv_event_t * e)
 {

     lv_event_code_t code = lv_event_get_code(e);
     lv_obj_t *btn = lv_event_get_target(e);
     if(code == LV_EVENT_CLICKED)
     {
         static uint8_t cnt =0;
         cnt ++;

         lv_obj_t *label = lv_obj_get_child(btn,0);
         lv_label_set_text_fmt(label,"Button: %d",cnt);

     }
 }
 
 
 
 static lv_style_t style_btn;
 
 void lv_wtest(void)
 {
     
	  lv_style_reset(&style_btn);
	  lv_style_init(&style_btn);
	 
	 lv_style_set_bg_opa(&style_btn,LV_OPA_100);
	 lv_style_set_border_color(&style_btn,lv_color_black());
	 lv_style_set_border_width(&style_btn,2);
	 
	 lv_style_set_text_color(&style_btn,lv_color_black());
	 
	 
	 
	   lv_obj_t * btn = lv_btn_create(lv_scr_act());
	 
	 lv_obj_add_style(btn,&style_btn,LV_STATE_DEFAULT);
	 
	 
     lv_obj_set_pos(btn,10,10);
     lv_obj_set_size(btn,80,20);

     //lv_obj_add_event_cb(btn,btn_event_cb,LV_EVENT_ALL,NULL);

     lv_obj_t *label = lv_label_create(btn);

	   lv_label_set_text_static(label,"Btn");
     lv_obj_center(label);

 }
 
 void lv_mytest(void)
 {
     lv_obj_t *label = lv_label_create(lv_scr_act());
     lv_label_set_text(label,"Btn");
     lv_obj_center(label);
 }
 
 






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








int main ( void )
{
	BaseType_t xReturn =pdPASS;
	
	//DHT11_Data_TypeDef DHT11_Data;
	DHT11_Init ();
	BASIC_TIM_Init();
	
	
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
	LED_GPIO_Config();
	//SysTick_Init();
	USART_Config();
	
	

	
	
	
	i2c_CfgGpio();
	
	Initial_LY096BG30();
	
	fill_picture(0x00);
	
	lv_init();
	lv_port_disp_init();
	//lv_port_indev_init();
	
	
	//lv_obj_get_disp(const lv_obj_t * obj)
	//lv_theme_mono_init(lv_disp_t * disp, bool dark_bg, const lv_font_t * font);
	lv_theme_mono_init(NULL, true, &lv_font_montserrat_14);
	lv_wtest();
	//lv_mytest();
	

	printf("Hello FreeRTOS!\n");
	
	
	
	//======================================================================
	
#define  FLASH_WriteAddress     0x00000
#define  FLASH_ReadAddress      FLASH_WriteAddress
#define  FLASH_SectorToErase    FLASH_WriteAddress
	
	
	
		/* 8M串行flash W25Q64初始化 */
	SPI_FLASH_Init();
	
	/* 获取 Flash Device ID */
	DeviceID = SPI_FLASH_ReadDeviceID();	
	delay( 200 );
	
	/* 获取 SPI Flash ID */
	FlashID = SPI_FLASH_ReadID();	
	printf("\r\n FlashID is 0x%X,\
	Manufacturer Device ID is 0x%X\r\n", FlashID, DeviceID);
	
	/* 检验 SPI Flash ID */
	if (FlashID == sFLASH_ID)
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
	
	
	
	
	
	//=======================================================================
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	

	#if 0
	while(1)
	{
		//portDISABLE_INTERRUPTS();
		if( DHT11_Read_TempAndHumidity ( & DHT11_Data ) == SUCCESS)
			{
				printf("\r\n读取DHT11成功!\r\n\r\n湿度为%d.%d ％RH ，温度为 %d.%d℃ \r\n",\
				DHT11_Data.humi_int,DHT11_Data.humi_deci,DHT11_Data.temp_int,DHT11_Data.temp_deci);
			}			
			else
			{
				printf("Read DHT11 ERROR!\r\n");
			}
			//portENABLE_INTERRUPTS();
			
			//vTaskDelay(1000);
			DHT11_delay_2us(600000);

	}
	#endif
	
	
	#if 1
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
											
#endif	
						

												
												
												
												
												
												
												
#if 0										
  xReturn = xTaskCreate((TaskFunction_t)DHT11_Task,
	                      (const char*)"DHT_Task",
												(uint16_t)512,
												(void*)NULL,
												(UBaseType_t)3,
												(TaskHandle_t*)&DHT11_Task_Handle);
#endif		

												
	vTaskStartScheduler();
	while(1)
	{
		//LED1_TOGGLE();
		//delay(1000000);
		//printf("FreeRTOS  error\n");
	}

}
