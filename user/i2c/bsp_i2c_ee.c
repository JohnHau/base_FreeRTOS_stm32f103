/**
  ******************************************************************************
  * @file    bsp_i2c_ee.c
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   i2c EEPROM(AT24C02)Ӧ�ú���bsp
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� F103-MINI STM32 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 

#include "bsp_i2c_ee.h"
#include "bsp_i2c_gpio.h"
//#include "bsp_usart.h" 


/*
*********************************************************************************************************
*	�� �� ��: i2c_CheckDevice
*	����˵��: ���I2C�����豸��CPU�����豸��ַ��Ȼ���ȡ�豸Ӧ�����жϸ��豸�Ƿ����
*	��    �Σ�_Address���豸��I2C���ߵ�ַ
*	�� �� ֵ: ����ֵ 0 ��ʾ��ȷ�� ����1��ʾδ̽�⵽
*********************************************************************************************************
*/
uint8_t ee_CheckDevice(uint8_t _Address)
{
	uint8_t ucAck;
	
	i2c_Start();		/* ���������ź� */

	/* �����豸��ַ+��д����bit��0 = w�� 1 = r) bit7 �ȴ� */
	i2c_SendByte(_Address | EEPROM_I2C_WR);
	ucAck = i2c_WaitAck();	/* ����豸��ACKӦ�� */

	i2c_Stop();			/* ����ֹͣ�ź� */

	i2c_NAck();	/*��������Ƕ���ַ����Ҫ������Ӧ���ź�*/
	
	return ucAck;
}


/*
*********************************************************************************************************
*	�� �� ��: ee_WaitStandby
*	����˵��: �ȴ�EEPROM��׼��״̬����д�����ݺ󣬱�����ñ�����
	
					д�����ʱ��ʹ��I2C�����ݴ��䵽EEPROM��
					EEPROM�����ڲ��ռ�д��������Ҫһ����ʱ�䣬
					��EEPROM�ڲ�д����ɺ���I2C���豸Ѱַ����Ӧ��
					���ñ������ɵȴ���EEPROM�ڲ�ʱ��д�����
*	��    �Σ���
*	�� �� ֵ: 0��ʾ������1��ʾ�ȴ���ʱ
*********************************************************************************************************
*/
uint8_t ee_WaitStandby(void)
{
	uint32_t wait_count = 0;
	
	while(ee_CheckDevice(EEPROM_DEV_ADDR))
	{
		//����ⳬ���������˳�ѭ��
		if(wait_count++>0xFFFF)
		{
			//�ȴ���ʱ
			return 1;
		}
	}
	//�ȴ����
	return 0;
}

/*
*********************************************************************************************************
*	�� �� ��: ee_ReadBytes
*	����˵��: �Ӵ���EEPROMָ����ַ����ʼ��ȡ��������
*	��    �Σ�_usAddress : ��ʼ��ַ
*			 _usSize : ���ݳ��ȣ���λΪ�ֽ�
*			 _pReadBuf : ��Ŷ��������ݵĻ�����ָ��
*	�� �� ֵ: 0 ��ʾʧ�ܣ�1��ʾ�ɹ�
*********************************************************************************************************
*/
uint8_t ee_ReadBytes(uint8_t *_pReadBuf, uint16_t _usAddress, uint16_t _usSize)
{
	uint16_t i;
	
	/* ���ô���EEPROM�漴��ȡָ�����У�������ȡ�����ֽ� */
	
	/* ��1��������I2C���������ź� */
	i2c_Start();
	
	/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	i2c_SendByte(EEPROM_DEV_ADDR | EEPROM_I2C_WR);	/* �˴���дָ�� */
	 
	/* ��3�����ȴ�ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}

	/* ��4���������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ������24C04���ϣ���ô�˴���Ҫ���������ַ */
	i2c_SendByte((uint8_t)_usAddress);
	
	/* ��5�����ȴ�ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}
	
	/* ��6������������I2C���ߡ�ǰ��Ĵ����Ŀ����EEPROM���͵�ַ�����濪ʼ��ȡ���� */
	i2c_Start();
	
	/* ��7������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	i2c_SendByte(EEPROM_DEV_ADDR | EEPROM_I2C_RD);	/* �˴��Ƕ�ָ�� */
	
	/* ��8��������ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}	
	
	/* ��9����ѭ����ȡ���� */
	for (i = 0; i < _usSize; i++)
	{
		_pReadBuf[i] = i2c_ReadByte();	/* ��1���ֽ� */
		
		/* ÿ����1���ֽں���Ҫ����Ack�� ���һ���ֽڲ���ҪAck����Nack */
		if (i != _usSize - 1)
		{
			i2c_Ack();	/* �м��ֽڶ����CPU����ACK�ź�(����SDA = 0) */
		}
		else
		{
			i2c_NAck();	/* ���1���ֽڶ����CPU����NACK�ź�(����SDA = 1) */
		}
	}
	/* ����I2C����ֹͣ�ź� */
	i2c_Stop();
	return 1;	/* ִ�гɹ� */

cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
	/* ����I2C����ֹͣ�ź� */
	i2c_Stop();
	return 0;
}

/*
*********************************************************************************************************
*	�� �� ��: ee_WriteBytes
*	����˵��: ����EEPROMָ����ַд���������ݣ�����ҳд�������д��Ч��
*	��    �Σ�_usAddress : ��ʼ��ַ
*			 _usSize : ���ݳ��ȣ���λΪ�ֽ�
*			 _pWriteBuf : ��Ŷ��������ݵĻ�����ָ��
*	�� �� ֵ: 0 ��ʾʧ�ܣ�1��ʾ�ɹ�
*********************************************************************************************************
*/
uint8_t ee_WriteBytes(uint8_t *_pWriteBuf, uint16_t _usAddress, uint16_t _usSize)
{
	uint16_t i,m;
	uint16_t usAddr;
	
	/* 
		д����EEPROM�������������������ȡ�ܶ��ֽڣ�ÿ��д����ֻ����ͬһ��page��
		����24xx02��page size = 8
		�򵥵Ĵ�������Ϊ�����ֽ�д����ģʽ��ÿд1���ֽڣ������͵�ַ
		Ϊ���������д��Ч��: ����������page wirte������
	*/

	usAddr = _usAddress;	
	for (i = 0; i < _usSize; i++)
	{
		/* �����͵�1���ֽڻ���ҳ���׵�ַʱ����Ҫ���·��������źź͵�ַ */
		if ((i == 0) || (usAddr & (EEPROM_PAGE_SIZE - 1)) == 0)
		{
			/*���ڣ�������ֹͣ�źţ������ڲ�д������*/
			i2c_Stop();
			
			/* ͨ���������Ӧ��ķ�ʽ���ж��ڲ�д�����Ƿ����,һ��С�� 10ms 	
			
			  CLKƵ��Ϊ200KHzʱ����ѯ����Ϊ30������	
			  ԭ��ͬ ee_WaitStandby ���������ú��������ɺ�����ֹͣ�źţ��������ڴ˴�				
			*/
			for (m = 0; m < 1000; m++)
			{				
				/* ��1��������I2C���������ź� */
				i2c_Start();
				
				/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
				i2c_SendByte(EEPROM_DEV_ADDR | EEPROM_I2C_WR);	/* �˴���дָ�� */
				
				/* ��3��������һ��ʱ�ӣ��ж������Ƿ���ȷӦ�� */
				if (i2c_WaitAck() == 0)
				{
					break;
				}
			}
			if (m  == 1000)
			{
				goto cmd_fail;	/* EEPROM����д��ʱ */
			}
		
			/* ��4���������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ������24C04���ϣ���ô�˴���Ҫ���������ַ */
			i2c_SendByte((uint8_t)usAddr);
			
			/* ��5�����ȴ�ACK */
			if (i2c_WaitAck() != 0)
			{
				goto cmd_fail;	/* EEPROM������Ӧ�� */
			}
		}
	
		/* ��6������ʼд������ */
		i2c_SendByte(_pWriteBuf[i]);
	
		/* ��7��������ACK */
		if (i2c_WaitAck() != 0)
		{
			goto cmd_fail;	/* EEPROM������Ӧ�� */
		}

		usAddr++;	/* ��ַ��1 */		
	}
	
	/* ����ִ�гɹ�������I2C����ֹͣ�ź� */
	i2c_Stop();
	
	//�ȴ����һ��EEPROM�ڲ�д�����
	if(ee_WaitStandby() == 1) //����1��ʾ��ʱ
		goto cmd_fail;
	
	return 1;

cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
	/* ����I2C����ֹͣ�ź� */
	i2c_Stop();
	return 0;
}





uint8_t  write_cmd_ssd1306(uint8_t cmd)
{

   ee_WriteBytes(&cmd, 0x00, 1);
	 return 0;

}









void ee_Erase(void)
{
	uint16_t i;
	uint8_t buf[EEPROM_SIZE];
	
	/* ��仺���� */
	for (i = 0; i < EEPROM_SIZE; i++)
	{
		buf[i] = 0xFF;
	}
	
	/* дEEPROM, ��ʼ��ַ = 0�����ݳ���Ϊ 256 */
	if (ee_WriteBytes(buf, 0, EEPROM_SIZE) == 0)
	{
		//printf("����eeprom������\r\n");
		return;
	}
	else
	{
		//printf("����eeprom�ɹ���\r\n");
	}
}





/*
 * eeprom AT24C02 ��д����
 * ��������1���쳣����0
 */

	uint8_t write_buf[EEPROM_SIZE];
  uint8_t read_buf[EEPROM_SIZE];

uint8_t ee_Test(void) 
{
  uint16_t i;

  
/*-----------------------------------------------------------------------------------*/  
  if (ee_CheckDevice(EEPROM_DEV_ADDR) == 1)
	{
		/* û�м�⵽EEPROM */
		//printf("û�м�⵽����EEPROM!\r\n");
				
		return 0;
	}
/*------------------------------------------------------------------------------------*/  
  /* �����Ի����� */
	for (i = 0; i < EEPROM_SIZE; i++)
	{		
		write_buf[i] = i;
	}
/*------------------------------------------------------------------------------------*/  
  if (ee_WriteBytes(write_buf, 0, EEPROM_SIZE) == 0)
	{
		//printf("дeeprom������\r\n");
		return 0;
	}
	else
	{		
		//printf("дeeprom�ɹ���\r\n");
	}  

/*-----------------------------------------------------------------------------------*/
  if (ee_ReadBytes(read_buf, 0, EEPROM_SIZE) == 0)
	{
		//printf("��eeprom������\r\n");
		return 0;
	}
	else
	{		
		//printf("��eeprom�ɹ����������£�\r\n");
	}
/*-----------------------------------------------------------------------------------*/  
  for (i = 0; i < EEPROM_SIZE; i++)
	{
		if(read_buf[i] != write_buf[i])
		{
			//printf("0x%02X ", read_buf[i]);
			//printf("����:EEPROM������д������ݲ�һ��");
			return 0;
		}
    //printf(" %02X", read_buf[i]);
		
		if ((i & 15) == 15)
		{
			//printf("\r\n");	
		}		
	}
  //printf("eeprom��д���Գɹ�\r\n");
  return 1;
}








//============================================================================
//============================================================================




/**********************************************
// IIC Write Command
**********************************************/
void Write_IIC_Command(unsigned char IIC_Command)
{
	
	#if 0
   IIC_Start();
   Write_IIC_Byte(0x78);            //Slave address,SA0=0
   Write_IIC_Byte(0x00);			//write command
   Write_IIC_Byte(IIC_Command); 
   IIC_Stop();
	
	#endif
	
	
	
	
	
	
	
	
}
/**********************************************
// IIC Write Data
**********************************************/
void Write_IIC_Data(unsigned char IIC_Data)
{
	#if 0
   IIC_Start();
   Write_IIC_Byte(0x78);			
   Write_IIC_Byte(0x40);			//write data
   Write_IIC_Byte(IIC_Data);
   IIC_Stop(); 
	
	
	#endif
}










void Initial_LY096BG30(void)
{
	
	write_cmd_ssd1306(0xAE);   //display off
	write_cmd_ssd1306(0x20);	//Set Memory Addressing Mode	
	write_cmd_ssd1306(0x02);//====write_cmd_ssd1306(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	write_cmd_ssd1306(0xa0);//====write_cmd_ssd1306(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	write_cmd_ssd1306(0xc0);//====write_cmd_ssd1306(0xc8);	//Set COM Output Scan Direction
	write_cmd_ssd1306(0x00);//---set low column address
	write_cmd_ssd1306(0x10);//---set high column address
	write_cmd_ssd1306(0x40);//--set start line address
	write_cmd_ssd1306(0x81);//--set contrast control register
	write_cmd_ssd1306(0x7f);
	write_cmd_ssd1306(0xa1);//--set segment re-map 0 to 127
	write_cmd_ssd1306(0xa6);//--set normal display
	write_cmd_ssd1306(0xa8);//--set multiplex ratio(1 to 64)
	write_cmd_ssd1306(0x3F);//
	write_cmd_ssd1306(0xa4);//0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	write_cmd_ssd1306(0xd3);//-set display offset
	write_cmd_ssd1306(0x00);//-not offset
	write_cmd_ssd1306(0xd5);//--set display clock divide ratio/oscillator frequency
	write_cmd_ssd1306(0xf0);//--set divide ratio
	write_cmd_ssd1306(0xd9);//--set pre-charge period
	write_cmd_ssd1306(0x22); //
	write_cmd_ssd1306(0xda);//--set com pins hardware configuration
	write_cmd_ssd1306(0x12);
	write_cmd_ssd1306(0xdb);//--set vcomh
	write_cmd_ssd1306(0x20);//0x20,0.77xVcc
	write_cmd_ssd1306(0x8d);//--set DC-DC enable
	write_cmd_ssd1306(0x14);//
	write_cmd_ssd1306(0xaf);//--turn on oled panel 
		
}




void fill_picture(unsigned char fill_Data)
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		//Write_IIC_Command(0xb0+m);		//page0-page1
		//Write_IIC_Command(0x00);		//low column start address
		//Write_IIC_Command(0x10);		//high column start address
		
		
		//write_buf[0]=0xb0+m;
	  //ee_WriteBytes(write_buf, 0x00, 1);
		write_cmd_ssd1306(0xb0+m);
		
		//write_buf[0]=0x00;
	  //ee_WriteBytes(write_buf, 0x00, 1);
		write_cmd_ssd1306(0x00);
		
		//write_buf[0]=0x10;
	  //ee_WriteBytes(write_buf, 0x00, 1);
		write_cmd_ssd1306(0x10);
		for(n=0;n<128;n++)
			{
				//Write_IIC_Data(fill_Data);
				
				write_buf[0]=fill_Data;
	      ee_WriteBytes(write_buf, 0x40, 1);
						
			}
	}
}







void fill_picture_x(unsigned char fill_Data)
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		//Write_IIC_Command(0xb0+m);		//page0-page1
		//Write_IIC_Command(0x00);		//low column start address
		//Write_IIC_Command(0x10);		//high column start address
		
		
		//write_buf[0]=0xb0+m;
	  //ee_WriteBytes(write_buf, 0x00, 1);
		write_cmd_ssd1306(0xb0+m);
		
		
		//write_buf[0]=0x0f;
	  //ee_WriteBytes(write_buf, 0x00, 1);
		write_cmd_ssd1306(0x0f);
		
		
		//write_buf[0]=0x10;
	  //ee_WriteBytes(write_buf, 0x00, 1);
		write_cmd_ssd1306(0x10);
		
		for(n=0;n<12;n++)
			{
				//Write_IIC_Data(fill_Data);
				
				write_buf[0]=fill_Data;
	      ee_WriteBytes(write_buf, 0x40, 1);
						
			}
	}
}












const unsigned char  show1[]=
{
0x00,0x06,0x0A,0xFE,0x0A,0xC6,0x00,0xE0,0x00,0xF0,0x00,0xF8,0x00,0x00,0x00,0x00,
0x00,0x00,0xFE,0x7D,0xBB,0xC7,0xEF,0xEF,0xEF,0xEF,0xEF,0xEF,0xEF,0xC7,0xBB,0x7D,
0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,
0x0C,0xFE,0xFE,0x0C,0x08,0x20,0x60,0xFE,0xFE,0x60,0x20,0x00,0x00,0x00,0x78,0x48,
0xFE,0x82,0xBA,0xBA,0x82,0xBA,0xBA,0x82,0xBA,0xBA,0x82,0xBA,0xBA,0x82,0xFE,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0xFF,
0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0xFF,0xFF,0x00,0x00,0xFE,0xFF,0x03,
0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0xFF,0xFE,0x00,0x00,0x00,0x00,0xC0,0xC0,
0xC0,0x00,0x00,0x00,0x00,0xFE,0xFF,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,
0xFF,0xFE,0x00,0x00,0xFE,0xFF,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0xFF,
0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,0xFF,0xFF,0x0C,
0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0xFF,0xFF,0x00,0x00,0x00,0x00,0xE1,0xE1,
0xE1,0x00,0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0xFF,0xFF,0x00,0x00,0xFF,0xFF,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0xFF,
0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0x1F,
0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x1F,0x0F,0x00,0x00,0x0F,0x1F,0x18,
0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x1F,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x0F,0x1F,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,
0x1F,0x0F,0x00,0x00,0x0F,0x1F,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x1F,
0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0xE2,0x92,0x8A,0x86,0x00,0x00,0x7C,0x82,0x82,0x82,0x7C,
0x00,0xFE,0x00,0x82,0x92,0xAA,0xC6,0x00,0x00,0xC0,0xC0,0x00,0x7C,0x82,0x82,0x82,
0x7C,0x00,0x00,0x02,0x02,0x02,0xFE,0x00,0x00,0xC0,0xC0,0x00,0x7C,0x82,0x82,0x82,
0x7C,0x00,0x00,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x24,0xA4,0x2E,0x24,0xE4,0x24,0x2E,0xA4,0x24,0x00,0x00,0x00,0xF8,0x4A,0x4C,
0x48,0xF8,0x48,0x4C,0x4A,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x20,0x10,0x10,
0x10,0x10,0x20,0xC0,0x00,0x00,0xC0,0x20,0x10,0x10,0x10,0x10,0x20,0xC0,0x00,0x00,
0x00,0x12,0x0A,0x07,0x02,0x7F,0x02,0x07,0x0A,0x12,0x00,0x00,0x00,0x0B,0x0A,0x0A,
0x0A,0x7F,0x0A,0x0A,0x0A,0x0B,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x20,0x40,0x40,
0x40,0x50,0x20,0x5F,0x80,0x00,0x1F,0x20,0x40,0x40,0x40,0x50,0x20,0x5F,0x80,0x00,
};



void picture_1(void)
{
  unsigned char x,y;
  unsigned int i=0;
  for(y=0;y<8;y++)
    {
      //Write_IIC_Command(0xb0+y);
      //Write_IIC_Command(0x00);
      //Write_IIC_Command(0x10);
			
			
					write_buf[0]=0xb0+y;
	  ee_WriteBytes(write_buf, 0x00, 1);
	
		
		write_buf[0]=0x00;
	  ee_WriteBytes(write_buf, 0x00, 1);
		
		write_buf[0]=0x10;
	  ee_WriteBytes(write_buf, 0x00, 1);
		
			
			
			
			
			
			
      for(x=0;x<128;x++)
        {
          //Write_IIC_Data(show1[i++]);
					
					write_buf[0]=show1[i++];
	        ee_WriteBytes(write_buf, 0x40, 1);
					
					
        }
    }
}



const unsigned char ASCII_16_Dot[  ] = {

#if 1
/*"   CHAR_22 */
0x00,0x00,0x00,0x00,0x00,0x78,0x0c,0x7c,
0xcc,0xcc,0xcc,0x76,0x00,0x00,0x00,0x00,
#endif
	
	//0x0e,0x5f,0x51,0x51,0x7e,0x3f,0x01,0x00,

};



void xdelay(uint32_t x)
{
uint32_t m,n;
	while(x)
	{
	for(m=0;m<1000;m++)
		for(n=0;n<1000;n++);
		
		x--;
	}

}

void oled_096_print(uint8_t col, uint8_t row, int8_t*str)
{
  unsigned char x,y;
  unsigned int i=0;
	y=0;
  for(y=0;y<8;y++)
    {
      //Write_IIC_Command(0xb0+y);
      //Write_IIC_Command(0x00);
      //Write_IIC_Command(0x10);
			
			
		write_buf[0]=0xb0+7-y;
	  ee_WriteBytes(write_buf, 0x00, 1);
	
		
		write_buf[0]=0x00;
	  ee_WriteBytes(write_buf, 0x00, 1);
		
		write_buf[0]=0x10;
	  ee_WriteBytes(write_buf, 0x00, 1);
		
			
			
			
			
			
			
      //for(x=0;x<128;x++)
			for(x=0;x<16;x++)
        {
          //Write_IIC_Data(show1[i++]);
					
					write_buf[0]=ASCII_16_Dot[i++];
					
					if(i==16)i=0;
					//write_buf[0]=0xff;
	        ee_WriteBytes(write_buf, 0x40, 1);
					
					
        }
				
				xdelay(20);
				
    }
}

#define ST7565_HOR_RES  128
#define ST7565_VER_RES  64
static uint8_t lcd_fb[ST7565_HOR_RES * ST7565_VER_RES / 8] = {0xAA, 0xAA};
static uint8_t pagemap[] = { 7, 6, 5, 4, 3, 2, 1, 0 };


/**********************
 *   STATIC FUNCTIONS
 **********************/
/**
 * Flush a specific part of the buffer to the display
 * @param x1 left coordinate of the area to flush
 * @param y1 top coordinate of the area to flush
 * @param x2 right coordinate of the area to flush
 * @param y2 bottom coordinate of the area to flush
 */
static void st7565_sync(int32_t x1, int32_t y1, int32_t x2, int32_t y2)
{

    //LV_DRV_DISP_SPI_CS(0);
    uint32_t m,n;
    uint8_t c, p;
	uint8_t tt;
    for(p = y1 / 8; p <= y2 / 8; p++) {
			//for(p = 0; p < 8; p++) {
        //st7565_command(CMD_SET_PAGE | pagemap[p]);
        //st7565_command(CMD_SET_COLUMN_LOWER | (x1 & 0xf));
        //st7565_command(CMD_SET_COLUMN_UPPER | ((x1 >> 4) & 0xf));
        //st7565_command(CMD_RMW);

			
				write_buf[0]=0xb0 + pagemap[p];
				ee_WriteBytes(write_buf, 0x00, 1);
				
				write_buf[0]=0x00 + x1%16;
				ee_WriteBytes(write_buf, 0x00, 1);
				
				write_buf[0]=0x10 + x1/16;
				ee_WriteBytes(write_buf, 0x00, 1);
			
			
			
        for(c = x1; c <= x2; c++) {
				//for(c = 0; c < 128; c++) {
            //st7565_data(lcd_fb[(ST7565_HOR_RES * p) + c]);
					ee_WriteBytes(&lcd_fb[(ST7565_HOR_RES * p)+c], 0x40, 1);
					
					
					//ee_WriteBytes(&lcd_fb[c], 0x40, 1);
					
					//tt=0xff;
					//ee_WriteBytes(&tt, 0x40, 1);
					
					
					//ee_WriteBytes(&tt, 0x40, 1);
					
						//for(m=0;m<4000;m++)
				      // for(n=0;n<1000;n++);
					
					
					
					
					
					
        }
				
				
				//for(m=0;m<5000;m++)
				   //for(n=0;n<1000;n++);
				
				
    }

    //LV_DRV_DISP_SPI_CS(1);
}
#include "../library/GUI/lvgl/src/misc/lv_color.h"
void st7565_flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_p)
{
	
    /*Return if the area is out the screen*/
    if(x2 < 0) return;
    if(y2 < 0) return;
    if(x1 > ST7565_HOR_RES - 1) return;
    if(y1 > ST7565_VER_RES - 1) return;

    /*Truncate the area to the screen*/
    int32_t act_x1 = x1 < 0 ? 0 : x1;
    int32_t act_y1 = y1 < 0 ? 0 : y1;
    int32_t act_x2 = x2 > ST7565_HOR_RES - 1 ? ST7565_HOR_RES - 1 : x2;
    int32_t act_y2 = y2 > ST7565_VER_RES - 1 ? ST7565_VER_RES - 1 : y2;

    int32_t x, y;

    /*Set the first row in */

    /*Refresh frame buffer*/
    for(y = act_y1; y <= act_y2; y++) {
        for(x = act_x1; x <= act_x2; x++) {
            if(lv_color_to1(*color_p) != 0) {
                lcd_fb[x + (y / 8)*ST7565_HOR_RES] &= ~(1 << (7 - (y % 8)));
							  //lcd_fb[x + (y / 8)*ST7565_HOR_RES] |= (1 << (7 - (y % 8)));
            } else {
                lcd_fb[x + (y / 8)*ST7565_HOR_RES] |= (1 << (7 - (y % 8)));
							  //lcd_fb[x + (y / 8)*ST7565_HOR_RES] &= ~(1 << (7 - (y % 8)));
            }
            color_p ++;
        }

        color_p += (x2 - act_x2); /*Next row*/
    }

    st7565_sync(act_x1, act_y1, act_x2, act_y2);
    //lv_flush_ready();
		
		
		
}














/*********************************************END OF FILE**********************/