#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ITEM_COUNT 16

// This function will print out the content of "secret.txt" file. You don't need
// to know the internals of this function.
void print_secret(void);

void menu(void) {
  printf("=== Manage your fund portfolio ===\n");
  printf("1. Check your portfolio\n");
  printf("2. Rebalance the portfolio\n");
  printf("3. Retire from the market\n");
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

void market_update(int items[]) {
  for (int i = 0; i < ITEM_COUNT; i++) {
    items[i] += ((rand() % 512) - 256);
  }
}

void print_portfolio(int items[]) {
  for (int i = 0; i < ITEM_COUNT; i++)
    printf("Investment on item %d: $ %d\n", i, items[i]);
}

void rebalance_portfolio(int items[]) {
  int src_idx, dst_idx, amount;
  printf("Input the ID of item you want to withdraw money from: ");
  src_idx = read_int();
  printf("Input the ID of item you want to invest more money: ");
  dst_idx = read_int();
  printf("Decide how much to move: ");
  amount = read_int();
  if (src_idx < 0 || dst_idx < 0 || amount < 0) {
    printf("Negative input not allowed for item number or amount of money\n");
    return;
  }
  items[src_idx] -= amount;
  items[dst_idx] += amount;
}

void manage_fund(void) {
  int items[ITEM_COUNT];
  int choice, i, quit_flag = 0;

  for (i = 0; i < ITEM_COUNT; i++) {
    items[i] = 100000;
  }

  while (!quit_flag) {
    market_update(items);
    menu();
    choice = read_int();
    switch (choice) {
      case 1:
        print_portfolio(items);
        break;

      case 2:
        rebalance_portfolio(items);
        break;

      case 3:
        quit_flag = 1;
        break;

      default:
        printf("[ERROR] Invalid choice\n");
    }
  }
}

int main(void) {
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  srand(time(NULL));
  manage_fund();
  return 0;
}
