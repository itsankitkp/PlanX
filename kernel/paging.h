#ifndef PAGING_H
#define PAGING_H

#include "common.h"
#include "isr.h"

extern void loadPageDirectory(unsigned int);
extern void enablePaging();
void init_paging();
#endif