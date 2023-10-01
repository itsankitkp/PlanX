OUTPUT_FORMAT(elf32-i386)
ENTRY(start)
SECTIONS
{
  . = 1M;
	_kernel_start = .;

  .multiboot.data  ALIGN (0x1000): {
    *(.multiboot.data)
  }

  .multiboot.text ALIGN (0x1000) : {
    *(.multiboot.text)
  }

	. += 0xC0000000;

   /* align at 4 KB and load at 1 MB */
  .text ALIGN (0x1000) : AT(ADDR(.text)-0xC0000000)
  {
    *(.text) /* all text sections from all files */
  }
  .rodata ALIGN (4K) : AT (ADDR (.rodata) - 0xC0000000)
	{
		*(.rodata)
	}

  /* align at 4 KB and load at 1 MB + . */
  .data ALIGN (0x1000) : AT(ADDR(.text)-0xC0000000)
  {
    *(.data) /* all data sections from all files */
  }

	.bss ALIGN (4K) : AT (ADDR (.bss) - 0xC0000000)
	{
		*(COMMON)
		*(.bss)
		*(.stack_space)
	}
	/* Add a symbol that indicates the end address of the kernel. */
	_kernel_end = .;
}