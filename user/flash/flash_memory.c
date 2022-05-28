#include "product.h"
#include "flash_memory.h"
#include "bsp_spi_flash.h"

	

	
	


uint8_t test_flash_tx_buf[4*1024] = {0};	
uint8_t test_flash_rx_buf[4*1024] = {0};		
	

void test_flash_memory(void)	
{

	/* ªÒ»° Flash Device ID */
	
	
	//u16 xid = SPI_Flash_ReadID();
	//printf("xid is %x\n",xid);
	//while(1);
	
	//DeviceID = SPI_FLASH_ReadDeviceID();	
	//FlashID = SPI_FLASH_ReadID();	
	uint32_t FlashID = flash_read_JEDEC_ID();
	uint32_t flash_size = pow(2,FlashID&0xff);
	//uint32_t flash_size = pow(2,0x15);
	printf("flash size is %d\n",flash_size);
	printf("flash ID is %x\n",FlashID);
	
	
	//test_flash_tx_buf
	//test_flash_rx_buf
	

	//SPI_FLASH_SectorErase(0x000000);//
	flash_sector_erase(0x000000);//
	printf("\n========================================\n");
	printf("start page program\n");
	
		for(int i=0;i<256;i++)
		test_flash_tx_buf[i] = i;
	
	//SPI_FLASH_PageWrite(test_flash_tx_buf, 0x05, 256);
	//flash_page_program(0x05,test_flash_tx_buf,256);
	test_flash_tx_buf[0] = 0x60;
	test_flash_tx_buf[1] = 0x61;
	test_flash_tx_buf[2] = 0x62;
	test_flash_tx_buf[3] = 0x63;
	test_flash_tx_buf[4] = 0x64;
	test_flash_tx_buf[5] = 0x65;
	test_flash_tx_buf[6] = 0x66;
	test_flash_tx_buf[7] = 0x67;
	
	flash_memory_write_data(25,test_flash_tx_buf,8);
	memset(test_flash_rx_buf,0,512);
	//SPI_FLASH_BufferRead(test_flash_rx_buf, 0, 512);
	flash_memory_read_data(25,test_flash_rx_buf,8);
	
		printf("rx is %x %x %x %x %x %x %x %x\n",
		test_flash_rx_buf[0],
		test_flash_rx_buf[1],
		test_flash_rx_buf[2],
		test_flash_rx_buf[3],
		test_flash_rx_buf[4],
		test_flash_rx_buf[5],
		test_flash_rx_buf[6],
		test_flash_rx_buf[7]
		);
	
	
	
	
		
	//SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead);
	//SPI_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)


}





uint32_t flash_memory_read_data(uint32_t addr,uint8_t*array,uint32_t size)
{

     flash_normal_read(addr,test_flash_rx_buf,size);

		 return 0;
}


uint32_t flash_memory_write_data_aligned(uint32_t addr,uint8_t*array,uint32_t size)
{

	if(addr % FLASH_MEMORY_PAGE_SIZE != 0)
		return PDT_STATUS_ERROR;//not aligned

	if(size % FLASH_MEMORY_PAGE_SIZE != 0)
		return PDT_STATUS_ERROR;//not aligned
	
	
	uint32_t total_page = size/FLASH_MEMORY_PAGE_SIZE;
	
	while(total_page)
	{
		//page program
		flash_page_program(addr,array,FLASH_MEMORY_PAGE_SIZE);
		array += FLASH_MEMORY_PAGE_SIZE;
		addr += FLASH_MEMORY_PAGE_SIZE;
		total_page --;
	
	}
	

  return PDT_STATUS_SUCCESS;

}




uint32_t flash_memory_write_data(uint32_t addr,uint8_t*array,uint32_t size)
{

      uint32_t addr_remainder = addr %FLASH_MEMORY_PAGE_SIZE;
	    uint32_t size_remainder = size %FLASH_MEMORY_PAGE_SIZE;
	
	    uint32_t addr_gap = 0;
	    
	    pdt_status_t status;
	    if(addr_remainder == 0)//naturally aligned
			{
					 
				   if(size <= FLASH_MEMORY_PAGE_SIZE)//simple case,data less than a page
					 {
						    flash_page_program(addr,array,size);
					 }
          else//data more than a page
					 {					 
					 
							 status = flash_memory_write_data_aligned(addr,array,size - size_remainder);
							 if(status != PDT_STATUS_SUCCESS)
							 {
								 return status;
							 }
							 
							 if(size_remainder)//still data less than a page size and handle that
							 {    
									 flash_page_program(addr + size - size_remainder,array,size_remainder);
							 }
			     }
			
			}
			else //unaligned
			{
			   addr_gap = FLASH_MEMORY_PAGE_SIZE - addr%FLASH_MEMORY_PAGE_SIZE;
				
				if(size > addr_gap)
				{
				   flash_page_program(addr,array,addr_gap);
					
					 addr += addr_gap;//align addr
					 size -= addr_gap;
					 array +=addr_gap;
					
					 addr_remainder = addr %FLASH_MEMORY_PAGE_SIZE;
	         size_remainder = size %FLASH_MEMORY_PAGE_SIZE;
					
					
					 if(addr_remainder == 0)//after adjustin gap, here should be aligned
			     {
			
							if(size <= FLASH_MEMORY_PAGE_SIZE)//simple case,data less than a page
							 {
										flash_page_program(addr,array,size);
							 }
							else//data more than a page
							 {					 
							 
									 status = flash_memory_write_data_aligned(addr,array,size - size_remainder);
									 if(status != PDT_STATUS_SUCCESS)
									 {
										 return status;
									 }
									 
									 if(size_remainder)//still data less than a page size and handle that
									 {    
											 flash_page_program(addr + size - size_remainder,array,size_remainder);
									 }
							 }
					
			     }
					
					
					
				}
				else
				{
				    flash_page_program(addr,array,size);
				
				}
				
			
			
			}
	
	
			
	
	
	
	

		 return 0;
}




