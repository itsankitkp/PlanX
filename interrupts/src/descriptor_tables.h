#include "common.h"

struct gdt_entry_bits {
	u32int limit_low              : 16;
	u32int base_low               : 24;
	u32int accessed               :  1;
	u32int read_write             :  1; // readable for code, writable for data
	u32int conforming_expand_down :  1; // conforming for code, expand down for data
	u32int code                   :  1; // 1 for code, 0 for data
	u32int code_data_segment      :  1; // should be 1 for everything but TSS and LDT
	u32int DPL                    :  2; // privilege level
	u32int present                :  1;
	u32int limit_high             :  4;
	u32int available              :  1; // only used in software; has no effect on hardware
	u32int long_mode              :  1;
	u32int big                    :  1; // 32-bit opcodes for code, u32int stack for data
	u32int gran                   :  1; // 1 to use 4k page addressing, 0 for byte addressing
	u32int base_high              :  8;
} __attribute__((packed));
typedef struct gdt_entry_bits gdt_entry_t;

struct tss_entry_struct {
	u32int prev_tss; // The previous TSS - with hardware task switching these form a kind of backward linked list.
	u32int esp0;     // The stack pointer to load when changing to kernel mode.
	u32int ss0;      // The stack segment to load when changing to kernel mode.
	// Everything below here is unused.
	u32int esp1; // esp and ss 1 and 2 would be used when switching to rings 1 or 2.
	u32int ss1;
	u32int esp2;
	u32int ss2;
	u32int cr3;
	u32int eip;
	u32int eflags;
	u32int eax;
	u32int ecx;
	u32int edx;
	u32int ebx;
	u32int esp;
	u32int ebp;
	u32int esi;
	u32int edi;
	u32int es;
	u32int cs;
	u32int ss;
	u32int ds;
	u32int fs;
	u32int gs;
	u32int ldt;
	u16int trap;
	u16int iomap_base;
} __attribute__((packed));;
 
typedef struct tss_entry_struct tss_entry_t;
struct gdt_ptr_struct
{
   u16int limit; // The upper 16 bits of all selector limits.
   u32int base;  // The address of the first gdt_entry_t struct.
}
__attribute__((packed));
typedef struct gdt_ptr_struct gdt_ptr_t;
// Initialisation function is publicly accessible.
void init_descriptor_tables();

// A struct describing an interrupt gate.
struct idt_entry_struct
{
   u16int base_lo; // The lower 16 bits of the address to jump to when this interrupt fires.
   u16int sel;     // Kernel segment selector.
   u8int always0;  // This must always be zero.
   u8int flags;    // More flags. See documentation.
   u16int base_hi; // The upper 16 bits of the address to jump to.
} __attribute__((packed));
typedef struct idt_entry_struct idt_entry_t;

// A struct describing a pointer to an array of interrupt handlers.
// This is in a format suitable for giving to 'lidt'.
struct idt_ptr_struct
{
   u16int limit;
   u32int base; // The address of the first element in our idt_entry_t array.
} __attribute__((packed));
typedef struct idt_ptr_struct idt_ptr_t;

#define GDTBASE 0xC0000800
#define GDTSIZE 6 // number of gdt entries

#define IDTBASE 0xC0000A00
#define IDTSIZE 32
// These extern directives let us access the addresses of our ASM ISR handlers.

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

void init_idt();
void idt_set_gate(u32int base, u16int sel, u8int flags, idt_entry_t* idt_entry);

// Lets us access our ASM functions from our C code.
extern void gdt_flush(u32int);
// Lets us access our ASM functions from our C code.
extern void idt_flush(u32int);

// Internal function prototypes.
void init_gdt();
void install_tss(gdt_entry_t *g);
extern void tss_flush();