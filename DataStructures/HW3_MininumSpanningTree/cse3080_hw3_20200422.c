#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int ac, char *av[]) {
	if (ac != 2) {
		printf("usage: ./hw3 input_filename\n"); return 0;
	}
	FILE *infile = fopen(av[1], "r");
	if (infile == NULL) {
		printf("The input file does not exist.\n"); return 0;
	}
	clock_t start = clock();
	// TODO: implement code
	clock_t end = clock();
	printf("running time: %.6lf\n", ((double)(end - start)) / CLOCKS_PER_SEC);

	fclose(infile);
	return 0;
}