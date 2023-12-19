#ifndef PROCESS_H
#define PROCESS_H

#include "common.h"
typedef struct process
{
   u32int id;
   registers_t regs;
   u32int pd[1024];
} process_t;

#endif