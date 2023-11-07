#include <stdio.h>
#include <string.h>
#include <unistd.h>

char gbuf[] = "Sometimes simpler program is harder to exploit ^_^\n";
char *msg = gbuf;

void f(void) {
  char buf[16];
  write(1, msg, strlen(gbuf));
  read(0, buf, 144);
  msg = NULL; // To make it a little bit harder.
}

int main(void) {
  f();
  return 0;
}
