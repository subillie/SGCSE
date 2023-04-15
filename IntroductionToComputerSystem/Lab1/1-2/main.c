#include <stdio.h>
#include <stdlib.h>
#include "absVal.h"

int main(int argc, char ** argv) {
    FILE *fp;
    int x, ret;

    if (argc != 2) {
        printf("Usage: %s <input file path>\n", argv[0]);
        exit(1);
    }

    fp = fopen(argv[1], "rt");
    if (fp == NULL) {
        printf("Failed to open %s\n", argv[1]);
        exit(1);
    }

    ret = fscanf(fp, "%d", &x);
    fclose(fp);
    if (ret == EOF) {
        printf("Empty input file\n");
        exit(1);
    } else if (ret != 1) {
        printf("Failed to read an integer (invalid format)\n");
        exit(1);
    }

    printf("%d\n", absVal(x));

    return 0;
}

