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

void *dma_thread_func(void *DMA_struct)
{
	DMA_controller *dma = (DMA_controller *) DMA_struct;
	while(1)
	{
		if(dma->DMA_en == 1)
		{	
			for(uint32_t i = 0; i < (dma->len); i++)
			{
				phys_mem[(dma->dst_addr) + i] = phys_mem[(dma->src_addr) + i];
			}
			(dma->DMA_en) = 0;
		}
		usleep(1000);
	}
}

void main(){
	pthread_t dma_thread;
	DMA_controller dma = {};
	DMA_controller *dma_ptr = &dma; 
	
	uint32_t mem_src_addr = 100;
	for (uint32_t i; i < 100; i++)
	{
		phys_mem[mem_src_addr + i] = i;
	}
	
	dma_ptr->src_addr 	= 100;
	dma_ptr->dst_addr 	= 150;
	dma_ptr->len		= 50;
	dma_ptr->DMA_en 	= 1;
	pthread_create(&dma_thread, NULL, dma_thread_func, (void*)dma_ptr);
	while(1)
	{
		if(dma_ptr->DMA_en == 0)
		{
			for(uint32_t j = dma_ptr->src_addr; j < dma_ptr->len; j++)
			{
				if(phys_mem[dma_ptr->src_addr] != phys_mem[dma_ptr->dst_addr])
					printf("Data mismatch");
					break;
			}
			printf("Data match");
			break;
		}
	}
	return;
}