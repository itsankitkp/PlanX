#include "io.h"
#include "keyboard.h"
#define FB_GREEN 2
#define FB_DARK_GREY 8

#define VGA_ADDRESS 0xB8000   /* video memory begins here. */

/* VGA provides support for 16 colors */
#define BLACK 0
#define GREEN 2
#define RED 4
#define YELLOW 14
#define WHITE_COLOR 15

unsigned short *terminal_buffer;
unsigned int vga_index;

void clear_screen(void)
{
    int index = 0;
    /* there are 25 lines each of 80 columns;
       each element takes 2 bytes */
    while (index < 80 * 25 * 2) {
            terminal_buffer[index] = ' ';
            index += 2;
    }
}

void print_string(char *str, unsigned char color)
{
    int index = 0;
    while (str[index]) {
            terminal_buffer[vga_index] = (unsigned
             short)str[index]|(unsigned short)color << 8;
            index++;
            vga_index++;
    }
}

int main()
{
fb_clear_screen();

fb_write("this is test\n", WHITE);
char x = 'A';
while (x!='W'){
x = get_ascii();
fb_write_cell(10, x, 15,0);
if (x=='W')
fb_write("Found W\n",WHITE);
}

fb_write_cell(10, x, 15,0);
    return 0;
}