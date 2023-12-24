#include "common.h"
#include "isr.h"
#include "io.h"
#include "stdio.h"
#include "paging.h"

// This gets called from our ASM interrupt handler stub.
void isr_handler(registers_t regs)
{
   char buffer[3];
   char result[32];
   itoa(buffer, regs.int_no, 10);

   concatenate_strings("Got interrupted by ", buffer, result);
   fb_write(result, RED);
   concatenate_strings(result, "\0", result);
   if (regs.int_no == 0xffffff80)
   {
      //char a[32] = 'auf';
      fb_write("AUF", YELLOW);
      return;
   }

   u32int faulting_address = 0;
   asm volatile("mov %%cr2, %0" : "=r"(faulting_address));
   if (faulting_address != 0)
   {
      page_fault(regs);
   }
}

void page_fault(registers_t regs)
{
   // A page fault has occurred.
   // The faulting address is stored in the CR2 register.
   u32int faulting_address;
   asm volatile("mov %%cr2, %0" : "=r"(faulting_address));

   // The error code gives us details of what happened.
   int present = !(regs.err_code & 0x1); // Page not present
   int rw = regs.err_code & 0x2;         // Write operation?
   int us = regs.err_code & 0x4;         // Processor was in user-mode?
   int reserved = regs.err_code & 0x8;   // Overwritten CPU-reserved bits of page entry?
   // int id = regs.err_code & 0x10;          // Caused by an instruction fetch?

   // Output an error message.
   fb_write("Page fault! ( \0", WHITE);
   if (present)
   {
      fb_write("present \0", WHITE);
   }
   if (rw)
   {
      fb_write("read-only \0", WHITE);
   }
   if (us)
   {
      fb_write("user-mode \0", WHITE);
   }
   if (reserved)
   {
      fb_write("reserved \0", WHITE);
   }
   fb_write(") at 0x\0", WHITE);
   char result[32];
   hextoa(faulting_address, result);
   concatenate_strings(result, "\0", result);

   fb_write(result, RED);
   fb_write("\n\0", WHITE);
}