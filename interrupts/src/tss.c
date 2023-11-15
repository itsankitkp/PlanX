// #include "tss.h"
// #include "descriptor_tables.h"

// tss_entry_t kernel_tss;


// void tss_init(u32int kss, u32int kesp) {
//     u32int base = (u32int)&kernel_tss;
    
//     /* Kernel tss, access(E9 = 1 11 0 1 0 0 1)
//         1   present
//         11  ring 3
//         0   should always be 1, why 0? may be this value doesn't matter at all
//         1   code?
//         0   can not be executed by ring lower or equal to DPL,
//         0   not readable
//         1   access bit, always 0, cpu set this to 1 when accessing this sector(why 0 now?)
//     */
//     memset((char*) &kernel_tss, 0, sizeof(tss_entry_t));
//     kernel_tss.ss0 = kss;
//     // Note that we usually set tss's esp to 0 when booting our os, however, we need to set it to the real esp when we've switched to usermode because
//     // the CPU needs to know what esp to use when usermode app is calling a kernel function(aka system call), that's why we have a function below called tss_set_stack
//     kernel_tss.esp0 = kesp;
//     kernel_tss.cs = 0x0b;
//     kernel_tss.ds = 0x13;
//     kernel_tss.es = 0x13;
//     kernel_tss.fs = 0x13;
//     kernel_tss.gs = 0x13;
//     kernel_tss.ss = 0x13;
//     gdt_set_gate(base, base + sizeof(tss_entry_t), 0xE9, 0, (gdt_entry_t*) &kernel_tss);
//     tss_flush();
// }

// /*
//  * This function is used to set the tss's esp, so that CPU knows what esp the kernel should be using
//  * */
// void tss_set_stack(u32int kss, u32int kesp) {
//     kernel_tss.ss0 = kss;
//     kernel_tss.esp0 = kesp;
// }