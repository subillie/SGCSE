#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main() {

    FILE *fp_history = fopen("history.txt", "w");
    fprintf(fp_history, "HI\n");
    fprintf(fp_history, "HELLO\n");
    char *line;
    fgets(line, 10, fp_history)
    printf("%s\n", line);
    int seek = lseek(*fp_history, 0, SEEK_SET);
    fgets(line, 10, fp_history);
    printf("%d, %s\n", seek, line);
    fclose(fp_history);

    return 0;
}