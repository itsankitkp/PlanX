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

USER_MODE_CODE_SEGMENT_SELECTOR equ 0x18
USER_MODE_DATA_SEGMENT_SELECTOR equ 0x20
[GLOBAL enter_user_mode]
enter_user_mode:
    push ebp
    mov ebp, esp
    mov eax, [ebp + 8]
    
    mov [esp+16], dword USER_MODE_DATA_SEGMENT_SELECTOR | 0x3
    mov [esp +12], esp
    
    mov [esp+ 4], dword USER_MODE_CODE_SEGMENT_SELECTOR | 0x3
    mov [esp+ 0], eax
    pushfl
    pop eax
    mov [esp+ 8], eax

    pop ebp
    iret 