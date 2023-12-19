#ifndef PROCESS_H
#define PROCESS_H

#include "common.h"


typedef  struct process
{
registers_t regs;
u32int code;
u32int  id;
u32int parent_id;
u8int priority;

//vm
u32int user_page_directory[1024]__attribute__((aligned(4096))); 

} process_t;



#endif 