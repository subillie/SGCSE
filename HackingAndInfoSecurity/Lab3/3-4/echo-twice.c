#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void safe_echo(void) {
  char buf[36];
  volatile int i;
  volatile size_t n;
  puts("First, I will echo your input safely");
  fgets(buf, sizeof(buf), stdin);
  n = strlen(buf);
  // Filter some characters.
  for (i = 0; i < n; i++) {
    if (('0' <= buf[i] && buf[i] <= '9') || buf[i] == 'n') {
      printf("Digit or 'n' is not allowed!\n");
      exit(1);
    }
  }
  printf(buf);
}

void unsafe_echo(void) {
  char buf[32];
  ssize_t n;
  puts("Now I will echo your input unsafely, but you can't easily run /bin/sh");
  n = read(0, buf, 64);
  write(1, buf, n);
}

int main(void) {
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  safe_echo();
  unsafe_echo();
  return 0;
}
