#include <stdio.h>
#include <unistd.h>

char global_buf[32];

/* Your goal is to execute this function with "/bin/sh" as argument. */
void execv_wrapper(char *progpath) {
  execv(progpath, NULL);
}

void safe(void) {
  printf("Input your message in global buffer: ");
  read(0, global_buf, sizeof(global_buf));
}

void vuln(void) {
  char buf[20];
  printf("Input your message in stack buffer: ");
  read(0, buf, 64);
}

int main(void) {
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  safe();
  vuln();
  return 0;
}
