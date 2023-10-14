#include "common.h"
#include "kheap.h"
#include "io.h"
extern u32int endkernel;
u32int page_directory[1024] __attribute__((aligned(4096)));
u32int first_page_table[1024] __attribute__((aligned(4096)));
u32int high_page_table[1024] __attribute__((aligned(4096)));

u32int KERNEL_HIGH_MEM = 0xC0000000;

void init_paging()
{
    //set each entry to not present
int i;
for(i = 0; i < 1024; i++)
{
    // This sets the following flags to the pages:
    //   Supervisor: Only kernel-mode can access them
    //   Write Enabled: It can be both read from and written to
    //   Not Present: The page table is not present
    page_directory[i] = 0x00000002;
}


//we will fill all 1024 entries in the table, mapping 4 megabytes
for(i = 0; i < 1024; i++)
{
    // As the address is page aligned, it will always leave 12 bits zeroed.
    // Those bits are used by the attributes ;)
   first_page_table[i] = (i * 0x1000) | 3; // attributes: supervisor level, read/write, present.
    high_page_table[i] = (i * 0x1000) | 3;
}
//page_directory[0] = (((unsigned int)first_page_table)-KERNEL_HIGH_MEM) | 3;
page_directory[768] = (((unsigned int)high_page_table)-KERNEL_HIGH_MEM) | 3;
u32int page_directory_phy_addr = (unsigned int)page_directory-KERNEL_HIGH_MEM;
loadPageDirectory(page_directory_phy_addr);
enablePaging();
}