
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
   gdt_ptr.base = GDTBASE;

   gdt_set_gate(0, 0, 0, 0, &gdt_entries[0]);                // Null segment
   gdt_set_gate(0, 0xFFFFFFFF, 0x9A, 0xCF, &gdt_entries[1]); // Code segment
   gdt_set_gate(0, 0xFFFFFFFF, 0x92, 0xCF, &gdt_entries[2]); // Data segment
   gdt_set_gate(0, 0xFFFFFFFF, 0xFA, 0xCF, &gdt_entries[3]); // User mode code segment
   gdt_set_gate(0, 0xFFFFFFFF, 0xF2, 0xCF, &gdt_entries[4]); // User mode data segment

   memcpy((char *)gdt_ptr.base, (char *)gdt_entries, gdt_ptr.limit);

   gdt_flush((u32int)&gdt_ptr);
}

// Set the value of one GDT entry.
void gdt_set_gate(u32int base, u32int limit, u8int access, u8int gran, gdt_entry_t *gdt_entry)
{
   gdt_entry->base_low = (base & 0xFFFF);
   gdt_entry->base_middle = (base >> 16) & 0xFF;
   gdt_entry->base_high = (base >> 24) & 0xFF;

   gdt_entry->limit_low = (limit & 0xFFFF);
   gdt_entry->granularity = (limit >> 16) & 0x0F;

   gdt_entry->granularity |= gran & 0xF0;
   gdt_entry->access = access;
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