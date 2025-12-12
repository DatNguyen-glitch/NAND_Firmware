#include <unistd.h>
#include "nvm.h"
#include "ftl.h"

extern uint8_t phys_mem[];
// Host global variables
NvmeCmd sq[QUEUE_DEPTH];
uint32_t sq_tail = 0;
uint32_t sq_head = 0;
volatile uint8_t  ssd_running = 0x01;   // 0x01 = Run, 0x00 = Shutdown
volatile uint32_t sq_doorbell = 0;      // SSD Register Sim

void host_submit_cmd(uint8_t op, uint32_t lba, uint32_t buffer_addr)
{
    sq[sq_tail].opcode  = op;
    sq[sq_tail].lba     = lba;
    sq[sq_tail].prp1    = buffer_addr;

    sq_tail     = (sq_tail + 1) % QUEUE_DEPTH;
    sq_doorbell = sq_tail;    // enable Doorbell
}

void *ssd_poll_and_process_thread(void *arg)
{
    printf("[SSD Thread] Controller Started...\n");
    NvmeCmd cmd = {0};
    while (ssd_running == 0x01)
    {
        if (sq_head != sq_doorbell)
        {
            cmd = sq[sq_head];

            if (cmd.opcode == 0x01)         // Write
            {
                printf("[SSD] Processing WRITE LBA %d...\n", cmd.lba);
                ftl_write(cmd.lba, &phys_mem[cmd.prp1]);
            }
            else if (cmd.opcode == 0x02)    // Read
            {
                printf("[SSD] Processing READ LBA %d...\n", cmd.lba);
                ftl_read(cmd.lba, &phys_mem[cmd.prp1]);
            }
            sq_head = (sq_head + 1) % QUEUE_DEPTH;
        }
        else
        {
            usleep(50);
        }

        printf("SSD: Executed Op:%d LBA:%d\n", cmd.opcode, cmd.lba);
    }
}

void ssd_shutdown()
{
    ssd_running = 0x00;
}