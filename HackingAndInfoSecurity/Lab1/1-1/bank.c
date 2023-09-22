#include <stdio.h>
#include <stdlib.h>

// This function will print out the content of "secret.txt" file. You don't need
// to know the internals of this function.
void print_secret(void);

void menu(void) {
  printf("[SYSTEM] What is your choice?\n");
  printf("1. Send money to Alice\n");
  printf("2. Read secret file\n");
  printf("3. Quit\n");
  printf("(Enter 1~3): ");
}

int read_int(void) {
  int v;
  if (scanf("%d", &v) != 1) {
    printf("[ERROR] Invalid input format\n");
    exit(1);
  }
  return v;
}

int main(void) {
  int alice_balance = 2000;
  int your_balance = 1000;
  int choice, amount;
  int quit_flag = 0;

  setvbuf(stdin, NULL, _IONBF, 0);

  while (!quit_flag) {
    printf("=============================\n");
    printf("[SYSTEM] Your balance = %d\n", your_balance);
    menu();
    choice = read_int();
    switch (choice) {
      case 1:
        printf("Input the amount of money you want to send: ");
        amount = read_int();
        if (amount > your_balance) {
          printf("[ERROR] You don't have enough balance\n");
          break;
        }
        your_balance -= amount;
        alice_balance += amount;
        break;

      case 2:
        if (your_balance < 50000) {
          printf("[ERROR] Only the VIP user can read the secret file\n");
          printf("(VIP requirement: balance >= 50000)\n");
          break;
        }
        print_secret();
        break;

      case 3:
        quit_flag = 1;
        break;

      default:
        printf("[ERROR] Invalid choice\n");
    }
  }

  return 0;
}
