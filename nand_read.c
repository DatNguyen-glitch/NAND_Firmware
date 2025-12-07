#include "nand_read.h"

uint8_t nand_mem[NAND_SIZE]; 

void nand_init()
{
    memset(nand_mem, 0xFF, NAND_SIZE*sizeof(uint8_t));
} 

int nand_read(uint32_t block_idx, uint32_t page_idx, uint8_t *buffer)
{
    if( (block_idx >= BLOCK_PER_DEVICE) || (page_idx >= PAGE_PER_BLOCK))
        return -1;

    uint32_t nand_mem_idx = (block_idx*PAGE_PER_BLOCK + page_idx)*PAGE_SIZE;

    if( memcpy(buffer, &nand_mem[nand_mem_idx], PAGE_SIZE*sizeof(uint8_t)) == NULL)
        return -1;

    return 0; 
} 

int nand_program(uint32_t block_idx, uint32_t page_idx, uint8_t *buffer)
{
    if( (block_idx >= BLOCK_PER_DEVICE) || (page_idx >= PAGE_PER_BLOCK))
        return -1;

    uint32_t nand_mem_idx = (block_idx*PAGE_PER_BLOCK + page_idx)*PAGE_SIZE;

    for (uint32_t i = 0; i < PAGE_SIZE; i++)
    {
        nand_mem[nand_mem_idx + i] &= buffer[i];
    }
    return 0;
}

int nand_erase(uint32_t block_idx)
{
    if( block_idx >= BLOCK_PER_DEVICE )
        return -1;

    uint32_t nand_mem_idx = block_idx * PAGE_PER_BLOCK * PAGE_SIZE;

    memset(&nand_mem[nand_mem_idx], 0xFF, PAGE_PER_BLOCK * PAGE_SIZE * sizeof(uint8_t));

    return 0;
}
