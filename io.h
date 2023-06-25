#ifndef INCLUDE_IO_H
#define INCLUDE_IO_H

void outb(unsigned short port, unsigned char data);
void fb_move_cursor(unsigned short pos);
void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg);
void fb_write(char* text, int color);
#endif /* INCLUDE_IO_H */