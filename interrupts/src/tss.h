#ifndef TSS_H
#define TSS_H
#include "common.h"


extern void tss_flush();

void tss_init(u32int idx, u32int kss, u32int kesp);

void tss_set_stack(u32int kss, u32int kesp);

#endif
