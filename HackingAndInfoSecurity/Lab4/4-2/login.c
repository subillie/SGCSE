#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

enum action { JOIN, LOGIN, EXIT, ADD_MAIL, WITHDRAW, LOGOUT };

typedef struct user_node {
  char id[16];
  char pw[16];
  char mail[32];
  struct user_node *prev;
  struct user_node *next;
} user_node;

user_node* head;

void* malloc_wrapper(size_t size);
ssize_t read_wrapper(int fd, void *buf, size_t count);
void init_admin(void);
enum action outer_menu(void);
enum action inner_menu(user_node *n);
void add_mail(user_node *n);
void withdraw(user_node *n);
void user_service(user_node **np);
void login(void);
void join(void);
void main_service(void);

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

void init_admin(void) {
  FILE *fp;

  head = (user_node*) malloc_wrapper(sizeof(user_node));
  memset(head, 0, sizeof(user_node));

  // Set ID.
  strcpy(head->id, "admin");

  // Set password.
  fp = fopen("/dev/urandom", "r");
  if (!fp) {
    puts("Failed to open /dev/urandom");
    exit(1);
  }
  fread(head->pw, 15, sizeof(char), fp);
  fclose(fp);

  // Fill the email field with secret string.
  fp = fopen("secret.txt", "r");
  if (!fp) {
    puts("Configuration error (secret.txt does not exist)");
    exit(1);
  }
  fgets(head->mail, 32, fp);
  fclose(fp);

  // Set the prev/next pointer.
  head-> prev = NULL;
  head-> next = NULL;
}

enum action outer_menu(void) {
  char buf[32];

  puts("==== Choose your action ====");
  puts("L. Login");
  puts("J. Join");
  puts("E. Exit");
  printf("(Enter J/L/E): ");

  read_wrapper(0, buf, sizeof(buf));
  buf[strcspn(buf, "\n")] = '\0';

  if (!strcmp(buf, "J")) return JOIN;
  if (!strcmp(buf, "L")) return LOGIN;
  if (!strcmp(buf, "E")) return EXIT;
  return -1;
}

enum action inner_menu(user_node *n) {
  char buf[32];

  printf("Welcome %s, what do you want?\n", n->id);
  puts("A. Add your email");
  puts("W. Withdraw");
  puts("L. Logout");
  printf("(Enter A/W/L): ");

  read_wrapper(0, buf, sizeof(buf));
  buf[strcspn(buf, "\n")] = '\0';

  if (!strcmp(buf, "A")) return ADD_MAIL;
  if (!strcmp(buf, "W")) return WITHDRAW;
  if (!strcmp(buf, "L")) return LOGOUT;
  return -1;
}

void add_mail(user_node *n) {
  char buf[32];
  char *p1, *p2;
  int has_at = 0;
  int has_dot = 0;

  printf("Your current email: %s, enter your new email: ", n->mail);
  memset(buf, 0, 32);
  read_wrapper(0, buf, 31);
  p1 = buf;
  p2 = n->mail;
  while (*p1) {
    if (*p1 == '@') {
      if (has_at) {
        puts("Multiple '@' character in email address");
        exit(1);
      }
      strcpy(p2, "_at_");
      p1++;
      p2 += 4;
      has_at = 1;
    } else if (*p1 == '.') {
      if (has_dot) {
        puts("Multiple '.' character in email address");
        exit(1);
      }
      strcpy(p2, "_dot_");
      p1++;
      p2 += 5;
      has_dot = 1;
    } else if (*p1 == '\n') {
      *p2 = '\0';
      break;
    } else {
      *p2++ = *p1++;
    }
  }
}

void withdraw(user_node *n) {
  user_node *prev = n->prev;
  user_node *next = n->next;

  if (prev) {
    if (prev->next != n) {
      printf("Previous node status is inconsistent!\n");
      exit(1);
    }
    prev->next = next;
  } else {
    if (head != n) {
      printf("Head pointer status is inconsistent!\n");
      exit(1);
    }
    head = next;
  }

  if (next) {
    if (next->prev != n) {
      printf("Next node status is inconsistent!\n");
      exit(1);
    }
    next->prev = prev;
  }

  n->prev = NULL;
  n->next = NULL;
  // No free(), since user may stay for a while after withdrawal.
}

void user_service(user_node **np) {
  enum action choice;

  while (1) {
    choice = inner_menu(*np);
    if (choice == ADD_MAIL)
      add_mail(*np);
    else if (choice == WITHDRAW) {
      withdraw(*np);
    }
    else if (choice == LOGOUT)
      return;
    else
      puts("[ERROR] Invalid choice\n");
  }
}

void login(void) {
  char buf[48];
  user_node *node;

  printf("Input your ID: ");
  read_wrapper(0, buf, 32);
  buf[strcspn(buf, "\n")] = '\0';

  node = head;
  while (node) {
    if (strcmp(node->id, buf) == 0) {
      printf("Input your password: ");
      read_wrapper(0, buf, 32);
      buf[strcspn(buf, "\n")] = '\0';
      if (strcmp(node->pw, buf) == 0) {
        user_service(&node);
      } else {
        puts("Incorrect password!");
      }
      return;
    }
    node = node->next;
  }

  printf("ID %s not found\n", buf);
  return;
}

void join(void) {
  char id_buf[16];
  char pw_buf[16];
  user_node *new_user;
  volatile char *chk = id_buf;

  memset(id_buf, 0, 16);
  memset(id_buf, 0, 16);
  printf("Choose your ID: ");
  read_wrapper(0, id_buf, 15);
  id_buf[strcspn(id_buf, "\n")] = '\0';

  while (chk) {
    if (*chk < 'a' || *chk > 'z') {
      puts("Only lowercase alphabet allowed");
      return;
    }
    if (*(++chk) == 0)
      chk = NULL;
  }

  printf("Choose your password: ");
  read_wrapper(0, pw_buf, 15);
  pw_buf[strcspn(pw_buf, "\n")] = '\0';

  // No duplicate ID check for simplicity.
  new_user = malloc_wrapper(sizeof(user_node));
  memset(new_user, 0, sizeof(user_node));
  strncpy(new_user->id, id_buf, 16);
  strncpy(new_user->pw, pw_buf, 16);
  strcpy(new_user->mail, "(empty)");
  new_user->next = head;
  head->prev = new_user;
  new_user->prev = NULL;
  head = new_user;
}

void main_service(void) {
  enum action choice;

  while (1) {
    choice = outer_menu();
    if (choice == LOGIN)
      login();
    else if (choice == JOIN)
      join();
    else if (choice == EXIT)
      exit(0);
    else
      puts("[ERROR] Invalid choice\n");
  }
}

int main(void) {
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  init_admin();
  main_service();
  return 0;
}
