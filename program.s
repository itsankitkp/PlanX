bits 32 # 32 bits

global _start

_start:
       mov eax, 0xDEADBEEF
       
       
       int 0x80
       jmp $

