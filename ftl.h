#ifndef FTL_CMD
#define FTL_CMD

#include<stdio.h>
#include<stdint.h>
#include<string.h>

#define PAGE_FREE       0
#define PAGE_VALID      1
#define PAGE_INVALID    2
#define MAX_LBA         1024                // LBA (logical block address): HOST manage
#define UNMAPPED_PBA    0xFFFFFFFF
#define UNMAPPED_LBA    0xFFFFFFFF

void ftl_init();
int  ftl_write(uint32_t lba, uint8_t *buffer);
int  ftl_read (uint32_t lba, uint8_t *buffer);
int  get_victim_block();
#endif