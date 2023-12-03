bits 32 # 32 bits

global _start

_start:
       mov eax, 0xDEADBEEF
       push eax
       mov ebp, esp
       push 0x1
       push 0x2
       push 0x3
       
       
       int 128
       push 0x4
       nop
       nop
       int 128
       jmp $

