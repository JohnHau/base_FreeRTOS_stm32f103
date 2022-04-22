#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include "../CRC16/CRC16.h"
#include "stp.h"



uint8_t stp_signal =0;
stp_frame_t stp_frame;
uint8_t stp_frame_buf[512]={0};

stp_state_t stp_state_machine(stp_frame_t *stp_frame,uint8_t ch)
{
	static uint8_t temp[2] ={0};
	static uint16_t packet_cnt=0;
	static uint16_t payload_len_cnt=0;
	static uint16_t payload_cnt=0;
	static uint16_t crc_cnt=0;
  static stp_state_t stp_state = STATE_STP_TAG;
	
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
				 if(stp_frame->packet_num)
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
				 stp_frame->payload_len = temp[0]*16 + temp[1];
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




uint8_t stp_temp[512]={0};
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
#if 0	
	printf("\r\n====rec is\r\n");
	for(i=0;i < stp_frame->payload_len +6;i++)
	{
		printf("%x ",stp_temp[i]);
	}
	//printf("====\r\n");
#endif	
	crc16 = cal_CRC16(stp_temp,stp_frame->payload_len + 6);
	
	if(crc16 == stp_frame->crc16)
	{
	//	printf("\r\n====valid stp frame\r\n");
		return 0;
	}
	else
	{
	//	printf("\r\n====invalid stp frame\r\n");
	}

//	printf("crc is %x",crc16);
//	printf("fcrc is %x", stp_frame->crc16);
	return 1;
}


uint16_t send_stp_frame(int fd,uint8_t *payload,uint16_t payload_len,uint16_t packet_num)
{
     uint16_t i =0;
	 uint16_t crc16 =0;

	 memset(stp_temp,0,512);
	 stp_temp[0] = 'S';
	 stp_temp[1] = 'T';
	 stp_temp[2] = packet_num/256;
	 stp_temp[3] = packet_num%256;
	 stp_temp[4] = payload_len/256;
	 stp_temp[5] = payload_len%256;
	
	for(i=0;i < payload_len;i++)
	{
	  stp_temp[6+i] = payload[i];
	}
	
	
    crc16 = cal_CRC16(stp_temp,payload_len + 6);


    stp_temp[payload_len + 6] = crc16/256;
    stp_temp[payload_len + 7] = crc16%256;
    //printf("crc16 is %x\r\n",crc16);
    if(write(fd,stp_temp,payload_len + 8) != (payload_len + 8))
    {
	    printf("error:send stp frame\r\n");
	    return -1;
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
