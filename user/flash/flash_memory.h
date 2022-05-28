#ifndef __FLASH_MEMORY_H__
#define __FLASH_MEMORY_H__
#include<stdint.h>

uint32_t flash_memory_read_data(uint32_t addr,uint8_t*array,uint32_t size);
uint32_t flash_memory_write_data(uint32_t addr,uint8_t*array,uint32_t size);
void test_flash_memory(void);







#endif
