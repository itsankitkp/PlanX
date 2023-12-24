bits 32 # 32 bits

global _start

_start:
       mov eax, 0xCAFEBABE
       push eax
       mov ebp, esp 
             
       int 128
       nop
       nop
       jmp $

