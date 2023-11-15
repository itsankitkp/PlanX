#ifndef PAGING_H
#define PAGING_H

#include "common.h"
#include "isr.h"

extern void loadPageDirectory(unsigned int);
extern void enablePaging();
void init_paging();
void enable_page(u32int addr, u32int count);
extern void enter_user_mode(u32int addr);
#endif