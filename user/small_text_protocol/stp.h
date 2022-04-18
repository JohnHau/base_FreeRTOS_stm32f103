#ifndef _STP_H_
#define _STP_H_


typedef struct stp_handshake
{
 uint8_t stp_version;
 uint32_t payload_size;
 uint16_t crc16;
	
}stp_handshake_t;






typedef enum stp_state
{
  STATE_STP_TAG =0,
	STATE_STP_PACKET_NUM,
	STATE_STP_PAYLOAD_LEN,
	STATE_STP_PAYLOAD,
	STATE_STP_CRC
	
}stp_state_t;

typedef struct stp_frame
{
 uint8_t tag[2];
 uint16_t packet_num;
 uint16_t payload_len;
 uint8_t *payload; 
 uint16_t crc16;
	
}stp_frame_t;
//payload "ST WRTIE START\r\n"
//payload "ST WRITE END\r\n"





//payload "ST READ START\r\n"
//payload "ST READ END\r\n"
//payload "ST READ ACK\r\n"


typedef struct stp_response
{
 uint8_t tag[2];
 uint16_t packet_num;
 uint16_t err_code; 
 uint16_t crc16;
	
}stp_response_t;



extern uint8_t stp_signal;

extern SemaphoreHandle_t BinarySem_stp_Handle;




extern uint8_t stp_read_signal;
extern stp_frame_t stp_frame_test;
extern uint8_t stp_frame_buf[512*2];
extern uint8_t stp_temp[512*2];

extern stp_state_t stp_state_machine(stp_frame_t *stp_frame,uint8_t ch);
extern uint16_t verify_stp_frame(stp_frame_t *stp_frame);

extern uint32_t init_stp_frame(stp_frame_t *stp_frame,uint8_t *buf);





extern void stp_thread(void);





#endif //_STP_H_
