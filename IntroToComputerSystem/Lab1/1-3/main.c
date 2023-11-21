#include <stdio.h>
#include <stdlib.h>
#include "conditional.h"

int main(int argc, char ** argv) {
    FILE *fp;
    int x, y, z, ret;

    if (argc != 2) {
        printf("Usage: %s <input file path>\n", argv[0]);
        exit(1);
    }

    fp = fopen(argv[1], "rt");
    if (fp == NULL) {
        printf("Failed to open %s\n", argv[1]);
        exit(1);
    }

    ret = fscanf(fp, "%d %d %d", &x, &y, &z);
    fclose(fp);
    if (ret == EOF) {
        printf("Empty input file\n");
        exit(1);
    } else if (ret != 3) {
        printf("Failed to read three integers (invalid format)\n");
        exit(1);
    }

    printf("%d\n", conditional(x, y, z));

    return 0;
}

