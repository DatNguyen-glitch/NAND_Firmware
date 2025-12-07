#include "ftl.h"
#include "nand_read.h"

#define MAX_LBA 1024
#define UNMAPPED_PBA 0xFFFFFFFF

uint32_t l2p_table[MAX_LBA];
uint32_t free_blk_idx = 0;
uint32_t free_page_idx = 0;

void ftl_init()
{
    memset(l2p_table, UNMAPPED_PBA, MAX_LBA*sizeof(uint32_t));
}

int ftl_write(uint32_t lba, uint8_t *buffer)
{
    if( lba >= MAX_LBA)
    {
        return -1;          // LBA Out of range Err
    }
    if (free_blk_idx >= BLOCK_PER_DEVICE)
    {
        return -2;          // Disk Full Err
    }
    nand_program(free_blk_idx, free_page_idx, buffer);
    l2p_table[lba] = free_blk_idx * PAGE_PER_BLOCK + free_page_idx;

    free_page_idx++;
    if (free_page_idx >= PAGE_PER_BLOCK)
    {
        free_page_idx = 0;
        free_blk_idx++;
    }
    return 0;
}

int ftl_read(uint32_t lba, uint8_t *buffer)
{
    if (lba >= MAX_LBA)
    {
        return -1;
    }
   
    uint32_t blk_idx = 0;
    uint32_t page_idx = 0;    
    if( l2p_table[lba] == UNMAPPED_PBA) // No data
    {
        memset(buffer, 0xFF, PAGE_SIZE * sizeof(uint8_t));        
    }
    else
    {
        blk_idx = l2p_table[lba] / PAGE_PER_BLOCK;
        page_idx = l2p_table[lba] % PAGE_PER_BLOCK;
        nand_read(blk_idx, page_idx, buffer);
    }
    return 0;
}