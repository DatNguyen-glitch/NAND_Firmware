#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "nvm.h"
#include "ftl.h"
#include "nand_read.h"
#include "Direct_Mem_Access_Sim.h" // Để dùng mảng phys_mem

int main()
{
    pthread_t ssd_thread;
    printf("=== SSD SIMULATION SYSTEM START ===\n");

    // 1. Khởi tạo hệ thống
    nand_init();
    ftl_init();
    printf("[System] Init Done.\n");

    pthread_create(&ssd_thread, NULL, ssd_poll_and_process_thread, NULL);

    uint32_t write_buffer_addr = 0; 
    uint8_t *host_write_ptr = &phys_mem[write_buffer_addr];
    
    // Điền dữ liệu mẫu: "Hello SSD World!"
    sprintf((char*)host_write_ptr, "Hello SSD World! This is LBA 5");

    printf("[Host] Submitting WRITE CMD to LBA 5...\n");
    host_submit_cmd(0x01, 5, write_buffer_addr);
    printf("[Host] Submitting WRITE CMD to LBA 15...\n");
    host_submit_cmd(0x01, 15, write_buffer_addr);
    uint32_t read_buffer_addr = 1024;
    printf("[Host] Submitting READ CMD from LBA 15...\n");
    host_submit_cmd(0x02, 15, read_buffer_addr);

    // // 7. So sánh kết quả (Verify)
    // printf("[System] Verifying Data...\n");
    // printf("   Original: %s\n", host_write_ptr);
    // printf("   Readback: %s\n", &phys_mem[read_buffer_addr]);

    // if (strcmp((char*)host_write_ptr, (char*)&phys_mem[read_buffer_addr]) == 0)
    // {
    //     printf("=== SUCCESS: Data Matches! ===\n");
    // }
    // else
    // {
    //     printf("=== FAILED: Data Mismatch! ===\n");
    // }

    ssd_shutdown();
    pthread_join(ssd_thread, NULL);
    return 0;
}