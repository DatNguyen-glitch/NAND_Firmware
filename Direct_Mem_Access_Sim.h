#ifndef DIRECT_MEM
#define DIRECT_MEM

#include <pthread.h>
#include <stdio.h>

#define RAM_SIZE (1024 * 1024) // 1MB RAM
uint8_t phys_mem[RAM_SIZE];

typedef struct {
	uint32_t dst_addr;
	uint32_t src_addr;
	uint32_t len;
	volatile uint8_t DMA_en;
} DMA_controller;

void *dma_thread_func(void *DMA_struct);

#endif