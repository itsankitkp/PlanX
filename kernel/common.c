#include "common.h"

void *memcpy(char *dst, char *src, int len)
{
    char *p = dst;
    while (len--)
        *dst++ = *src++;

    return p;
}

void *memset(char *dst,char src, int n)
{
	char *p = dst;
	while (n--)
		*dst++ = src;
	return p;
}