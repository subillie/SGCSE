#include "heapsort.h"

/* funcion prototypes */
void *Malloc(size_t size);
void insert_max_heap(element item, int *n);
void ascend(int *n, int count, FILE *fp_out);
void descend(int *n, int count, FILE *fp_out);

/* main */
int main(int ac, char **av) {

	int i, insertNum, count = 0;
	char *line = (char *)Malloc(sizeof(char) * (MAX_LEN + 1));
	element item;
	clock_t start, end;

	/* If the number of command-line arguments is not 1 */
	if (ac != 2) {
		fprintf(stderr, "usage: ./hw2 input_filename\n"); exit(1);
	}
	FILE *fp_in = fopen(av[1], "r");
	/* If the input file does not exist */
	if (access(av[1], F_OK) == -1) {
		fprintf(stderr, "The input file does not exist.\n"); exit(1);
	}
	FILE *fp_out = fopen("hw2_result.txt", "w");
	/* If failed to open input or output file */
	if (!fp_in || !fp_out) {
		fprintf(stderr, "Error: file open failed.\n"); exit(1);
	}

	start = clock();
	/* read lines from the input file, and execute the commands */
	while (fgets(line, MAX_LEN, fp_in)) {
		if (line[0] == 'I') {      /* insert */
			insertNum = atoi(line + 7);
			item.key = insertNum;
			insert_max_heap(item, &n);
			count++;
		}
		else if (line[0] == 'A' || line[0] == 'D') {
			// tmp에 복사
			// tmp를 delete하면서 print
		}
		// else if (line[0] == 'A') { /* ascend */
		// 	for (i = count; i > 1; i /= 2) { fprintf(fp_out, "%d ", heap[i].key); }
		// 	fprintf(fp_out, "%d\n", heap[i].key);
		// }
		// else if (line[0] == 'D') { /* descend */
		// 	for (i = 1; i <= count; i *= 2) { fprintf(fp_out, "%d ", heap[i].key); }
		// 	fprintf(fp_out, "%d\n", heap[i].key);
		// }
		else {                     /* invalid command */
			fprintf(stderr, "Invalid command.\n");
			fclose(fp_in);
			fclose(fp_out);
			exit(1);
		}
	}
	end = clock();

	free(line);
	printf("output written to hw2_result.txt.\n");
	printf("running time: %lf seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
	fclose(fp_in);
	fclose(fp_out);
	return 0;
}

/* insert item into a max heap of current size *n */
void insert_max_heap(element item, int *n) {

	if (HEAP_FULL(*n)) {
		fprintf(stderr, "The heap is full.\n");
		exit(1);
	}

	int i = ++(*n);
	while ((i != 1) && (item.key > heap[i / 2].key)) {
		heap[i] = heap[i / 2];
		i /= 2;
	}
	heap[i] = item;
}

/* wrapper function of malloc */
void *Malloc(size_t size) {

	void *ptr = malloc(size);
	if (!ptr) {
		fprintf(stderr, "Memory allocation failed.\n");
		exit(1);
	}
	return ptr;
}
