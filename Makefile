OBJECTS = loader.o kmain.o io.o ios.o
CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
-nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c
LDFLAGS = -T link.ld -melf_i386
AS = nasm
ASFLAGS = -f elf
all: kernel.elf
kernel.elf: $(OBJECTS)
	ld $(LDFLAGS) $(OBJECTS) -o kernel.elf

os.iso: kernel.elf
	mkdir -p iso
	mkdir -p iso/boot
	mkdir -p iso/boot/grub
	cp kernel.elf iso/boot/kernel.elf
	cp grub.cfg iso/boot/grub/grub.cfg
	grub-mkrescue -o os.iso iso/

run: os.iso
	qemu-system-x86_64 -boot d -cdrom os.iso -m 512 -D ./log.txt -monitor stdio
%.o: %.c
	$(CC) $(CFLAGS) $< -o $@
%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf *.o $(OBJECTS)