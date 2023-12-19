#include "common.h"
#include "kheap.h"
#include "io.h"
#include "process.h"
extern u32int endkernel;
u32int page_directory[1024] __attribute__((aligned(4096)));
u32int first_page_table[1024] __attribute__((aligned(4096)));
u32int first_page_table2[1024] __attribute__((aligned(4096)));
u32int first_page_table3[1024] __attribute__((aligned(4096)));
u32int first_page_table4[1024] __attribute__((aligned(4096)));
u32int first_page_table5[1024] __attribute__((aligned(4096)));
u32int high_page_table[1024] __attribute__((aligned(4096)));
u32int high_page_table2[1024] __attribute__((aligned(4096)));
u32int high_page_table3[1024] __attribute__((aligned(4096)));
u32int high_page_table4[1024] __attribute__((aligned(4096)));
u32int allocated_phy_addr=0x0;
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
   first_page_table2[i] = (i * 0x1000) | 3;
   first_page_table3[i] = (i * 0x1000) | 3;
   first_page_table4[i] = (i * 0x1000) | 3;
   first_page_table5[i] = (i * 0x1000) | 3;
    high_page_table[i] = (i * 0x1000) | 3;
        high_page_table2[i] = (i * 0x1000) | 3;

    high_page_table3[i] = (i * 0x1000) | 3;
    high_page_table4[i] = (i * 0x1000) | 3;

    allocated_phy_addr += (i * 0x1000);
}
page_directory[0] = (((unsigned int)first_page_table)-KERNEL_HIGH_MEM) | 3;
page_directory[1] = (((unsigned int)first_page_table2)-KERNEL_HIGH_MEM) | 3;
page_directory[2] = (((unsigned int)first_page_table3)-KERNEL_HIGH_MEM) | 3;
page_directory[3] = (((unsigned int)first_page_table4)-KERNEL_HIGH_MEM) | 3;
page_directory[4] = (((unsigned int)first_page_table5)-KERNEL_HIGH_MEM) | 3;
page_directory[768] = (((unsigned int)high_page_table)-KERNEL_HIGH_MEM) | 3;
page_directory[769] = (((unsigned int)high_page_table2)-KERNEL_HIGH_MEM) | 3;
page_directory[770] = (((unsigned int)high_page_table3)-KERNEL_HIGH_MEM) | 3;
u32int page_directory_phy_addr = (unsigned int)page_directory-KERNEL_HIGH_MEM;
loadPageDirectory(page_directory_phy_addr);

}

void enable_page(u32int addr, u32int count)
{
    static u32int page[1024] __attribute__((aligned(4096)));

    u32int pde = addr >> 22;
    u32int i;
    u32int offset =allocated_phy_addr;
    for (i=0; i<count; i++)
    {
        page[i] = (i*1000 + offset) |3;
        allocated_phy_addr+=1000;
    }
    page_directory[pde] =(((unsigned int)page)-KERNEL_HIGH_MEM) | 3;

   invlpg(addr);


}

// u32int user_page_directory[1024] __attribute__((aligned(4096)));
u32int user_page_table1[1024] __attribute__((aligned(4096)));
u32int user_page_table2[1024] __attribute__((aligned(4096)));
u32int user_page_table1023[1024] __attribute__((aligned(4096)));



void init_user_space_paging(process_t* p)
{
     //set each entry to not present
int i;

for(i = 0; i < 1024; i++)
{
    // This sets the following flags to the pages:
    //   Supervisor: Only kernel-mode can access them
    //   Write Enabled: It can be both read from and written to
    //   Not Present: The page table is not present
    p->user_page_directory[i] = 0x00000004;
}


//we will fill all 1024 entries in the table, mapping 4 megabytes
for(i = 0; i < 1024; i++)
{
    // As the address is page aligned, it will always leave 12 bits zeroed.
    // Those bits are used by the attributes ;)
   user_page_table1[i] = (i * 0x1000) | 7; // attributes: supervisor level, read/write, present.
    user_page_table2[i] = (i * 0x1000) | 7;
    user_page_table1023[i] = (i * 0x1000) | 7;
    allocated_phy_addr += (i * 0x1000);
}
p->user_page_directory[0] = (((unsigned int)user_page_table1)-KERNEL_HIGH_MEM) | 7;
p->user_page_directory[1] = (((unsigned int)user_page_table2)-KERNEL_HIGH_MEM) | 7;
p->user_page_directory[768] = (((unsigned int)high_page_table)-KERNEL_HIGH_MEM) | 7;
p->user_page_directory[769] = (((unsigned int)high_page_table2)-KERNEL_HIGH_MEM) | 7;
p->user_page_directory[770] = (((unsigned int)high_page_table3)-KERNEL_HIGH_MEM) | 7;
p->user_page_directory[1023] = (((unsigned int)user_page_table1023)-KERNEL_HIGH_MEM) | 7;

u8int page_directory_phy_addr = (u8int)&p->user_page_directory-KERNEL_HIGH_MEM;
loadPageDirectory(page_directory_phy_addr);


}
