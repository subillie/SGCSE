#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_ITEMS 16
#define MAX_NAME_LEN 32
#define INIT_DESCR_LEN 32
#define MAX_DESCR_LEN 72

typedef struct item {
  char name[MAX_NAME_LEN];
  char *description;
  int descr_len;
  int price;
  void (*print_info)(struct item *i);
} item;

typedef struct tv {
  char name[MAX_NAME_LEN];
  char *description;
  int descr_len;
  int price;
  void (*print_info)(struct item *i);
  char typ[8];
  size_t size;
  int warranty;
} tv;

typedef struct pc {
  char name[MAX_NAME_LEN];
  char *description;
  int descr_len;
  int price;
  void (*print_info)(struct item *i);
  char cpu[8];
  int ram;
  int disk;
} pc;

item *items[MAX_ITEMS];

void* malloc_wrapper(size_t size) {
  void* p = malloc(size);
  if (p == NULL) {
    puts("Failed to allocate memory");
    exit(1);
  }
  return p;
}

ssize_t read_wrapper(int fd, void *buf, size_t count) {
  ssize_t n = read(fd, buf, count);
  if (n <= 0) {
    puts("read() failed");
    exit(1);
  }
  return n;
}

void write_string(char *s) {
  write(1, s, strlen(s));
}

long int read_int(void) {
  char buf[32];
  ssize_t n = read_wrapper(0, buf, 31);
  buf[n] = '\0';
  return strtol(buf, NULL, 10);
}

void print_tv_info(item *i) {
  tv *t = (tv*) i;
  printf("(Details of %s)\n", t->name);
  printf("Display type: %s\n", t->typ);
  printf("Size: %ld inch\n", t->size);
  printf("Warranty: %d years\n", t->warranty);
}

void print_pc_info(item *i) {
  pc *p = (pc*) i;
  printf("(Details of %s)\n", p->name);
  printf("CPU: %s\n", p->cpu);
  printf("RAM: %d GB\n", p->ram);
  printf("disk: %d GB\n", p->disk);
}

void list_items(void) {
  item *item;

  for (int i = 0; i < MAX_ITEMS; i++) {
    item = items[i];
    if (item != NULL) {
      printf("[*] %s (%s): $%d\n", item->name, item->description, item->price);
    }
  }
}

void view_item(void) {
  char name[MAX_NAME_LEN];
  item *item;

  write_string("Input the name of item to view: ");
  fgets(name, MAX_NAME_LEN, stdin);
  name[strcspn(name, "\n")] = '\0';

  for (int i = 0; i < MAX_ITEMS; i++) {
    item = items[i];
    if (item != NULL && strncmp(name, item->name, MAX_NAME_LEN - 1) == 0) {
      item->print_info(item);
      break;
    }
  }
}

void add_tv_item_at(int i) {
  tv *t = malloc_wrapper(sizeof(tv));
  t->description = malloc_wrapper(INIT_DESCR_LEN);
  t->descr_len = INIT_DESCR_LEN;
  t->print_info = print_tv_info;

  write_string("Input the name of the new item: ");
  fgets(t->name, MAX_NAME_LEN, stdin);
  t->name[strcspn(t->name, "\n")] = '\0';

  write_string("Input the description: ");
  fgets(t->description, INIT_DESCR_LEN, stdin);
  t->description[strcspn(t->description, "\n")] = '\0';

  write_string("Input the price: ");
  t->price = (int) read_int();

  write_string("Input the type of display (LCD, OLED, ...): ");
  fgets(t->typ, 8, stdin);
  t->typ[strcspn(t->typ, "\n")] = '\0';

  write_string("Input the size of display (in inch): ");
  t->size = read_int();

  write_string("Input the warranty of the item (in year): ");
  t->warranty = (int) read_int();

  items[i] = (item*) t;
}

void add_pc_item_at(int i) {
  pc *p = malloc_wrapper(sizeof(pc));
  p->description = malloc_wrapper(INIT_DESCR_LEN);
  p->descr_len = INIT_DESCR_LEN;
  p->print_info = print_pc_info;

  write_string("Input the name of the new item: ");
  fgets(p->name, MAX_NAME_LEN, stdin);
  p->name[strcspn(p->name, "\n")] = '\0';

  write_string("Input the description: ");
  fgets(p->description, INIT_DESCR_LEN, stdin);
  p->description[strcspn(p->description, "\n")] = '\0';

  write_string("Input the price: ");
  p->price = (int) read_int();

  write_string("Input the type of CPU (i5, i7, ...): ");
  fgets(p->cpu, 8, stdin);
  p->cpu[strcspn(p->cpu, "\n")] = '\0';

  write_string("Input the size of RAM (in GB): ");
  p->ram = (int) read_int();

  write_string("Input the size of hard disk (in GB): ");
  p->disk = (int) read_int();

  items[i] = (item*) p;
}

void add_item(void) {
  char buf[16];

  write_string("Input the type of item (tv/pc): ");
  fgets(buf, sizeof(buf), stdin);
  buf[strcspn(buf, "\n")] = '\0';

  for (int i = 0; i < MAX_ITEMS; i++) {
    if (items[i] == NULL) {
      if (strcmp(buf, "tv") == 0) {
        add_tv_item_at(i);
      } else if (strcmp(buf, "pc") == 0) {
        add_pc_item_at(i);
      } else {
        puts("Invalid type of item");
      }
      return;
    }
  }
}

void delete_item(void) {
  char name[MAX_NAME_LEN];
  item *item;

  write_string("Input the name of item to delete: ");
  fgets(name, MAX_NAME_LEN, stdin);
  name[strcspn(name, "\n")] = '\0';

  for (int i = 0; i < MAX_ITEMS; i++) {
    item = items[i];
    if (item != NULL && strncmp(name, item->name, MAX_NAME_LEN - 1) == 0) {
      free(item->description);
      free(item);
      items[i] = NULL;
      break;
    }
  }
}

void modify_description(void) {
  char name[MAX_NAME_LEN];
  char descr[MAX_DESCR_LEN + 8] = { '\0' };
  item *item;
  size_t new_len;
  void *p;

  write_string("Input the name of item to modify: ");
  fgets(name, MAX_NAME_LEN, stdin);
  name[strcspn(name, "\n")] = '\0';

  for (int i = 0; i < MAX_ITEMS; i++) {
    item = items[i];
    if (item != NULL && strncmp(name, item->name, MAX_NAME_LEN - 1) == 0) {
      write_string("Input the new description: ");
      read_wrapper(0, descr, MAX_DESCR_LEN + 1);
      descr[strcspn(descr, "\n")] = '\0';
      new_len = strlen(descr);
      if (new_len > item->descr_len) {
        p = realloc(item->description, new_len);
        if (new_len > MAX_DESCR_LEN) {
          puts("Error, invalid description length");
          return;
        }
        item->description = p;
        item->descr_len = new_len;
      }
      strcpy(item->description, descr);
      break;
    }
  }
}

void manage_shopping_mall(void) {
  int choice;
  char buf[300];
  puts("**************************************************");
  puts("*** Welcome to the admin page of shopping mall ***");
  puts("**************************************************");
  while (1) {
    puts("======== Menu ========");
    puts("(L) List items");
    puts("(V) View item details");
    puts("(A) Add item");
    puts("(D) Delete item");
    puts("(M) Modify description");
    puts("(E) Exit");
    write_string("> Input your choice: ");
    memset(buf, 0, sizeof(buf));
    fgets(buf, sizeof(buf), stdin);
    buf[strcspn(buf, "\n")] = '\0';
    if (strcmp(buf, "L") == 0) {
        list_items();
    } else if (strcmp(buf, "V") == 0) {
        view_item();
    } else if (strcmp(buf, "A") == 0) {
        add_item();
    } else if (strcmp(buf, "D") == 0) {
        delete_item();
    } else if (strcmp(buf, "M") == 0) {
        modify_description();
    } else if (strcmp(buf, "E") == 0) {
        break;
    } else {
        puts("Invalid choice");
    }
  }
}

int main(void) {
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  manage_shopping_mall();
  return 0;
}
