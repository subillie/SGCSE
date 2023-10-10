#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PASSCODE_LEN 16

// This function will print out the content of "secret.txt" file. You don't need
// to know the internals of this function.
void print_secret(void);

void load_passcode(char *buf, int bufsize) {
  FILE *fp = fopen("/dev/urandom", "r");
  if (!fp) {
    printf("Failed to open /dev/urandom\n");
    exit(1);
  }
  fread(buf, bufsize, sizeof(char), fp);
  fclose(fp);
}

int main(void) {
  char input[20];
  char passcode[PASSCODE_LEN + 4];
  int trial;

  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);

  load_passcode(passcode, PASSCODE_LEN);
  passcode[PASSCODE_LEN] = '\0';

  for (trial = 1; trial <= 3; trial++) {
    printf("(Trial %d) What is the passcode? : ", trial);
    scanf("%s", input);
    if (strlen(input) < 8) {
      printf("Invalid passcode length\n");
      continue;
    }
    if (strcmp(input, passcode) == 0) {
      print_secret();
    } else {
      printf("Wrong passcode!\n");
    }
  }

  return 0;
}
