#include "io.h"
#include "descriptor_tables.h"
#include "keyboard.h"
#include "multiboot.h"
#include "stdio.h"

void run_modules(multiboot_info_t *mb_info)
{

    /* Check if modules are available */
    multiboot_uint32_t mods_count = mb_info->mods_count; /* Get the amount of modules available */

    multiboot_uint32_t mods_addr = mb_info->mods_addr; /* And the starting address of the modules */

    for (u32int mod = 0; mod < mods_count; mod++)
    {
        multiboot_module_t *module = (multiboot_module_t *)(mods_addr + (mod * sizeof(multiboot_module_t))); /* Loop through all modules */
        call_module_t start_program = (call_module_t)module->mod_start;
        start_program();
    }
}

int main(const void *multiboot_struct)
{
    fb_clear_screen();

    fb_write("System is up\n", WHITE);
    init_descriptor_tables();
    //run_modules((multiboot_info_t *)multiboot_struct);
   // asm volatile ("int $0x8");
}