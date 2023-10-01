bits 32

MAGIC_NUMBER equ 0x1BADB002
ALIGN_MODULES equ 0x00000001
KERNEL_VIRTUAL_BASE equ 0xC0000000
KERNEL_PAGE_NUMBER equ (KERNEL_VIRTUAL_BASE >> 22) ; Index in the page directory

; calculate the checksum (all options + checksum should equal 0)
CHECKSUM equ -(MAGIC_NUMBER + ALIGN_MODULES) 
section .multiboot.data               ;according to multiboot spec
        dd MAGIC_NUMBER           ;set magic number for
                                 ;bootloader
        dd ALIGN_MODULES         ;set flags
        dd CHECKSUM  ;set checksum

page_table1:
    dd 0x00000003  ; Identity mapping for the first 4 MB

page_table768:
    dd 0x00000003  ; Mapping 768th page table  0xC0000000 to first 4 MB    

page_directory:
    ; the first entry identity maps the first 4MB of memory
    ; All bits are clear except the following:
    ; bit 7: PS The kernel page is 4MB.
    ; bit 1: RW The kernel page is read/write.
    ; bit 0: P  The kernel page is present.
    dd 0x00000083; first table
    times (768-1) dd 0; rest of the tables are empty
    dd 0x00000083 ; 768th table for 0xC0000000
    times 1024 - 768 - 1 dd 0;  rest of entries unmapped again

; GDT (Global Descriptor Table) Setup
gdt_start:
    dq 0x0  ; Null descriptor
    dq 0x0000000000000000  ; First descriptor (null)
    
    ; Code segment descriptor for accessing the first 1 MB (Limit = 1 MB)
    dq 0x00CF920000000000  ; Base = 0x0, Limit = 1 MB, Code segment
    
    ; Data segment descriptor for accessing the first 1 MB (Limit = 1 MB)
    dq 0x00CF960000000000  ; Base = 0x0, Limit = 1 MB, Data segment

    ; Code segment descriptor for the higher-half kernel (Base = 0xC0000000, Limit = 4 GB)
    dq 0x0020980000000000  ; Code segment descriptor (Base = 0xC0000000, Limit = 4 GB)

    ; Data segment descriptor for the higher-half kernel (Base = 0xC0000000, Limit = 4 GB)
    dq 0x0000920000000000  ; Data segment descriptor (Base = 0xC0000000, Limit = 4 GB)

gdt_end:
gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start


global start
extern main                      ;defined in the C file

section .multiboot.text
start:

        xor eax, eax
        lgdt  [gdt_descriptor]
        
        mov eax, cr0 ; Enable paging
        or eax, 0x80000001
        mov cr0, eax

        mov ecx, page_directory
        mov cr3, ecx ;
        mov ebx, cr4 ; read current cr4
        or ebx, 0x00000010 ; set PSE
        mov cr4, ebx; update cr4
        mov ebx, cr0 ; read current cr0
        or ebx, 0x80000000 ; set PG
        mov cr0, ebx; update cr0
        cli                      ;block interrupts
        lea ebx, [higher_half] ; load the address of the label in ebx
        jmp ebx ; jump to the label

section .text
higher_half:
        mov dword [page_directory], 0
        invlpg [0]
        mov esp, stack_space     ;set stack pointer
        mov ebp, stack_space     ;set stack pointer
        push ebx
        call main
        hlt                      ;halt the CPU

section .bss
resb 8192                        ;8KB for stack
stack_space:

