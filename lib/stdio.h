#ifndef STDIO_H
#define STDIO_H

void itoa(char *buf, unsigned int n, int base);

void reverse(char str[], int length);
int string_length(const char* str);
void concatenate_strings(const char* str1, const char* str2, char* result) ;
char* hextoa(unsigned int num, char str[]) ;
#endif