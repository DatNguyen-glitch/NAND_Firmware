#ifndef NVM
#define NVM

#include<stdio.h>
#include<stdint.h>
#include<string.h>

#define QUEUE_DEPTH 16

typedef struct {
    uint8_t  opcode;      // 1: Write, 2: Read
    uint32_t lba;         
    uint32_t prp1;        // Buffer Add in RAM (Phys Address)
} NvmeCmd;

void *ssd_poll_and_process_thread(void *arg);
void host_submit_cmd(uint8_t op, uint32_t lba, uint32_t buffer_addr);       // Host sent cmd
void ssd_shutdown();
#endif