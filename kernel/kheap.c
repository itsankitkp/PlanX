#include "common.h"
#include "kheap.h"
u32int placement_address;
u32int kmalloc(u32int sz)
{
    u32int tmp = placement_address;
    placement_address += sz;
    return tmp;
}

u32int kmalloc_a(u32int sz, int align)
{
    if (align == 1 && (placement_address & 0xFFFFF000)) // If the address is not already page-aligned
    {
        // Align it.
        placement_address &= 0xFFFFF000;
        placement_address += 0x1000;
    }
    u32int tmp = placement_address;
    placement_address += sz;
    return tmp;
}

u32int kmalloc_ap(u32int sz, int align, u32int *phys)
{
    if (align == 1 && (placement_address & 0xFFFFF000)) // If the address is not already page-aligned
    {
        // Align it.
        placement_address &= 0xFFFFF000;
        placement_address += 0x1000;
    }
    if (phys)
    {
        *phys = placement_address;
    }
    u32int tmp = placement_address;
    placement_address += sz;
    return tmp;
}

u32int allocate_page_frame(u32int addr, u32int size)
{
    u32int no_of_page_frames = (size/4096) + 1;
    no_of_page_frames += 1;

    return addr;

}