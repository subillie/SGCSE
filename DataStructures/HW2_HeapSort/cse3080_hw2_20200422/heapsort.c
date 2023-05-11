#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <time.h>

#define MAX_LEN 17 /* max length of commands + max size of integer + 1 */
#define MAX_ELEMENTS 1000000
#define HEAP_FULL(n) (n == MAX_ELEMENTS - 1)
#define HEAP_EMPTY(n) (!n)

/* global variables */
typedef struct {
	int key;
} element;

element heap[MAX_ELEMENTS];
int n = 0;

/* funcion prototypes */
void *Malloc(size_t size);
void insert_max_heap(element item, int *n);
void insert_min_heap(element *tmpHeap, element tmpItem, int *tmpN);
element delete_max_heap(int *tmpN, element *tmpHeap);
element delete_min_heap(int *tmpN, element *tmpHeap);

/* main */
int main(int ac, char **av) {

	int i, newItem, tmpN;
	char *line = (char *)Malloc(sizeof(char) * (MAX_LEN + 1));
	element item, tmpItem, tmpHeap[MAX_ELEMENTS];
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
		fprintf(stderr, "error: file open failed.\n"); exit(1);
	}

	start = clock();
	/* read lines from the input file, and execute the commands */
	while (fgets(line, MAX_LEN, fp_in)) {
		if (line[0] == 'I') {        /* insert */
			newItem = atoi(line + 7);
			item.key = newItem;
			insert_max_heap(item, &n);
		} else if (line[0] == 'A') { /* ascend */
			tmpN = 0;
			for (i = 1; i <= n; i++) { insert_min_heap(tmpHeap, heap[i], &tmpN); }
			for (i = 1; i <= n; i++) {
				tmpItem = delete_min_heap(&tmpN, tmpHeap);
				fprintf(fp_out, "%d ", tmpItem.key);
			}
			fprintf(fp_out, "\n");
		} else {                     /* descend */
			tmpN = n;
			for (i = 1; i <= n; i++) { tmpHeap[i] = heap[i]; }
			for (i = 1; i <= n; i++) {
				tmpItem = delete_max_heap(&tmpN, tmpHeap);
				fprintf(fp_out, "%d ", tmpItem.key);
			}
			fprintf(fp_out, "\n");
		}
	}
	end = clock();

	free(line);
	fclose(fp_in);
	fclose(fp_out);
	printf("output written to hw2_result.txt.\n");
	printf("running time: %lf seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
	return 0;
}

/* insert item into a max heap of current size *n */
void insert_max_heap(element item, int *n) {

	if (HEAP_FULL(*n)) {
		fprintf(stderr, "The heap is full.\n"); exit(1);
	}

	int i = ++(*n);
	while ((i != 1) && (item.key > heap[i / 2].key)) {
		heap[i] = heap[i / 2];
		i /= 2;
	}
	heap[i] = item;
}

/* insert item into a min heap of current size n */
void insert_min_heap(element *tmpHeap, element tmpItem, int *tmpN) {

	int i = ++(*tmpN);
	while ((i != 1) && (tmpItem.key < tmpHeap[i / 2].key)) {
		tmpHeap[i] = tmpHeap[i / 2];
		i /= 2;
	}
	tmpHeap[i] = tmpItem;
}

/* delete element with the highest key from the heap */
element delete_max_heap(int *tmpN, element *tmpHeap) {

	if (HEAP_EMPTY(*tmpN)) {
		fprintf(stderr, "The heap is empty"); exit(1);
	}
	/* save value of the element with the largest key */
	element item = tmpHeap[1];
	/* use the last element in the heap to adjust heap */
	element temp = tmpHeap[(*tmpN)--];

	int parent = 1, child = 2;
	while (child <= *tmpN) {
		/* find the larger child of the current parent */
		if((child < *tmpN) && (tmpHeap[child].key < tmpHeap[child + 1].key)) child++;
		if(temp.key >= tmpHeap[child].key) break;
		/* move to the next lower level */
		tmpHeap[parent] = tmpHeap[child];
		parent = child;
		child *= 2;
	}
	tmpHeap[parent] = temp;
	return item;
}

/* delete element with the highest key from the heap */
element delete_min_heap(int *tmpN, element *tmpHeap) {

	if (HEAP_EMPTY(*tmpN)) {
		fprintf(stderr, "The heap is empty"); exit(1);
	}
	/* save value of the element with the largest key */
	element item = tmpHeap[1];
	/* use the last element in the heap to adjust heap */
	element temp = tmpHeap[(*tmpN)--];

	int parent = 1, child = 2;
	while (child <= *tmpN) {
		/* find the larger child of the current parent */
		if((child < *tmpN) && (tmpHeap[child].key > tmpHeap[child + 1].key)) child++;
		if(temp.key <= tmpHeap[child].key) break;
		/* move to the next lower level */
		tmpHeap[parent] = tmpHeap[child];
		parent = child;
		child *= 2;
	}
	tmpHeap[parent] = temp;
	return item;
}

/* wrapper function of malloc */
void *Malloc(size_t size) {

	void *ptr = malloc(size);
	if (!ptr) {
		fprintf(stderr, "error: memory allocation failed.\n");
		exit(1);
	}
	return ptr;
}
