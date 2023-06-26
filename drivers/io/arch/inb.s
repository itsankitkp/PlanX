global inb
inb:
    mov dx, [esp + 4]
    in al, dx
    ret
