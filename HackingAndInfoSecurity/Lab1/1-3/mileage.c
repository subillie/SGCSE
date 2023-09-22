#include <stdio.h>
#include <stdlib.h>

#define MAX_MILEAGE 5000

// This function will print out the content of "secret.txt" file. You don't need
// to know the internals of this function.
void print_secret(void);

void menu(void) {
  printf("[SYSTEM] What is your choice?\n");
  printf("1. Use airplane\n");
  printf("2. Transfer mileage\n");
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
  int alice_mileage = 1000;
  int bob_mileage = 2000;
  int your_mileage = 3000;
  int choice, amount1, amount2;
  int quit_flag = 0;

  setvbuf(stdin, NULL, _IONBF, 0);

  while (!quit_flag) {
    printf("=============================\n");
    printf("[SYSTEM] Your mileage = %d\n", your_mileage);
    if (your_mileage > 100000) {
        print_secret();
        exit(0);
    }
    menu();
    choice = read_int();
    switch (choice) {
      case 1:
        if (your_mileage >= MAX_MILEAGE) {
          printf("Cannot accumulate mileage anymore\n");
          break;
        }
        your_mileage += 100;
        printf("Accumulated 100 point mileage, thank you!\n");
        break;

      case 2:
        printf("How much mileage do you want to transfer to Alice?\n");
        amount1 = read_int();
        printf("How much mileage do you want to transfer to Bob?\n");
        amount2 = read_int();
        if (amount1 < 0 || amount2 < 0) {
          printf("[ERROR] You cannot input negative number\n");
          break;
        }
        if (amount1 + amount2 > your_mileage) {
          printf("[ERROR] You don't have enough mileage to transfer\n");
          break;
        }
        your_mileage -= (amount1 + amount2);
        alice_mileage += amount1;
        bob_mileage += amount2;
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
