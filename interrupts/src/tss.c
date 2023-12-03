#include "common.h"
#include "tss.h"
#include "descriptor_tables.h"
tss_entry_t kernel_tss;

/*
 * We don't need tss to assist task switching, but it's required to have one tss for switching back to kernel mode(system call for example)
 *
 * */
void tss_init(u32int idx, u32int kss, u32int kesp) {
    u32int base = (u32int)&kernel_tss;
    gdt_set_entry(idx, base, base + sizeof(tss_entry_t), 0xE9, 0);
    /* Kernel tss, access(E9 = 1 11 0 1 0 0 1)
        1   present
        11  ring 3
        0   should always be 1, why 0? may be this value doesn't matter at all
        1   code?
        0   can not be executed by ring lower or equal to DPL,
        0   not readable
        1   access bit, always 0, cpu set this to 1 when accessing this sector(why 0 now?)
    */
    memset((char*)base, 0, sizeof(tss_entry_t));
    kernel_tss.ss0 = kss;
    // Note that we usually set tss's esp to 0 when booting our os, however, we need to set it to the real esp when we've switched to usermode because
    // the CPU needs to know what esp to use when usermode app is calling a kernel function(aka system call), that's why we have a function below called tss_set_stack
    kernel_tss.esp0 = kesp;
    kernel_tss.cs = 0x08;
    kernel_tss.ds = 0x10;
    kernel_tss.es = 0x10;
    kernel_tss.fs = 0x10;
    kernel_tss.gs = 0x10;
    kernel_tss.ss = 0x10;
    tss_flush();
}

/*
 * This function is used to set the tss's esp, so that CPU knows what esp the kernel should be using
 * */
void tss_set_stack(u32int kss, u32int kesp) {
    kernel_tss.ss0 = kss;
    kernel_tss.esp0 = kesp;
}
