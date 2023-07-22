
#include "common.h"
#include "descriptor_tables.h"

gdt_entry_t gdt_entries[GDTSIZE];
gdt_ptr_t gdt_ptr;
idt_entry_t idt_entries[256];
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
   return;
   idt_ptr.limit = sizeof(idt_entry_t) * 256 - 1;
   idt_ptr.base = (u32int)&idt_entries;

   // memset(&idt_entries, 0, sizeof(idt_entry_t)*256);

   idt_set_gate(0, (u32int)isr0, 0x08, 0x8E);
   idt_set_gate(1, (u32int)isr1, 0x08, 0x8E);
   idt_set_gate(2, (u32int)isr2, 0x08, 0x8E);
   idt_set_gate(3, (u32int)isr3, 0x08, 0x8E);
   idt_set_gate(31, (u32int)isr31, 0x08, 0x8E);

   idt_flush((u32int)&idt_ptr);
}

void idt_set_gate(u8int num, u32int base, u16int sel, u8int flags)
{
   idt_entries[num].base_lo = base & 0xFFFF;
   idt_entries[num].base_hi = (base >> 16) & 0xFFFF;

   idt_entries[num].sel = sel;
   idt_entries[num].always0 = 0;
   // We must uncomment the OR below when we get to using user-mode.
   // It sets the interrupt gate's privilege level to 3.
   idt_entries[num].flags = flags /* | 0x60 */;
}