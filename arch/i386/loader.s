bits 32

MAGIC_NUMBER equ 0x1BADB002
ALIGN_MODULES equ 0x00000001

; calculate the checksum (all options + checksum should equal 0)
CHECKSUM equ -(MAGIC_NUMBER + ALIGN_MODULES) 
section .multiboot.data               ;according to multiboot spec
        dd MAGIC_NUMBER           ;set magic number for
                                 ;bootloader
        dd ALIGN_MODULES         ;set flags
        dd CHECKSUM  ;set checksum


global start
extern main                      ;defined in the C file

section .multiboot.text
start:
        cli                      ;block interrupts
        lea ebx, [higher_half] ; load the address of the label in ebx
        jmp ebx ; jump to the label

section .text
higher_half:
        mov esp, stack_space     ;set stack pointer
        mov ebp, stack_space     ;set stack pointer
        push ebx
        call main
        hlt                      ;halt the CPU

section .bss
resb 8192                        ;8KB for stack
stack_space: