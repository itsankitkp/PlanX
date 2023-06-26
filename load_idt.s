global load_idt

load_idt:
    mov eax, [esp+4]

    lidt eax ; load the IDT
    ret ; return to the calling function