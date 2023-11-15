[GLOBAL loadPageDirectory]

loadPageDirectory:
    push ebp
    mov ebp, esp
    mov eax, [ebp + 8]
    mov cr3, eax
    mov esp, ebp
    pop ebp
    ret

[GLOBAL enablePaging]
enablePaging:
    push ebp
    mov ebp, esp
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
    mov esp, ebp
    pop ebp
    ret

USER_MODE_CODE_SEGMENT_SELECTOR equ 4*8
USER_MODE_DATA_SEGMENT_SELECTOR equ 5*8
[GLOBAL enter_user_mode]
enter_user_mode:
    push ebp
    mov ebp, esp
    mov ecx, [ebp + 8]
    mov bx, USER_MODE_CODE_SEGMENT_SELECTOR| 0x3
    mov ds, bx
    mov es, bx 
	mov fs, bx 
	mov gs, bx ; SS is handled by iret
    
	; set up the stack frame iret expects
	mov eax, esp
	push USER_MODE_DATA_SEGMENT_SELECTOR | 3 ; data selector
	push eax ; current esp
	pushf ; eflags
	push USER_MODE_CODE_SEGMENT_SELECTOR | 3 ; code selector (ring 3 code with bottom 2 bits set for ring 3)
	push ecx ; instruction address to return to
	iret