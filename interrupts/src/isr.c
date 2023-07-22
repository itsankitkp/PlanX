#include "common.h"
#include "isr.h"
#include "io.h"

// This gets called from our ASM interrupt handler stub.
void isr_handler(registers_t regs)
{

fb_write("Got interuppted"+(char)regs.int_no+'\n', WHITE);
} 