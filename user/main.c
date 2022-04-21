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
	/* SystemFrequency / 1000    1ms�ж�һ��
	 * SystemFrequency / 100000	 10us�ж�һ��
	 * SystemFrequency / 1000000 1us�ж�һ��
	 */
//	if (SysTick_Config(SystemFrequency / 100000))	// ST3.0.0��汾
	if (SysTick_Config(SystemCoreClock / 1000000))	// ST3.5.0��汾
	{ 
		/* Capture error */ 
		while (1);
	}
		// �رյδ�ʱ��  
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
}



static TaskHandle_t LED1_Task_Handle = NULL;
static TaskHandle_t LED2_Task_Handle = NULL;
static TaskHandle_t DHT11_Task_Handle = NULL;
static TaskHandle_t OLED_thread_Handle = NULL;

static TaskHandle_t sensors_thread_Handle = NULL;


static TaskHandle_t stp_thread_Handle = NULL;


typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;


/* ��ȡ�������ĳ��� */
#define TxBufferSize1   (countof(TxBuffer1) - 1)
#define RxBufferSize1   (countof(TxBuffer1) - 1)
#define countof(a)      (sizeof(a) / sizeof(*(a)))
#define  BufferSize (countof(Tx_Buffer)-1)


/* ���ͻ�������ʼ�� */
uint8_t Tx_Buffer[] = "��л��ѡ��Ұ��stm32������\r\n";
uint8_t Rx_Buffer[BufferSize];
__IO uint32_t DeviceID = 0;
__IO uint32_t FlashID = 0;
__IO TestStatus TransferStatus1 = FAILED;

/*
 * ��������Buffercmp
 * ����  ���Ƚ������������е������Ƿ����
 * ����  ��-pBuffer1     src������ָ��
 *         -pBuffer2     dst������ָ��
 *         -BufferLength ����������
 * ���  ����
 * ����  ��-PASSED pBuffer1 ����   pBuffer2
 *         -FAILED pBuffer1 ��ͬ�� pBuffer2
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
	
	char *ppp = NULL;
	BaseType_t xReturn =pdPASS;
	

	DHT11_Init ();
	BASIC_TIM_Init();
	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	LED_GPIO_Config();
	EXTI_Key_Config();
	//SysTick_Init();
	USART_Config();
	
	
	//init_RTC();
	
	
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
	
#define  FLASH_WriteAddress     0x00000
#define  FLASH_ReadAddress      FLASH_WriteAddress
#define  FLASH_SectorToErase    FLASH_WriteAddress
	
	
	
		/* 8M����flash W25Q64��ʼ�� */
	SPI_FLASH_Init();
	
	/* ��ȡ Flash Device ID */
	DeviceID = SPI_FLASH_ReadDeviceID();	
	delay( 200 );
	
	/* ��ȡ SPI Flash ID */
	FlashID = SPI_FLASH_ReadID();	
	printf("\r\n FlashID is 0x%X,\
	Manufacturer Device ID is 0x%X\r\n", FlashID, DeviceID);
	
	/* ���� SPI Flash ID */
	if (FlashID == sFLASH_ID)
	{	
		printf("\r\n ��⵽����flash W25Q64 !\r\n");
		
		/* ������Ҫд��� SPI FLASH ������FLASHд��ǰҪ�Ȳ��� */
		// �������4K����һ����������������С��λ������
		SPI_FLASH_SectorErase(FLASH_SectorToErase);	 	 
		
		/* �����ͻ�����������д��flash�� */
		// ����дһҳ��һҳ�Ĵ�СΪ256���ֽ�
		SPI_FLASH_BufferWrite(Tx_Buffer, FLASH_WriteAddress, BufferSize);		
		printf("\r\n д�������Ϊ��%s \r\t", Tx_Buffer);
		
		/* ���ո�д������ݶ������ŵ����ջ������� */
		SPI_FLASH_BufferRead(Rx_Buffer, FLASH_ReadAddress, BufferSize);
		printf("\r\n ����������Ϊ��%s \r\n", Rx_Buffer);
		
		/* ���д�������������������Ƿ���� */
		TransferStatus1 = Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);
		
		if( PASSED == TransferStatus1 )
		{ 
			//LED2_ON;
			printf("\r\n 8M����flash(W25Q64)���Գɹ�!\n\r");
		}
		else
		{        
			//LED1_ON;
			printf("\r\n 8M����flash(W25Q64)����ʧ��!\n\r");
		}
	}// if (FlashID == sFLASH_ID)
	else// if (FlashID == sFLASH_ID)
	{ 
		//LED1_ON;
		printf("\r\n ��ȡ���� W25Q64 ID!\n\r");
	}
	
	
	
	
	
	//=======================================================================
	
	
	
	
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
												
												
					


												
												
//	xReturn = xTaskCreate((TaskFunction_t)stp_thread,
//	                      (const char*)"stp_thread",
//												(uint16_t)512,
//												(void*)NULL,
//												(UBaseType_t)2,
//												(TaskHandle_t*)&stp_Task_Handle);											
												
												
												
					


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
