#include "stdio.h"
#include "common.h"

// Helper function to reverse a string
void reverse(char str[], int length) {
    int start = 0;
    int end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}


// Convert integer to string
void itoa(char *buf, unsigned int n, int base)
    {
        unsigned int tmp;
        int i, j;

        tmp = n;
        i = 0;

        do {
            tmp = n % base;
            buf[i++] = (tmp < 10) ? (tmp + '0') : (tmp + 'a' - 10);
        } while (n /= base);
        buf[i--] = 0;

        for (j = 0; j < i; j++, i--) {
            tmp = buf[j];
            buf[j] = buf[i];
            buf[i] = tmp;
        }
    }

int string_length(const char* str) {
    int length = 0;
    while (str[length] != '\0') {
        length++;
    }
    return length;
}
// Function to concatenate two strings
void concatenate_strings(const char* str1, const char* str2, char* result) {
    if (str1 == NULL || str2 == NULL || result == NULL) {
        // Handle NULL strings
        return;
    }

    int i = 0;

    // Copy the contents of str1 to the result string
    while (str1[i] != '\0') {
        result[i] = str1[i];
        i++;
    }

    // Append the contents of str2 to the result string
    int j = 0;
    while (str2[j] != '\0') {
        result[i] = str2[j];
        i++;
        j++;
    }

    result[i] = '\0'; // Null-terminate the result string
}

// Convert hexadecimal number to string
char* hextoa(unsigned int num, char str[]) {
    int i = 0;

    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    while (num != 0) {
        int rem = num % 16;
        str[i++] = (rem < 10) ? (rem + '0') : (rem - 10 + 'A');
        num = num / 16;
    }

    str[i] = '\0';
    reverse(str, i);

    return str;
}