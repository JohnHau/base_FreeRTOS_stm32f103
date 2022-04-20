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
		fd = open("/dev/ttyS8",O_RDWR|O_NOCTTY|O_NDELAY);
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

uint8_t rbuff[2048] = {0};
int main(int argc, char** argv)
{
	int fd;
	int nread,i;

	char buff[] = "Hello\n";



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

	//send_stp_frame(int fd,uint8_t *payload,uint16_t payload_len,uint16_t packet_num);
	//send_stp_frame(fd,stp_frame_test,480,1);
	//send_stp_frame(fd,frame_write_start,strlen(frame_write_start),1);
//	sleep(3);






	send_stp_frame(fd,frame_read_start,strlen(frame_read_start),1);
	//sleep(3);

	nread = read(fd,rbuff,1024);

	if(nread >  0)
	{
		printf("read data is %s\n",rbuff);

	}



	for(uint32_t m=0;m<1024*1024;m++)
	{
	
		send_stp_frame(fd,frame_reading,strlen(frame_reading),1);
		memset(rbuff,0,16);
		nread = read(fd,rbuff,1024);

		if(nread >  0)
		{
			printf("nread is %d\n",nread);
			for(int i =0; i< nread;i++)
			{
				printf("%2x ",rbuff[i]);	
			}
			printf("\n");

		}


	
	}






#if 0
	printf("start to read\n");
	nread = read(fd,rbuff,1024);
	printf("finish read\n");

	if(nread >  0)
	{
		printf("%s\n",rbuff);

	}
	printf("nread = %d\n",nread);
#endif





//	send_stp_frame(fd,frame_write_end,strlen(frame_write_end),1);

#if 0
	if(write(fd,stp_frame_test,16) != 16)
		write(2,"A write error has occurred!\n",28);
#endif



//	sleep(3);
#if 0
	printf("start to read\n");
	nread = read(fd,rbuff,1024);
	printf("finish read\n");

	if(nread >  0)
	{
		printf("%s\n",rbuff);

	}
	printf("nread = %d\n",nread);
#endif









	close(fd);


	return -1;
}
