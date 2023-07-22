#include "common.h"

void *memcpy(char *dst, char *src, int len)
{
    char *p = dst;
    while (len--)
        *dst++ = *src++;

    return p;
}