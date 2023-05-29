#include <stdio.h>

/* This function will print a secret string to you. Your goal is to execute this
 * function by exploiting buffer overflow vulnerability.
 */
void print_secret(void);

void echo(void) {
    char buf[24];
    puts("Input your message:");
    gets(buf);
    puts(buf);
}

int main(void) {
    echo();
    return 0;
}
