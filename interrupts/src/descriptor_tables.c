
#include "common.h"
#include "descriptor_tables.h"

gdt_entry_t gdt_entries[GDTSIZE];
gdt_ptr_t gdt_ptr;
idt_entry_t idt_entries[IDTSIZE];
idt_ptr_t idt_ptr;
tss_entry_t tss_entry;
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
   gdt_ptr.base = GDTBASE;
   gdt_entry_t *null_segment = &gdt_entries[0];
   // ring 0
   gdt_entry_t *kernel_code_segment = &gdt_entries[1];
   gdt_entry_t *kernel_data_segment = &gdt_entries[2];

   // ring 3
   gdt_entry_t *user_code_segment = &gdt_entries[3];
   gdt_entry_t *user_data_segment = &gdt_entries[4];

   null_segment->limit_low = 0;
   null_segment->base_low = 0;
   null_segment->accessed = 0;
   null_segment->read_write = 0;             // since this is a code segment, specifies that the segment is readable
   null_segment->conforming_expand_down = 0; // does not matter for ring 3 as no lower privilege level exists
   null_segment->code = 0;
   null_segment->code_data_segment = 1;
   null_segment->DPL = 0; // ring 3
   null_segment->present = 0;
   null_segment->limit_high = 0;
   null_segment->available = 0;
   null_segment->long_mode = 0;
   null_segment->big = 0;  // it's 32 bits
   null_segment->gran = 0; // 4KB page addressing
   null_segment->base_high = 0;

   kernel_code_segment->limit_low = 0xFFFF;
   kernel_code_segment->base_low = 0;
   kernel_code_segment->accessed = 0;
   kernel_code_segment->read_write = 1;             // since this is a code segment, specifies that the segment is readable
   kernel_code_segment->conforming_expand_down = 0; // does not matter for ring 3 as no lower privilege level exists
   kernel_code_segment->code = 1;
   kernel_code_segment->code_data_segment = 1;
   kernel_code_segment->DPL = 0; // ring 0 [=]
   kernel_code_segment->present = 1;
   kernel_code_segment->limit_high = 0xF;
   kernel_code_segment->available = 1;
   kernel_code_segment->long_mode = 0;
   kernel_code_segment->big = 1;  // it's 32 bits
   kernel_code_segment->gran = 1; // 4KB page addressing
   kernel_code_segment->base_high = 0;

   *kernel_data_segment = *kernel_code_segment; // contents are similar so save time by copying
   kernel_data_segment->code = 0;               // not code but data

   user_code_segment->limit_low = 0xFFFF;
   user_code_segment->base_low = 0;
   user_code_segment->accessed = 0;
   user_code_segment->read_write = 1;             // since this is a code segment, specifies that the segment is readable
   user_code_segment->conforming_expand_down = 0; // does not matter for ring 3 as no lower privilege level exists
   user_code_segment->code = 1;
   user_code_segment->code_data_segment = 1;
   user_code_segment->DPL = 3; // ring 3
   user_code_segment->present = 1;
   user_code_segment->limit_high = 0xF;
   user_code_segment->available = 1;
   user_code_segment->long_mode = 0;
   user_code_segment->big = 1;  // it's 32 bits
   user_code_segment->gran = 1; // 4KB page addressing
   user_code_segment->base_high = 0;

   *user_data_segment = *user_code_segment; // contents are similar so save time by copying
   user_data_segment->code = 0;             // not code but data

   memcpy((char *)gdt_ptr.base, (char *)gdt_entries, gdt_ptr.limit);

   gdt_flush((u32int)&gdt_ptr);

   install_tss(&gdt_entries[5]);
   tss_flush();
}

void install_tss(gdt_entry_t *g)
{
   // Compute the base and limit of the TSS for use in the GDT entry.
   u32int base = (u32int)&tss_entry;
   u32int limit = sizeof(tss_entry);

   // Add a TSS descriptor to the GDT.
   g->limit_low = limit;
   g->base_low = base;
   g->accessed = 1;               // With a system entry (`code_data_segment` = 0), 1 indicates TSS and 0 indicates LDT
   g->read_write = 0;             // For a TSS, indicates busy (1) or not busy (0).
   g->conforming_expand_down = 0; // always 0 for TSS
   g->code = 1;                   // For a TSS, 1 indicates 32-bit (1) or 16-bit (0).
   g->code_data_segment = 0;      // indicates TSS/LDT (see also `accessed`)
   g->DPL = 0;                    // ring 0, see the comments below
   g->present = 1;
   g->limit_high = (limit & (0xf << 16)) >> 16; // isolate top nibble
   g->available = 0;                            // 0 for a TSS
   g->long_mode = 0;
   g->big = 0;                                 // should leave zero according to manuals.
   g->gran = 0;                                // limit is in bytes, not pages
   g->base_high = (base & (0xff << 24)) >> 24; // isolate top byte

   // Ensure the TSS is initially zero'd.
   memset((char *)&tss_entry, 0, sizeof(tss_entry));

   tss_entry.ss0 = 0x10; // Set the kernel stack segment.
   tss_entry.esp0 = 0;   // Set the kernel stack pointer.
                         // note that CS is loaded from the IDT entry and should be the regular kernel code segment
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
   idt_set_gate((u32int)isr14, 0x08, 0x8E, &idt_entries[14]);
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
   idt_entry->flags = flags /* | 0x60 */;
}
