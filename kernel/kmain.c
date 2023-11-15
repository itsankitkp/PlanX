#include "io.h"
#include "descriptor_tables.h"
#include "keyboard.h"
#include "multiboot.h"
#include "stdio.h"
#include "paging.h"
#include "tss.h"
void run_modules(multiboot_info_t *mb_info)
{

    /* Check if modules are available */
    multiboot_u32int mods_count = mb_info->mods_count; /* Get the amount of modules available */

    multiboot_u32int mods_addr = mb_info->mods_addr; /* And the starting address of the modules */

    for (u32int mod = 0; mod < mods_count; mod++)
    {
        multiboot_module_t *module = (multiboot_module_t *)(mods_addr + (mod * sizeof(multiboot_module_t))); /* Loop through all modules */
        call_module_t start_program = (call_module_t)module->mod_start;
        enable_page((u32int) start_program, 10);
        //start_program();
        if (start_program)
        {
            enter_user_mode((u32int)module->mod_start);
        }
    }
}

int main(const void *multiboot_struct)
{
    fb_clear_screen();
    init_descriptor_tables();

    fb_write("System is up\0", WHITE);
    if (!multiboot_struct){}
    init_paging();
    enable_page(0x10000000, 10);

//     enable_page(0x00000000, 10);
//     enable_page(0x10000000, 10);
//     enable_page(0xBFFFFFFB, 1);
//      u32int ptr = *(u32int*)0xA0000000;
// ptr++;
//     return 0;
 //run_modules((multiboot_info_t *)multiboot_struct);
}