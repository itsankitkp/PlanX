#include "common.h"
#include "paging.h"

#ifndef KHEAP_H
#define KHEAP_H

extern u32int placement_address;

// Macros used in the bitset algorithms.
#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

u32int kmalloc(u32int sz);                // page aligned.
u32int kmalloc_a(u32int sz, int align);
u32int kmalloc_ap(u32int sz, int align, u32int *phys);
u32int kmalloc(u32int sz);                  // vanilla (normal).

#endif