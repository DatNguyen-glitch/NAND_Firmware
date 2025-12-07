#include "ftl.h"
#include "nand_read.h"

#define MAX_LBA 1024
#define UNMAPPED_PBA 0xFFFFFFFF
#define UNMAPPED_LBA 0xFFFFFFFF

uint32_t l2p_table[MAX_LBA];
uint32_t p2l_table[BLOCK_PER_DEVICE][PAGE_PER_BLOCK];
uint32_t free_blk_idx = 0;
uint32_t free_page_idx = 0;
uint8_t  page_status_table[BLOCK_PER_DEVICE][PAGE_PER_BLOCK];   // To track page status: FREE, VALID, INVALID

void ftl_init()
{
    memset(l2p_table, UNMAPPED_PBA, MAX_LBA*sizeof(uint32_t));
    memset(p2l_table, UNMAPPED_LBA, BLOCK_PER_DEVICE*PAGE_PER_BLOCK*sizeof(uint32_t));
    memset(page_status_table, PAGE_FREE, BLOCK_PER_DEVICE*PAGE_PER_BLOCK*sizeof(uint8_t));
    free_blk_idx = 0;
    free_page_idx = 0;
}

int ftl_write(uint32_t lba, uint8_t *buffer)        // lba (logical block address)
{
    if( lba >= MAX_LBA)
    {
        return -1;          // LBA Out of range Err
    }
    if (free_blk_idx >= BLOCK_PER_DEVICE)
    {
        return -2;          // Disk Full Err
    }

    uint32_t blk_idx = l2p_table[lba] / PAGE_PER_BLOCK;
    uint32_t page_idx = l2p_table[lba] % PAGE_PER_BLOCK;
    if( l2p_table[lba] != UNMAPPED_PBA ) // Existing valid data
    {
        page_status_table[blk_idx][page_idx] = PAGE_INVALID;        // Mark old page as INVALID
    }
    nand_program(free_blk_idx, free_page_idx, buffer);
    p2l_table[free_blk_idx][free_page_idx] = lba;
    l2p_table[lba] = free_blk_idx * PAGE_PER_BLOCK + free_page_idx;
    page_status_table[free_blk_idx][free_page_idx] = PAGE_VALID;    // Mark new page as VALID

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

int get_victim_block()
{
    int max_invalid_count = 0;
    int victim_blk = -1;

    for (uint32_t blk = 0; blk < BLOCK_PER_DEVICE; blk++)
    {
        int invalid_count = 0;
        for (uint32_t pg = 0; pg < PAGE_PER_BLOCK; pg++)
        {
            if (page_status_table[blk][pg] == PAGE_INVALID)
            {
                invalid_count++;
            }
        }
        if (invalid_count > max_invalid_count)
        {
            max_invalid_count = invalid_count;
            victim_blk = blk;
        }
    }

    return victim_blk;
}

void ftl_gc()
{
    int victim_blk = get_victim_block();
    if (victim_blk == -1) 
        return;

    uint8_t temp_buffer[4096]; // Giả sử page size 4KB

    // 2. Di chuyển dữ liệu Valid (Valid Page Move)
    // ... Viết code của bạn ở đây ...

    // 3. Xóa Block cũ
    // ...

    // 4. Xử lý Free Block index (Logic đơn giản hóa: reset free block về đây)
    free_blk_idx = victim_blk;
    free_page_idx = 0;
    
    // Lưu ý: Trong thực tế, ta cần Free List để quản lý block trống, 
    // nhưng ở đây ta ép luôn con trỏ ghi về block vừa xóa để test cho dễ.
}