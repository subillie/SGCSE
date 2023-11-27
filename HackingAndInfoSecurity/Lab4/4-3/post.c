#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#define HEADER_SIZE 168

typedef struct post {
  char title[32];
  char content[128];
  long int max_comment;
  long int cur_comment;
  char *comment_buf;
} post;

post *posts[8];

void write_string(char *s);
void* malloc_wrapper(size_t size);
ssize_t read_wrapper(int fd, void *buf, size_t count);
long int read_int(void);
int post_menu(post *p);
void add_comment(post *p);
void view_comment(post *p);
void edit_comment(post *p);
void do_open(post *p);
void create_post(void);
void save_post(void);
void load_post(void);
void open_post(void);
void post_system(void);
int main_menu(void);

void write_string(char *s) {
  write(1, s, strlen(s));
}

void* malloc_wrapper(size_t size) {
  void* p = malloc(size);
  if (p == NULL) {
    write_string("Failed to allocate memory\n");
    exit(1);
  }
  return p;
}

ssize_t read_wrapper(int fd, void *buf, size_t count) {
  ssize_t n = read(fd, buf, count);
  if (n <= 0) {
    write_string("read() failed\n");
    exit(1);
  }
  return n;
}

long int read_int(void) {
  char buf[24];
  ssize_t n = read_wrapper(0, buf, 23);
  buf[n] = '\0';
  return strtol(buf, NULL, 10);
}

int post_menu(post *p) {
  write_string("=== Post Menu ===\n");
  write_string("1. Read content\n");
  write_string("2. Add comment\n");
  write_string("3. View comment\n");
  write_string("4. Edit comment\n");
  write_string("5. Close\n");
  write_string("(Input your choice in number): ");
  return read_int();
}

void add_comment(post *p) {
  char buf[24];
  size_t len;

  if (p->cur_comment >= p->max_comment) {
    write_string("Cannot add more comment!\n");
    return;
  }

  write_string("Input your comment: ");
  memset(buf, 0, sizeof(buf));
  fgets(buf, 15, stdin);
  buf[strcspn(buf, "\n")] = '\0';
  // At this point, len is at most 14.
  strcpy(p->comment_buf + 16 * (p->cur_comment++), buf);
}

void view_comment(post *p) {
  long int m;

  write_string("Input the index of comment to view: ");
  m = read_int();

  if (m < 0 || m >= p->max_comment) {
    write_string("Invalid comment number range\n");
    return;
  }

  write_string("Comment: ");
  write_string(p->comment_buf + 16 * m);
  write_string("\n");
}

void edit_comment(post *p) {
  char buf[24];
  size_t len;
  long int m;

  write_string("Input the index of comment to edit: ");
  m = read_int();

  if (m < 0 || m >= p->max_comment) {
    write_string("Invalid comment number range\n");
    return;
  }

  write_string("Input a new comment: ");
  memset(buf, 0, sizeof(buf));
  fgets(buf, 15, stdin);
  buf[strcspn(buf, "\n")] = '\0';
  // At this point, len is at most 14.
  strcpy(p->comment_buf + 16 * m, buf);
}

void do_open(post *p) {
  int i;
  while(1) {
    i = post_menu(p);
    switch(i) {
      case 1:
        write_string("Content: ");
        write_string(p->content);
        write_string("\n");
        break;
      case 2:
        add_comment(p);
        break;
      case 3:
        view_comment(p);
        break;
      case 4:
        edit_comment(p);
        break;
      case 5:
        return;
      default:
        write_string("Invalid choice\n");
        break;
    }
  }
}

void create_post(void) {
  char path[24];
  post *p;

  for (int i = 0; i < 8; i++) {
    strcpy(path, "files/post-");
    path[11] = '0' + (char) i;
    path[12] = '\0';
    if (posts[i] || access(path, F_OK) == 0) {
      continue;
    }

    p = (post*) malloc_wrapper(sizeof(post));
    memset(p, 0, sizeof(post));

    write_string("Input the title of post: ");
    read_wrapper(0, p->title, 32 - 1);
    p->title[strcspn(p->title, "\n")] = '\0';

    write_string("Input the content of post: ");
    read_wrapper(0, p->content, 128 - 1);
    p->content[strcspn(p->content, "\n")] = '\0';

    write_string("Input the maximum number of comments to allow: ");
    p->max_comment = read_int();
    if (p->max_comment < 4 || p->max_comment > 16) {
      write_string("Valid range of maximum comment is 4~16\n");
      free(p);
      return;
    }
    p->cur_comment = 0;
    p->comment_buf = malloc_wrapper(16 * p->max_comment);
    memset(p->comment_buf, 0, 16 * p->max_comment);
    posts[i] = p;
    return;
  }

  write_string("All the post entries are full!\n");
}

void save_post(void) {
  long int n;
  FILE *fp;
  char path[24];
  post *p;
  char *tmp;

  write_string("Input the index of post you want to save: ");
  n = read_int();

  if (n < 0 || n >= 8) {
    write_string("Invalid post number range\n");
    return;
  }

  if (posts[n] == NULL) {
    write_string("Post does not exist\n");
    return;
  }

  p = posts[n];

  strcpy(path, "files/post-");
  path[11] = '0' + (char) n;
  path[12] = '\0';
  fp = fopen(path, "w");
  if (fp == NULL) {
    write_string("[ERROR] Failed to open file for save()");
    return;
  }

  fwrite(p, sizeof(char), HEADER_SIZE, fp);
  for (int i = 0; i < p->cur_comment; i++) {
    tmp = p->comment_buf + 16 * i;
    fwrite(tmp, sizeof(char), strlen(tmp), fp);
    fputc('\n', fp);
  }
  fclose(fp);
  free(posts[n]->comment_buf);
  free(posts[n]);
  posts[n] = NULL;
}

void load_post(void) {
  int direct_open = 0;
  long int n;
  size_t len;
  FILE *fp;
  char path[24];
  char buf[4];
  char *tmp;
  post *p;

  write_string("Input the index of post you want to load: ");
  n = read_int();

  if (n < 0 || n >= 8) {
    write_string("Invalid post number range\n");
    return;
  }

  if (posts[n] != NULL) {
    write_string("Post already in the memory\n");
    return;
  }

  strcpy(path, "files/post-");
  path[11] = '0' + (char) n;
  path[12] = '\0';
  fp = fopen(path, "r");
  if (fp == NULL) {
    write_string("[ERROR] Failed to open file for load()\n");
    return;
  }

  p = (post*) malloc_wrapper(sizeof(post));
  len = fread(p, sizeof(char), HEADER_SIZE, fp);

  if (len != HEADER_SIZE) {
    write_string("[ERROR] Failed to read the post header\n");
    fclose(fp);
    free(p);
    return;
  }

  write_string("Do you want to open it directly after load? (Y/N): ");
  read_wrapper(0, buf, sizeof(buf));
  if (buf[0] == 'Y' || buf[0] == 'y')
    direct_open = 1;

  p->comment_buf = malloc_wrapper(16 * p->max_comment);
  memset(p->comment_buf, 0, 16 * p->max_comment);

  p->cur_comment = 0;
  while (fgets(p->comment_buf + 16 * p->cur_comment, 16, fp) != NULL) {
    tmp = p->comment_buf + 16 * p->cur_comment;
    tmp[strcspn(tmp, "\n")] = '\0';
    p->cur_comment++;
  }

  posts[n] = p;
  fclose(fp);

  if (direct_open) {
    do_open(p);
  }
}

void open_post(void) {
  long int n;

  write_string("Input the index of post you want to open: ");
  n = read_int();

  if (n < 0 || n >= 8) {
    write_string("Invalid post number range\n");
    return;
  }

  if (posts[n] == NULL) {
    write_string("Post not created or loaded\n");
    return;
  }

  do_open(posts[n]);
}

int main_menu(void) {
  write_string("=== Main Menu ===\n");
  write_string("1. Create post\n");
  write_string("2. Save post\n");
  write_string("3. Load post\n");
  write_string("4. Open post\n");
  write_string("5. Exit\n");
  write_string("(Input your choice in number): ");
  return read_int();
}

void post_system(void) {
  int i;
  while(1) {
    i = main_menu();
    switch(i) {
      case 1:
        create_post();
        break;
      case 2:
        save_post();
        break;
      case 3:
        load_post();
        break;
      case 4:
        open_post();
        break;
      case 5:
        exit(0);
      default:
        write_string("Invalid choice\n");
        break;
    }
  }
}

int main(void) {
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  post_system();
  return 0;
}
