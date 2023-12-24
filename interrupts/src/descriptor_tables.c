
#include "common.h"
#include "descriptor_tables.h"

gdt_entry_t gdt_entries[GDTSIZE];
gdt_ptr_t gdt_ptr;
idt_entry_t idt_entries[IDTSIZE];
idt_ptr_t idt_ptr;
// Initialisation routine - zeroes all the interrupt service routines,
// initialises the GDT and IDT.
void init_descriptor_tables()
{
   // Initialise the global descriptor table.
   init_gdt();
   init_idt();
}

void init_gdt()
{
   gdt_ptr.limit = GDTSIZE * 8;
   gdt_ptr.base = (u32int)gdt_entries;

    // NULL Segment, required
    gdt_set_entry(0, 0, 0, 0, 0);
    /* Kernel code, access(9A = 1 00 1 1 0 1 0)
        1   present
        00  ring 0
        1   always 1
        1   code segment
        0   can be executed by ring lower or equal to DPL,
        1   code segment is readable
        0   access bit, always 0, cpu set this to 1 when accessing this sector
    */
    gdt_set_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    /* Kernel data, access(92 = 1 00 1 0 0 1 0)
        Only differ at the fifth bit(counting from least insignificant bit), 0 means it's a data segment.
    */
    gdt_set_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
    // User code and data segments, only differ in ring number(ring 3)
    gdt_set_entry(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
    gdt_set_entry(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);

   memcpy((char *)gdt_ptr.base, (char *)gdt_entries, gdt_ptr.limit);

   gdt_flush((u32int)&gdt_ptr);
}



void init_idt()
{

   idt_ptr.limit = IDTSIZE * 8;
   idt_ptr.base = IDTBASE;

   idt_set_gate((u32int)isr0, 0x08, 0x8E, &idt_entries[0]);
   idt_set_gate((u32int)isr1, 0x08, 0x8E, &idt_entries[1]);
   idt_set_gate((u32int)isr2, 0x08, 0x8E, &idt_entries[2]);
   idt_set_gate((u32int)isr3, 0x08, 0x8E, &idt_entries[3]);
   idt_set_gate((u32int)isr4, 0x08, 0x8E, &idt_entries[4]);
   idt_set_gate((u32int)isr5, 0x08, 0x8E, &idt_entries[5]);
   idt_set_gate((u32int)isr6, 0x08, 0x8E, &idt_entries[6]);
   idt_set_gate((u32int)isr7, 0x08, 0x8E, &idt_entries[7]);
   idt_set_gate((u32int)isr8, 0x08, 0x8E, &idt_entries[8]);
   idt_set_gate((u32int)isr9, 0x08, 0x8E, &idt_entries[9]);
   idt_set_gate((u32int)isr10, 0x08, 0x8E, &idt_entries[10]);
   idt_set_gate((u32int)isr11, 0x08, 0x8E, &idt_entries[11]);
   idt_set_gate((u32int)isr12, 0x08, 0x8E, &idt_entries[12]);
   idt_set_gate((u32int)isr13, 0x08, 0x8E, &idt_entries[13]);
   idt_set_gate((u32int)isr14, 0x08, 0xEE, &idt_entries[14]);
   idt_set_gate((u32int)isr15, 0x08, 0x8E, &idt_entries[15]);
   idt_set_gate((u32int)isr16, 0x08, 0x8E, &idt_entries[16]);
   idt_set_gate((u32int)isr17, 0x08, 0x8E, &idt_entries[17]);
   idt_set_gate((u32int)isr18, 0x08, 0x8E, &idt_entries[18]);
   idt_set_gate((u32int)isr19, 0x08, 0x8E, &idt_entries[19]);
   idt_set_gate((u32int)isr20, 0x08, 0x8E, &idt_entries[20]);
   idt_set_gate((u32int)isr21, 0x08, 0x8E, &idt_entries[21]);
   idt_set_gate((u32int)isr22, 0x08, 0x8E, &idt_entries[22]);
   idt_set_gate((u32int)isr23, 0x08, 0x8E, &idt_entries[23]);
   idt_set_gate((u32int)isr24, 0x08, 0x8E, &idt_entries[24]);
   idt_set_gate((u32int)isr25, 0x08, 0x8E, &idt_entries[25]);
   idt_set_gate((u32int)isr26, 0x08, 0x8E, &idt_entries[26]);
   idt_set_gate((u32int)isr27, 0x08, 0x8E, &idt_entries[27]);
   idt_set_gate((u32int)isr28, 0x08, 0x8E, &idt_entries[28]);
   idt_set_gate((u32int)isr29, 0x08, 0x8E, &idt_entries[29]);
   idt_set_gate((u32int)isr30, 0x08, 0x8E, &idt_entries[30]);
   idt_set_gate((u32int)isr31, 0x08, 0x8E, &idt_entries[31]);
   idt_set_gate((u32int)isr128, 0x08, 0xEE, &idt_entries[128]);


   memcpy((char *)idt_ptr.base, (char *)idt_entries, idt_ptr.limit);

   idt_flush((u32int)&idt_ptr);
}

void idt_set_gate(u32int base, u16int sel, u8int flags, idt_entry_t *idt_entry)
{
   idt_entry->base_lo = base & 0xFFFF;
   idt_entry->base_hi = (base >> 16) & 0xFFFF;

   idt_entry->sel = sel;
   idt_entry->always0 = 0;
   // We must uncomment the OR below when we get to using user-mode.
   // It sets the interrupt gate's privilege level to 3.
   idt_entry->flags = flags | 0x60 ;
}

void gdt_set_entry(int index, u32int base, u32int limit, u8int access, u8int gran) {
    gdt_entry_t * this = &gdt_entries[index];

    // Low 16 bits, middle 8 bits and high 8 bits of base
    this->base_low = base & 0xFFFF;
    this->base_middle = (base >> 16) & 0xFF;
    this->base_high = (base >> 24 & 0xFF);

    /* Low 16 bits and high 4 bits of limit, since the high 4 bits of limits is between granularity and access, and we don't have 4 bit variable,
    low 4 bits of granularity actually represents high 4 bits of limits. It's weird, I know. */
    this->limit_low = limit & 0xFFFF;
    this->granularity = (limit >> 16) & 0x0F;

    this->access = access;

    // Only need the high 4 bits of gran
    this->granularity = this->granularity | (gran & 0xF0);
}