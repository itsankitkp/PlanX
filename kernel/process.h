#ifndef PROCESS_H
#define PROCESS_H

#include "common.h"
typedef struct process
{
   u32int id;
   registers_t regs;
   u32int pd[1024]__attribute__((aligned(4096)));
} process_t __attribute__((aligned(4096)));

#endif