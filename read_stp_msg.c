#include<sys/types.h>
#include<errno.h>
#include<string.h>
#include<termios.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>

#include "CRC16/CRC16.h"			
#include "small_text_protocol/stp.h"			

int open_port(int fd, int comport)
{
	char *dev[] = {"/dev/ttyS0","/dev/ttyS1","/dev/ttyS6"};
	long vdisable;

	if(comport == 1)
	{
		fd = open("/dev/ttyS10",O_RDWR|O_NOCTTY|O_NDELAY);
		if(fd == -1)
		{
			perror("can not open serial port failed\n");
			return -1;	
		}

	}




	if(fcntl(fd,F_SETFL,0) < 0)
	{
		printf("fcntl  failed!\n");	
	}
	else
	{
		printf("fcntl = %d\n",fcntl(fd,F_SETFL,0));	

	}

	if(isatty(STDIN_FILENO) == 0)
	{
		printf("standard input is not a terminal device\n");	
	}
	else
	{
		printf("isatty success!\n");	
	}

	printf("fd-open = %d\n",fd);


	return fd;
}


int set_opt(int fd,int nSpeed,int nBits,char nEvent,int nStop)
{
	struct termios newtio,oldtio;

	if(tcgetattr(fd,&oldtio) != 0)
	{
		perror("Setupserial 1");
		return -1;	
	}	

	bzero(&newtio,sizeof(newtio));

	newtio.c_cflag |= CLOCAL |CREAD;
	newtio.c_cflag &= ~CSIZE;

	switch(nBits)
	{
		case 7:
			newtio.c_cflag |= CS7;
			break;

		case 8:
			newtio.c_cflag |= CS8;
			break;
	}

	switch(nEvent)
	{
		case 'O':
			newtio.c_cflag |= PARENB;	
			newtio.c_cflag |= PARODD;	
			newtio.c_iflag |= (INPCK | ISTRIP);	
			break;

		case 'E':
			newtio.c_iflag |= (INPCK | ISTRIP);	
			newtio.c_cflag |= PARENB;	
			newtio.c_cflag &= ~PARODD;	
			break;
		case 'N':
			newtio.c_cflag &= ~PARENB;
			break;
	}

	switch(nSpeed)
	{
		case 2400:
			cfsetispeed(&newtio,B2400);	
			cfsetospeed(&newtio,B2400);	
			break;

		case 4800:
			cfsetispeed(&newtio,B4800);	
			cfsetospeed(&newtio,B4800);	
			break;

		case 9600:
			cfsetispeed(&newtio,B9600);	
			cfsetospeed(&newtio,B9600);	
			break;

		case 115200:
			cfsetispeed(&newtio,B115200);	
			cfsetospeed(&newtio,B115200);	
			break;

		default:
			cfsetispeed(&newtio,B9600);	
			cfsetospeed(&newtio,B9600);	
			break;
	}

	if(nStop ==1 )
		newtio.c_cflag &= ~CSTOPB;
	else if(nStop == 2)
		newtio.c_cflag |= CSTOPB;

	newtio.c_cc[VTIME] = 1;

	newtio.c_cc[VMIN] = 1;

	tcflush(fd,TCIFLUSH);


	if((tcsetattr(fd,TCSANOW,&newtio)) != 0)
	{
		perror("serial com error");	
		return -1;
	}

	printf("set done!\n");
	return 0;

}


#if 0
uint8_t stp_frame_test[16] = {
	0x53,0x54,
	0x00,0x01,
	0x00,0x08,
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,
	0x87,0x63
};
#endif
uint8_t stp_frame_test[512] = {

	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,
};

uint8_t frame_write_start[] = "ST WRITE START\r\n";
uint8_t frame_write_end[]   = "ST WRITE END\r\n";

uint8_t frame_read_start[] = "ST READ START\r\n";
uint8_t frame_read_end[]   = "ST READ END\r\n";
uint8_t frame_read_ack[]   = "ST READ ACK\r\n";
uint8_t frame_reading[]   = "ST READING\r\n";



typedef struct _stp_queue
{

	uint16_t head;
	uint16_t tail;
	uint8_t rbuf[65535];

}stp_queue;


uint8_t rxbuf[65535] = {0};

stp_queue  stp_q;


uint8_t stp_msg_buf[65535]={0};
uint8_t msg_buf[65535]={0};
stp_frame_t stp_msg;

uint8_t stp_ack[] = "stp ack";



uint32_t read_stp_msg(int fd,uint8_t* buf,uint16_t len)
{

	uint16_t nread =0;
	nread = read(fd,buf,len);

	if(nread >  0)
	{
		//printf("read data is %s\n",rxbuf);

		for(uint16_t i =0;i<nread;i++)
		{
			stp_q.rbuf[stp_q.tail] = rxbuf[i];
			stp_q.tail ++; 
		}
	}




	while(stp_q.head != stp_q.tail)
	{

		if(stp_q.rbuf[stp_q.head]  == 'S')
		{

			//	printf("111111\n");
			//	exit(0);
			stp_q.head ++; 
			if(stp_q.rbuf[stp_q.head]  == 'T')
			{

				//		printf("222222\n");
				msg_buf[0] = 'S';
				msg_buf[1] = 'T';
				stp_msg.tag[0]= msg_buf[0] ;
				stp_msg.tag[1]= msg_buf[1];


				stp_q.head ++;
				msg_buf[2]= stp_q.rbuf[stp_q.head]; 
				stp_q.head ++;
				msg_buf[3]= stp_q.rbuf[stp_q.head]; 
				stp_msg.packet_num = msg_buf[2]*256 + msg_buf[3];


				stp_q.head ++;
				msg_buf[4]= stp_q.rbuf[stp_q.head]; 
				stp_q.head ++;
				msg_buf[5]= stp_q.rbuf[stp_q.head]; 

				stp_msg.payload_len = msg_buf[4]*256 + msg_buf[5];

				for(uint16_t i=0;i< stp_msg.payload_len; i++)
				{
					stp_q.head ++;
					msg_buf[6 + i]= stp_q.rbuf[stp_q.head]; 
					stp_msg.payload[i] = msg_buf[6 + i]; 


				}

				stp_q.head ++;
				msg_buf[6 + stp_msg.payload_len] = stp_q.rbuf[stp_q.head]; 
				stp_q.head ++;
				msg_buf[7 + stp_msg.payload_len] = stp_q.rbuf[stp_q.head]; 

				stp_msg.crc16 = msg_buf[6 + stp_msg.payload_len]*256 + msg_buf[7 + stp_msg.payload_len];
#if 0
				printf("\n===== msg is \n");
				for(int i=0;i< (8 + stp_msg.payload_len);i++)
				{
					printf("%2x ",msg_buf[i]);
				}
				printf("\n=====\n");
#endif
				if(verify_stp_frame(&stp_msg) == 0)
				{
					return 0;
				}
				else
				{
					return 1;

				}



			}
			else
			{
				stp_q.head ++; 
			}
		}
		else
		{

			stp_q.head ++; 
		}





	}



}


uint32_t  stp_msg_process(stp_frame_t* stp_msg_received)
{

	printf("msg is %s\n",stp_msg.payload);

	if(strncmp((char*)stp_msg.payload,stp_ack,strlen(stp_ack)) == 0)
	{
		printf("stp ack\r\n");
	}
	else
	{
		printf("error stp ack\r\n");
	}



	if(strncmp((char*)stp_msg.payload,"stp dat",strlen("stp dat")) == 0)
	{
		//send_stp_frame(USART3,stp_ack,strlen((char*)stp_ack),mn++);

		if(strncmp((char*)stp_msg.payload + 7,"dht11",strlen("dht11")) == 0)
		{
			//send_stp_frame(USART3,stp_ack,strlen((char*)stp_ack),mn++);dht11_rt
			//
			//send_stp_frame(USART3,dht11_rt,strlen((char*)dht11_rt),mn++);
			//
			printf("R is %d.%d and T is %d.%d\n",stp_msg.payload[12],stp_msg.payload[13],
					stp_msg.payload[14],stp_msg.payload[15]);


			return 0;
		}


		//printf("stp ack\r\n");
	}
	else
	{
		//printf("error st write start\r\n");
	}



	return 1;


}







int main(int argc, char** argv)
{
	int fd;
	int nread,i;

	char buf[] = "Hello\n";

	stp_q.head = 0;
	stp_q.tail = 0;
	memset(stp_q.rbuf,0,sizeof(stp_q.rbuf));


	stp_msg.payload = stp_msg_buf;
	stp_msg.tag[0]=0;
	stp_msg.tag[1]=0;

	if((fd = open_port(fd,1)) < 0)
	{
		perror("open_port error");
		return -1;	
	}

	if((i = set_opt(fd,115200,8,'N',1)) < 0 )
	{
		perror("set_opt error");	
		return -1;
	}

	printf("fd = %d\n",fd);


	uint8_t test[16] = {0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39};
	uint8_t test_sn = 0;

#if 0
	for(int i=0;i<100;i++)
	{
		send_stp_frame(fd,test,sizeof(test),test_sn++);
		sleep(2);

	}


#endif
	test[0] = 's';
	test[1] = 't';
	test[2] = 'p';
	test[3] = ' ';
	test[4] = 'c';
	test[5] = 'm';
	test[6] = 'd';

	test[7] = 'd';
	test[8] = 'h';
	test[9] = 't';
	test[10] = '1';
	test[11] = '1';

	test[12] = 0x71;
	test[13] = 0x72;
	test[14] = 0x73;
	test[15] = 0x74;

	while(1)
	{
		sleep(1);
		send_stp_frame(fd,test,sizeof(test),test_sn++);

		if(read_stp_msg(fd,rxbuf,1024) == 0)
		{
			stp_msg_process(&stp_msg);

		}
	}


	close(fd);

	return -1;
}
