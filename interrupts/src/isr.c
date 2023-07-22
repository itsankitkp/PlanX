#include "common.h"
#include "isr.h"
#include "io.h"
#include "stdio.h"

// This gets called from our ASM interrupt handler stub.
void isr_handler(registers_t regs)
{
    char buffer[3];
    char result[32];
    itoa(buffer, regs.int_no, 10);

    concatenate_strings("Got interrupted by ", buffer, result);

    concatenate_strings(result, "\0", result);

    fb_write(result, RED);
}