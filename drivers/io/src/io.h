#ifndef INCLUDE_IO_H
#define INCLUDE_IO_H

/* The I/O ports */
#define FB_COMMAND_PORT 0x3D4
#define FB_DATA_PORT 0x3D5
/* The I/O port commands */
#define FB_HIGH_BYTE_COMMAND 14
#define FB_LOW_BYTE_COMMAND 15

/* VGA provides support for 16 colors */
#define BLACK 0
#define GREEN 2
#define RED 4
#define YELLOW 14
#define WHITE 15

void outb(unsigned short port, unsigned char data);

/** inb:
* Read a byte from an I/O port.
*
* @param port The address of the I/O port
* @return
The read byte
*/
unsigned char inb(unsigned short port);

void fb_move_cursor(unsigned short pos);
void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg);
void fb_write(char* text, int color);
void fb_clear_screen();
void fb_print_address(char* some);
#endif /* INCLUDE_IO_H */