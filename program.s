bits 32 # 32 bits

global _start

_start:
       mov eax, 0xDEADBEEF
       hlt; hlt                      ;halt the CPU

