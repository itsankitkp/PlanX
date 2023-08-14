OBJECTS = loader.o kmain.o io.o inb.o outb.o keyboard.o interrupts.o interrupts.0 descriptor_tables.o
CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
-nostartfiles -nodefaultlibs -Wall -Wextra -Werror -Wno-error=main -ggdb -c
LDFLAGS = -T link.ld -melf_i386
AS = nasm
ASFLAGS = -f elf

# Directories
DRIVERS_DIR := drivers
INTERRUPTS_DIR := interrupts
KERNEL_DIR := kernel
ARCH_DIR := arch
LIB_DIR := lib

# Source files
C_SRCS := $(shell find $(DRIVERS_DIR) -name "*.c") $(shell find $(INTERRUPTS_DIR) -name "*.c") $(shell find $(KERNEL_DIR) -name "*.c") $(shell find $(LIB_DIR) -name "*.c")
ASM_SRCS := $(shell find $(DRIVERS_DIR) -name "*.s") $(shell find $(ARCH_DIR) -name "*.s") $(shell find $(INTERRUPTS_DIR) -name "*.s")

# Object files
OBJS := $(C_SRCS:%.c=%.o) $(ASM_SRCS:%.s=%.o)

# Include directories
INCLUDE_DIRS := $(shell find $(DRIVERS_DIR) -type d) $(shell find $(INTERRUPTS_DIR) -type d) $(shell find $(ARCH_DIR) -type d) $(shell find $(KERNEL_DIR) -type d) $(shell find $(LIB_DIR) -type d)

# Include flags
INCLUDE_FLAGS := $(addprefix -I, $(INCLUDE_DIRS))



all: kernel.elf
kernel.elf: $(OBJS)
	ld $(LDFLAGS) $(OBJS) -o kernel.elf

os.iso: kernel.elf
	mkdir -p iso
	mkdir -p iso/boot
	mkdir -p iso/boot/grub
	mkdir -p iso/boot/modules
	cp kernel.elf iso/boot/kernel.elf
	cp grub.cfg iso/boot/grub/grub.cfg
	cp menu.lst iso/boot/grub/menu.lst
	nasm -f elf32   program.s -o program.o
	ld -m elf_i386 program.o -o program
	cp program iso/boot/modules/program
	grub-mkrescue -o os.iso iso/

run: os.iso
	qemu-system-x86_64 -boot d -cdrom os.iso -m 512 -D ./log.txt -monitor stdio
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE_FLAGS)  $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) && 	rm program && rm kernel.elf