#ifndef COMMON_H
#define COMMON_H

// Some nice typedefs, to standardise sizes across platforms.
// These typedefs are written for 32-bit X86.
typedef unsigned int   u32int;
typedef          int   s32int;
typedef unsigned short u16int;
typedef          short s16int;
typedef unsigned char  u8int;
typedef          char  s8int;
#define NULL ((void*)0)

typedef void (*call_module_t)(void);
void* memcpy(char* dst, char* src, int len);
void *memset(char *dst,char src, int n);

#endif