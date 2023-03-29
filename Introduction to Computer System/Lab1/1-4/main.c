#include <stdio.h>
#include <stdlib.h>
#include "bitset.h"

int main(int argc, char ** argv) {
    FILE *fp;
    int x, ret;
    unsigned char set[16] = { 0 };

    if (argc != 2) {
        printf("Usage: %s <input file path>\n", argv[0]);
        exit(1);
    }

    fp = fopen(argv[1], "rt");
    if (fp == NULL) {
        printf("Failed to open %s\n", argv[1]);
        exit(1);
    }


    while ((ret = fscanf(fp, "%d", &x)) != EOF) {
        if (ret != 1) {
            printf("Failed to read an integer (invalid format)\n");
            fclose(fp);
            exit(1);
        }
        if (x < 0 || x > 127) {
            printf("Invalid range: %d\n", x);
            fclose(fp);
            exit(1);
        }
        addNumber(set, x);
    }
    fclose(fp);

    for (int i = 0; i < sizeof(set); i++) {
      printf("%.2x ", set[i]);
    }
    printf("\n");

    return 0;
}

