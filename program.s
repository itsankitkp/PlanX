bits 32 # 32 bits

global _start

_start:
       mov eax, 0xDEADBEEF
       jmp $;                     ;halt the CPU

