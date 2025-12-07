#include<stdio.h>
#include<stdint.h>
#include<string.h>

#define NAND_SIZE 			(32*4*4)
#define PAGE_SIZE			(32)
#define PAGE_PER_BLOCK		(4)
#define	BLOCK_PER_DEVICE	(4)

uint8_t	nand_mem[NAND_SIZE];

void nand_init()
{
	memset(nand_mem, 0xFF, NAND_SIZE*sizeof(uint8_t));
}

int nand_read(uint32_t block_idx, uint32_t page_idx, uint8_t *buffer)
{
	if( (block_idx >= BLOCK_PER_DEVICE) || (page_idx >= PAGE_PER_BLOCK) )
		return -1;
		
	uint32_t nand_mem_idx = (block_idx*PAGE_PER_BLOCK + page_idx)*PAGE_SIZE;
	
	if( memcpy(buffer, &nand_mem[nand_mem_idx], PAGE_SIZE*sizeof(uint8_t)) == NULL)
		return -1;
	return 0; 
}