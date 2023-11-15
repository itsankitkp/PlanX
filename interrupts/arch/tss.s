global tss_flush
tss_flush:
    mov ax, (5 * 8) | 0 
    ltr ax
    ret
