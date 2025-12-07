#ifndef FTL_CMD
#define FTL_CMD

#include<stdio.h>
#include<stdint.h>
#include<string.h>

void ftl_init();
int  ftl_write(uint32_t lba, uint8_t *buffer);
int  ftl_read (uint32_t lba, uint8_t *buffer);

#endif