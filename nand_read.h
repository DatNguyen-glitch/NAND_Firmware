#ifndef NAND_CMD
#define NAND_CMD

#include<stdio.h>
#include<stdint.h>
#include<string.h>

#define NAND_SIZE (32*4*4)
#define PAGE_SIZE (32)
#define PAGE_PER_BLOCK (4)
#define BLOCK_PER_DEVICE (4)

void nand_init();
int  nand_read   (uint32_t block_idx, uint32_t page_idx, uint8_t *buffer);
int  nand_program(uint32_t block_idx, uint32_t page_idx, uint8_t *buffer);
int  nand_erase  (uint32_t block_idx);

#endif