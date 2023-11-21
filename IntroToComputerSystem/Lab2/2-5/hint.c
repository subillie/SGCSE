#include <stdio.h>
#include <string.h>

typedef struct Student {
    char name[8];
    char major[8];
} Student;

/* This function will print a secret string to you. Your goal is to execute this
 * function by exploiting buffer overflow vulnerability.
 */
void print_secret(void);

/* Prints out the usage (menu) of this service */
void usage(void) {
    puts("<Menu>");
    puts("A. Add a new student");
    puts("U. Update the major of a student");
    puts("E. Exit");
}

/* Safely read up to max_size-1 bytes and terminate with NULL chracter. Newline
 * character is replaced with NULL character, as in gets(). */
int safe_gets(char * buf, int max_size) {
    char * p = fgets(buf, max_size, stdin);
    int len = strlen(buf);
    if (len > 0 && buf[len-1] == '\n')
        buf[len-1] = '\0';
    if (p == NULL)
        return -1;
    else
        return 0;
}

void add_student(Student* students, int max_id) {
    /* TODO: Figure out what this function is doing */
}

void update_major(Student* students, int max_id) {
    /* TODO: Figure out what this function is doing */
}


void manage_students(void) {
    /* TODO: Figure out what this function is doing */
}

int main(void) {
    manage_students();
    return 0;
}
