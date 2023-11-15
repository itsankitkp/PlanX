

// #ifndef TSS_H
// #define TSS_H
// #include "common.h"


// struct tss_entry_struct {
// 	u32int prev_tss; // The previous TSS - with hardware task switching these form a kind of backward linked list.
// 	u32int esp0;     // The stack pointer to load when changing to kernel mode.
// 	u32int ss0;      // The stack segment to load when changing to kernel mode.
// 	// Everything below here is unused.
// 	u32int esp1; // esp and ss 1 and 2 would be used when switching to rings 1 or 2.
// 	u32int ss1;
// 	u32int esp2;
// 	u32int ss2;
// 	u32int cr3;
// 	u32int eip;
// 	u32int eflags;
// 	u32int eax;
// 	u32int ecx;
// 	u32int edx;
// 	u32int ebx;
// 	u32int esp;
// 	u32int ebp;
// 	u32int esi;
// 	u32int edi;
// 	u32int es;
// 	u32int cs;
// 	u32int ss;
// 	u32int ds;
// 	u32int fs;
// 	u32int gs;
// 	u32int ldt;
// 	u16int trap;
// 	u16int iomap_base;
// } __attribute__((packed));
 
// typedef struct tss_entry_struct tss_entry_t;

// extern void tss_flush();

// void tss_init(u32int kss, u32int kesp);

// void tss_set_stack(u32int kss, u32int kesp);
// #endif