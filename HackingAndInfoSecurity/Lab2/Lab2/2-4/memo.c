#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_MEMO_COUNT 10
#define MAX_MEMO_LEN 24
#define BUFSIZE 50

struct memo {
  unsigned short id;
  unsigned short modify_cnt;
  char buf[MAX_MEMO_LEN];
};

// This function will print out the content of "secret.txt" file. You don't need
// to know the internals of this function.
void print_secret(void);

void menu(void) {
  printf("<Memo System>\n");
  printf("W: Write memo\n");
  printf("R: Read memo\n");
  printf("M: Modify memo\n");
  printf("E. Exit\n");
  printf("(Enter W/R/M/E): ");
}

void write_memo(struct memo marr[], int cur_cnt) {
  char * buf = marr[cur_cnt].buf;
  marr[cur_cnt].id = cur_cnt;
  marr[cur_cnt].modify_cnt = 0;
  memset(buf, 0, MAX_MEMO_LEN);
  printf("Input the content of memo #%d: ", cur_cnt);
  read(0, buf, MAX_MEMO_LEN - 1);
  buf[strcspn(buf, "\n")] = '\0'; // Replace trailing newline.
}

void read_memo(struct memo marr[], int cur_cnt) {
  char buf[BUFSIZE];
  int i;

  memset(buf, 0, BUFSIZE);
  printf("Input the ID of memo to read: ");
  read(0, buf, BUFSIZE - 1);
  i = (int) strtol(buf, NULL, 10);
  if (i >= cur_cnt) {
    printf("[ERROR] Invalid ID\n");
    return;
  }
  printf("ID: %hu\n", marr[i].id);
  printf("Modification count: %hu\n", marr[i].modify_cnt);
  printf("Content: ");
  write(1, marr[i].buf, MAX_MEMO_LEN);
  printf("\n");
}

void modify_memo(struct memo marr[], int cur_cnt) {
  char buf[BUFSIZE];
  int i;

  memset(buf, 0, BUFSIZE);
  printf("Input the ID of memo to modify: ");
  read(0, buf, BUFSIZE - 1);
  i = (int) strtol(buf, NULL, 10);
  if (0 <= i && i < cur_cnt) {
    printf("Input the new content of memo #%d: ", i);
    read(0, marr[i].buf, BUFSIZE);
    marr[i].buf[strcspn(marr[i].buf, "\n")] = '\0'; // Replace trailing newline.
    marr[i].modify_cnt++;
  } else {
    printf("[ERROR] Invalid ID\n");
  }
}

void memo_system(void) {
  struct memo marr[MAX_MEMO_COUNT];
  long iter = 0;
  int quit_flag = 0, cur_cnt = 0;
  int choice, c;

  memset(marr, 0, sizeof(marr));

  while (!quit_flag && iter < 32) {
    menu();
    choice = getchar();
    if (choice == EOF) break;
    do { // Consume trailing characters and newline.
      c = getchar();
    } while (c != '\n');
    switch (choice) {
      case 'W':
        if (cur_cnt >= MAX_MEMO_COUNT) {
          printf("[ERROR] Cannot write more memo\n");
          break;
        }
        write_memo(marr, cur_cnt++);
        break;

      case 'R':
        read_memo(marr, cur_cnt);
        break;

      case 'M':
        modify_memo(marr, cur_cnt);
        break;

      case 'E':
        quit_flag = 1;
        break;

      default:
        printf("[ERROR] Invalid choice\n");
    }
    iter++;
  }
}

int main(void) {
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  memo_system();
  return 0;
}
