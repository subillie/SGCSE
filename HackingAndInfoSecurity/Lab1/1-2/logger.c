#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LOGFORMAT "%s/%s.txt"
#define LOGDIR "logs"

void read_line(char *buf, int bufsize) {
  if (!fgets(buf, bufsize, stdin)) {
    exit(1);
  }
  buf[strcspn(buf, "\n")] = '\0'; // Replace the trailing newline character.
}

void write_log(char *file_path) {
  time_t now = time(NULL);
  FILE * fp = fopen(file_path, "w");
  if (!fp) {
    printf("Failed to open %s\n", file_path);
    exit(1);
  }
  fprintf(fp, "Last login: %s", ctime(&now));
  fclose(fp);
  printf("Recorded your last login time!\n");
}

void read_log(char *file_path) {
  char logbuf[64];
  FILE * fp = fopen(file_path, "r");
  if (!fp) {
    printf("Failed to open %s\n", file_path);
    exit(1);
  }
  fgets(logbuf, sizeof(logbuf), fp);
  fputs(logbuf, stdout);
  fclose(fp);
}

int main(void) {
  char file_path[64];
  char input_buf[32];

  setvbuf(stdin, NULL, _IONBF, 0);

  printf("Welcome! Please input your ID: ");
  read_line(input_buf, sizeof(input_buf));
  snprintf(file_path, sizeof(file_path), LOGFORMAT, LOGDIR, input_buf);
  write_log(file_path);

  printf("Do you want to check other user's login record? (Y/N): ");
  read_line(input_buf, sizeof(input_buf));
  if (strcmp("Y", input_buf) == 0) {
    printf("Input the user ID you want to check: ");
    read_line(input_buf, sizeof(input_buf));
    snprintf(file_path, sizeof(file_path), LOGFORMAT, LOGDIR, input_buf);
    read_log(file_path);
  }

  return 0;
}
