#include "io.h"

/** fb_move_cursor:
 * Moves the cursor of the framebuffer to the given position
 *
 * @param pos The new position of the cursor
 */
void fb_move_cursor(unsigned short pos)
{
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT,
         ((pos >> 8) & 0x00FF));
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outb(FB_DATA_PORT, pos & 0x00FF);
}

void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg)
{
    char *fb = (char *)0xC00B8000;
    fb[i] = c;
    fb[i + 1] = ((fg & 0x0F) << 4) | (bg & 0x0F);
}
int write_index = 0;
void fb_write(char *text, int color)
{

    while (*text != '\0')
    {
        if (*text == '\n')
        {
            int new_index = (write_index + 55) * 2;
            while (write_index < new_index)
            {
                fb_write_cell(write_index, ' ', BLACK, WHITE);
                write_index += 2;
                continue;
            }
        }
        fb_write_cell(write_index, *text, 0, color);
        text++;
        write_index = write_index + 2;
    }
    fb_move_cursor(write_index / 2);
}

void fb_clear_screen()
{
    int index = 0;
    /* there are 25 lines each of 80 columns;
       each element takes 2 bytes */
    while (index < 80 * 25 * 2)
    {
        fb_write_cell(index, ' ', BLACK, WHITE);
        index += 2;
    }
}

// void fb_print_address(char* some)
// {
//     char address[20];  // Assuming a fixed size for the character array to store the address

//     unsigned long long int addressValue = (unsigned long long int)some;

//     char hexDigits[] = "0123456789ABCDEF";
//     int i = 0;

//     while (addressValue > 0) {
//         address[i++] = hexDigits[addressValue % 16];
//         addressValue /= 16;
//     }

//     // Reverse the string
//     int start = 0;
//     int end = i - 1;
//     while (start < end) {
//         char temp = address[start];
//         address[start] = address[end];
//         address[end] = temp;
//         start++;
//         end--;
//     }

//     // Null-terminate the string
//     address[i] = '\n';

//     fb_write(address, WHITE);
// }