#include "io.h"
#define KBD_DATA_PORT 0x60
/** read_scan_code:
 * Reads a scan code from the keyboard
 *
 * @return The scan code (NOT an ASCII character!)
 */
unsigned char read_scan_code(void)
{
    return inb(KBD_DATA_PORT);
}

char get_ascii(void)
{
    unsigned char scan_code = read_scan_code();

    if (scan_code == 0x10)
        return 'Q';
    if (scan_code == 0x11)
        return 'W';
    if (scan_code == 0x12)
        return 'E';
    if (scan_code == 0x13)
        return 'R';
    if (scan_code == 0x14)
        return 'T';
    if (scan_code == 0x15)
        return 'Y';
    if (scan_code == 0x16)
        return 'U';
    if (scan_code == 0x17)
        return 'I';
    return 'X';
}