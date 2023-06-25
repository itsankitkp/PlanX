global loader
MAGIC_NUMBER equ 0x1BADB002
FLAGS equ 0x0
CHECKSUM equ -MAGIC_NUMBER; define the magic number constant

section .text:
align 4
    dd MAGIC_NUMBER
    dd FLAGS; start of the text (code) section
    dd CHECKSUM

loader:
    mov eax, 0xCAFEBABE

.loop:
    jmp .loop

KERNEL_STACK_SIZE equ 4096
section .bss
align 4
kernel_stack:
    resb KERNEL_STACK_SIZE

mov esp, kernel_stack + KERNEL_STACK_SIZE