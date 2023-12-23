#include "io.h"
#include "descriptor_tables.h"
#include "keyboard.h"
#include "multiboot.h"
#include "stdio.h"
#include "paging.h"
#include "tss.h"
#include "kheap.h"
#include "process.h"

void run_modules(multiboot_info_t *mb_info)
{

    /* Check if modules are available */
    multiboot_uint32_t mods_count = mb_info->mods_count; /* Get the amount of modules available */

    multiboot_uint32_t mods_addr = mb_info->mods_addr; /* And the starting address of the modules */
    process_t *p1, *p2;
    for (u32int mod = 0; mod < mods_count; mod++)
    {
        multiboot_module_t *module = (multiboot_module_t *)(mods_addr + (mod * sizeof(multiboot_module_t))); /* Loop through all modules */
        call_module_t start_program = (call_module_t)module->mod_start;
        if (mod == 0)
        {
            if (start_program)
            {
                u32int size = (call_module_t)module->mod_end - (call_module_t)module->mod_start;
                p1 = (process_t *)kmalloc_a(sizeof(process_t), TRUE);
                init_user_space_paging(p1);
                // __asm__ ("int $0x80");

                invlpg(0);
                invlpg(0xC0000000);
                memcpy(0, (char *)start_program, size);
                // enter_user_mode();
            }
        }
        else
        {
            if (start_program)
            {
                u32int size = (call_module_t)module->mod_end - (call_module_t)module->mod_start;
                p2 = (process_t *)kmalloc_a(sizeof(process_t), TRUE);
                init_user_space_paging(p2);
                // __asm__ ("int $0x80");

                invlpg(0);
                invlpg(0xC0000000);
                memcpy(0, (char *)start_program, size);
                // enter_user_mode();
            }
        }
    }
    u32int page_directory_phy_addr = (u32int)&p2->pd - 0xC0000000;
    loadPageDirectory(page_directory_phy_addr);
    enter_user_mode();
}

int main(const void *multiboot_struct)
{
    fb_clear_screen();
    init_descriptor_tables();

    fb_write("System is up\0", WHITE);
    tss_init(5, 0x10, 0x10);
    if (!multiboot_struct)
    {
    }
    init_paging();

    //     enable_page(0x00000000, 10);
    //     enable_page(0x10000000, 10);
    //     enable_page(0xA0000000, 1);
    //      u32int ptr = *(u32int*)0xA0000000;
    // ptr++;
    //     return 0;
    run_modules((multiboot_info_t *)multiboot_struct);
}