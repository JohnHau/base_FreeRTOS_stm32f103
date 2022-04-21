#include <stdio.h>
#include <stdint.h>
#include "usart/bsp_usart.h"
#include "CRC16/CRC16.h"
#include "small_text_protocol/stp.h"

#include "FreeRTOS.h"					//FreeRTOSÊ¹ÓÃ		  
#include "task.h" 
#include "queue.h"
#include "semphr.h"	

#include "../library/GUI/lvgl/lvgl.h"
#include "GUI_APP/gui_app.h"
uint8_t stp_signal =0;

SemaphoreHandle_t BinarySem_stp_Handle = NULL;



uint8_t stp_read_signal =0;
stp_frame_t stp_frame_test;
uint8_t stp_frame_buf[512*2]={0};




stp_state_t stp_state_machine(stp_frame_t *stp_frame,uint8_t ch)
{
	static uint8_t temp[2] ={0};
	static uint16_t packet_cnt=0;
	static uint16_t payload_len_cnt=0;
	static uint16_t payload_cnt=0;
	static uint16_t crc_cnt=0;
  static stp_state_t stp_state = STATE_STP_TAG;
	BaseType_t  xHigherPriorityTaskWoken;
  switch(stp_state)
	{
		case STATE_STP_TAG:
	
		  if(ch == 'S')
			{
			   stp_frame->tag[0] = ch;
			}
			else if(stp_frame->tag[0] == 'S' && ch == 'T')
			{
			   stp_frame->tag[1] = ch;			
				 stp_state = STATE_STP_PACKET_NUM;
				 //printf("state stp packer num");
				
			}
			else
			{
			   stp_frame->tag[0] =0;
				 stp_frame->tag[1] =0;
			}
		
		
			break;
		
		case STATE_STP_PACKET_NUM:
			
		   temp[packet_cnt++] = ch;
		   if(packet_cnt>=2)
			 {
			   packet_cnt =0;
				 stp_frame->packet_num = temp[0]*16 + temp[1];
				 if(1)//(stp_frame->packet_num)
				 {
				   stp_state = STATE_STP_PAYLOAD_LEN;
					 //printf("state stp payload len");
				 }
				 else
				 {
				   stp_frame->tag[0] =0;
				   stp_frame->tag[1] =0;
					 stp_state = STATE_STP_TAG;
				 }
			 }
		
			break;
			 
		case STATE_STP_PAYLOAD_LEN:
			
		   temp[payload_len_cnt++] = ch;
		   if(payload_len_cnt>=2)
			 {
			   payload_len_cnt =0;
				 stp_frame->payload_len = temp[0]*256 + temp[1];
				 if(stp_frame->payload_len)
				 {
				   stp_state = STATE_STP_PAYLOAD;
					 //printf("state stp payload");
				 }
				 else
				 {
				   stp_frame->tag[0] =0;
				   stp_frame->tag[1] =0;
					 stp_state = STATE_STP_TAG;
				 }
			 }
		
			break;
			 
			 
			 
		
		case STATE_STP_PAYLOAD:
		   
       if(payload_cnt < stp_frame->payload_len)	
			 {
					stp_frame->payload[payload_cnt++] = ch;
				 
				  if(payload_cnt == stp_frame->payload_len)
					{
					   stp_state = STATE_STP_CRC;
					}
			 }	

			break;
			
		case STATE_STP_CRC:

			temp[crc_cnt++] = ch;
		 //printf("state stp reach end");
		  if(crc_cnt>=2)
			 {

				 stp_frame->crc16 = temp[0]*256 + temp[1];
				
				 //stp_frame->tag[0] =0;
				 //stp_frame->tag[1] =0;
				 
				 			
			   packet_cnt =0;
		     payload_len_cnt=0;
		     payload_cnt=0;
		     crc_cnt =0;
		 
				//if(verify_stp_frame(stp_frame) == 0) 
				{					
					xSemaphoreGiveFromISR(BinarySem_stp_Handle, &xHigherPriorityTaskWoken);
					portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
				}
				 
				 
				 stp_state = STATE_STP_TAG;
				 stp_signal =1;
			 }
			break;
			
		default:
			   stp_frame->tag[0] =0;
				 stp_frame->tag[1] =0;
				 stp_state = STATE_STP_TAG;
			break;
	
	}
	return stp_state;
}




uint8_t stp_temp[512*2]={0};
uint16_t verify_stp_frame(stp_frame_t *stp_frame)
{
   uint16_t i =0;
	 uint16_t crc16 =0;
	 stp_temp[0] = stp_frame->tag[0];
	 stp_temp[1] = stp_frame->tag[1];
	 stp_temp[2] = stp_frame->packet_num/256;
	 stp_temp[3] = stp_frame->packet_num%256;
	 stp_temp[4] = stp_frame->payload_len/256;
	 stp_temp[5] = stp_frame->payload_len%256;
	
	for(i=0;i < stp_frame->payload_len;i++)
	{
	  stp_temp[6+i] = stp_frame->payload[i];
	}
	
	//printf("rec is\r\n");
	for(i=0;i < stp_frame->payload_len +6;i++)
	{
	  
		//printf("%x ",stp_temp[i]);
	}
	//printf("\r\n");
	
  crc16 = cal_CRC16(stp_temp,stp_frame->payload_len + 6);
	
	if(crc16 == stp_frame->crc16)
	{
	  
		//static uint8_t mn =1;
		//printf("\r\n valid stp frame\r\n");
		//lv_label_set_text_fmt(lbl_R, "%d stp",mn++);
		return 0;
	}
	else
	{
	  //printf("\r\n invalid stp frame\r\n");
	}
	
	//printf("crc is %x",crc16);
	//printf("fcrc is %x", stp_frame->crc16);
	return 1;
}

uint16_t send_stp_frame(USART_TypeDef * pUSARTx,uint8_t *payload,uint16_t payload_len,uint16_t packet_num)
{
     uint16_t i =0;
	 uint16_t crc16 =0;

	uint8_t mt[8]="hello";
	//uart_write(USART1,mt, 5);
	//return 0;
	
	
	
	 memset(stp_temp,0,512*2);
	 stp_temp[0] = 'S';
	 stp_temp[1] = 'T';
	 stp_temp[2] = packet_num/256;
	 stp_temp[3] = packet_num%256;
	 stp_temp[4] = payload_len/256;
	 stp_temp[5] = payload_len%256;
	//uart_write(USART1,mt, 5);
	//return 0;
	for(i=0;i < payload_len;i++)
	{
	  stp_temp[6+i] = payload[i];
	}
	
	
    crc16 = cal_CRC16(stp_temp,payload_len + 6);


    stp_temp[payload_len + 6] = crc16/256;
    stp_temp[payload_len + 7] = crc16%256;
    //printf("crc16 is %x\r\n",crc16);
	
	
	  uart_write(pUSARTx,stp_temp, payload_len + 8);
	  //uart_write(USART1,mt, 5);
    //if(write(fd,stp_temp,payload_len + 8) != (payload_len + 8))
    {
	    //printf("error:send stp frame\r\n");
	    //return -1;
    }

    return 0;
}


uint32_t init_stp_frame(stp_frame_t *stp_frame,uint8_t *buf)
{
	
	stp_frame->tag[0] = 0;//'L';
	stp_frame->tag[1] = 0;//'T';
	stp_frame->packet_num = 0;
	stp_frame->payload_len =0;
	stp_frame->payload = buf;
	stp_frame->crc16= 0;
	
	return 0;
	
}




uint8_t tcrc[]={0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39};
uint16_t v=0;



uint16_t pn =0;
uint16_t bblock =0;
uint16_t bsize =0;
uint16_t cnt=0;
uint8_t st_read_ack[]="ST READ ACK\r\n";

uint8_t stp_tempx[8] = {0};


uint8_t stp_ack[] = "stp ack";


uint8_t dht11_rt[16]={'s','t','p',' ','d','a','t','d','h','t','1','1',0x14,0x15,0x16,0x17};

void stp_thread(void)
{
  BaseType_t xReturn =pdPASS;
	init_stp_frame(&stp_frame_test,stp_frame_buf);
	
	for(cnt=0;cnt<8;cnt++)
		stp_tempx[cnt] = 0x31;
	
BinarySem_stp_Handle = xSemaphoreCreateBinary();
while(1)
{
	   
	
	
	xReturn =xSemaphoreTake(BinarySem_stp_Handle,portMAX_DELAY);
	  if(1)//if(stp_signal)
		 {
		   stp_signal =0;
		   if(verify_stp_frame(&stp_frame_test) == 0)
			 {	 
				 
				       static uint8_t mn =1;
		           //printf("\r\n valid stp frame\r\n");
		           //lv_label_set_text_fmt(lbl_R, "%d stp",mn++);
				       //send_stp_frame(USART3,stp_ack,strlen((char*)stp_ack),mn++);
				 
				 
				 
				  	   if(strncmp((char*)stp_frame_test.payload,"stp cmd",strlen("stp cmd")) == 0)
							 {
									//send_stp_frame(USART3,stp_ack,strlen((char*)stp_ack),mn++);
								 
								 if(strncmp((char*)stp_frame_test.payload + 7,"dht11",strlen("dht11")) == 0)
								 {
								     //send_stp_frame(USART3,stp_ack,strlen((char*)stp_ack),mn++);dht11_rt
									   send_stp_frame(USART3,dht11_rt,strlen((char*)dht11_rt),mn++);
								 }
								 
								 
								 //printf("stp ack\r\n");
							 }
							 else
							 {
							    //printf("error st write start\r\n");
							 }
				 
							
				 
				 
				 
				 	     if(strncmp((char*)stp_frame_test.payload,"stp ack",strlen("stp ack")) == 0)
							 {
									send_stp_frame(USART3,stp_ack,strlen((char*)stp_ack),mn++);
								 //printf("stp ack\r\n");
							 }
							 else
							 {
							    //printf("error st write start\r\n");
							 }
				 
				 
				 
				 
				 
							 if(strncmp((char*)stp_frame_test.payload,"ST WRITE START\r\n",strlen("ST WRITE START\r\n")) == 0)
							 {
									printf("st write start\r\n");
							 }
							 else
							 {
							    //printf("error st write start\r\n");
							 }
							 //printf("start is %s\n",stp_frame_test.payload);
							 
							 
							 if(strncmp((char*)stp_frame_test.payload,"ST WRITE END\r\n",strlen("ST WRITE END\r\n")) == 0)
							 {
									printf("st write end\r\n");
							 }
							 else
							 {
							    //printf("error st write end\r\n");
							 }
							 
							 
							 
							 if(strncmp((char*)stp_frame_test.payload,"ST READ START\r\n",strlen("ST READ START\r\n")) == 0)
							 {
									
								 //send_stp_frame(USART1,"ST READ ACK\r\n",strlen("ST READ ACK\r\n"),0);
								 //send_stp_frame(USART1,st_read_ack,strlen(st_read_ack),0);
								 stp_read_signal =1;
								 printf("st read start\r\n");
							 }
							 else
							 {
							    //printf("error st write start\r\n");
							 }
							 //printf("start is %s\n",stp_frame_test.payload);
							 
							 
							 if(strncmp((char*)stp_frame_test.payload,"ST READ END\r\n",strlen("ST READ END\r\n")) == 0)
							 {
									printf("st read end\r\n");
							 }
							 else
							 {
							    //printf("error st write end\r\n");
							 }
							 
							 
							 
							 if(stp_read_signal)
							 {
							 
							      //for(bblock=0;bblock < 0x8000;bblock++) 
								 	 if(strncmp((char*)stp_frame_test.payload,"ST READING\r\n",strlen("ST READING\r\n")) == 0)
									 {

										  //printf("st reading\r\n");
											//send_stp_frame(USART3,stp_tempx,sizeof(stp_tempx),pn++);
										  //send_stp_frame(USART1,stp_tempx,sizeof(stp_tempx),700);
									 }
								 
							 
							 }
							 
							 
							 
							 
							 
			 
			 }
			 
			 memset(stp_frame_test.payload,0,512*2);
		 }

}



}










































