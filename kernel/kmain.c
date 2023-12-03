#include "io.h"
#include "descriptor_tables.h"
#include "keyboard.h"
#include "multiboot.h"
#include "stdio.h"
#include "paging.h"
#include "tss.h"
#include "kheap.h"
void run_modules(multiboot_info_t *mb_info)
{

    /* Check if modules are available */
    multiboot_uint32_t mods_count = mb_info->mods_count; /* Get the amount of modules available */

    multiboot_uint32_t mods_addr = mb_info->mods_addr; /* And the starting address of the modules */

    for (u32int mod = 0; mod < mods_count; mod++)
    {
        multiboot_module_t *module = (multiboot_module_t *)(mods_addr + (mod * sizeof(multiboot_module_t))); /* Loop through all modules */
        call_module_t start_program = (call_module_t)module->mod_start;

        if (start_program)
        {
            init_user_space_paging();
            // __asm__ ("int $0x80");
            
            invlpg(0);
            invlpg(0xC0000000);
            memcpy(0, (char*) start_program, (call_module_t)module->mod_end-(call_module_t)module->mod_start);
            enter_user_mode();
       

        }
    }
}

int main(const void *multiboot_struct)
{
    fb_clear_screen();
    init_descriptor_tables();
    
    fb_write("System is up\0", WHITE);
    tss_init(5, 0x10, 0x10);
    if (!multiboot_struct){}
    init_paging();
    
//     enable_page(0x00000000, 10);
//     enable_page(0x10000000, 10);
//     enable_page(0xA0000000, 1);
//      u32int ptr = *(u32int*)0xA0000000;
// ptr++;
//     return 0;
 run_modules((multiboot_info_t *)multiboot_struct);
}