#ifndef __I2C_EE_H
#define	__I2C_EE_H


#include "stm32f10x.h"
#include "bsp_i2c_gpio.h"

#include "../library/GUI/lvgl/src/misc/lv_color.h"
/* 
 * AT24C02 2kb = 2048bit = 2048/8 B = 256 B
 * 32 pages of 8 bytes each
 *
 * Device Address
 * 1 0 1 0 A2 A1 A0 R/W
 * 1 0 1 0 0  0  0  0 = 0XA0
 * 1 0 1 0 0  0  0  1 = 0XA1 
 */

/* AT24C01/02ÿҳ��8���ֽ� 
 * AT24C04/08A/16Aÿҳ��16���ֽ� 
 */
	

//#define EEPROM_DEV_ADDR			0xA0		/* 24xx02���豸��ַ */
#define EEPROM_DEV_ADDR			0x78		/* 24xx02���豸��ַ */
#define EEPROM_PAGE_SIZE		  8			  /* 24xx02��ҳ���С */
#define EEPROM_SIZE				  256			  /* 24xx02������ */



uint8_t ee_ReadBytes(uint8_t *_pReadBuf, uint16_t _usAddress, uint16_t _usSize);
uint8_t ee_WriteBytes(uint8_t *_pWriteBuf, uint16_t _usAddress, uint16_t _usSize);
uint8_t ee_CheckDevice(uint8_t _Address);
void ee_Erase(void);
uint8_t ee_WaitStandby(void);
uint8_t ee_Test(void) ;


void Initial_LY096BG30(void);
void fill_picture(unsigned char fill_Data);
void picture_1(void);

void fill_picture_x(unsigned char fill_Data);

void oled_096_print(uint8_t col, uint8_t row, int8_t*str);




void st7565_flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_p);


#endif /* __I2C_EE_H */
