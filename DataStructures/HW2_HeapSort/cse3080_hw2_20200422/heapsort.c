#include "heapsort.h"

/* funcion prototypes */
void *Malloc(size_t size);
void insert_max_heap(element item, int *n);
element delete_max_heap(int *n, element *tmpHeap);
element delete_min_heap(int *n, element *tmpHeap, int count);

/* main */
int main(int ac, char **av) {

	int i, newItem, count = 0;
	char *line = (char *)Malloc(sizeof(char) * (MAX_LEN + 1));
	element item, tmpItem;
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
		/* insert */
		if (line[0] == 'I') {
			newItem = atoi(line + 7);
			item.key = newItem;
			insert_max_heap(item, &n);
			count++;
		} else {
			element tmpHeap[MAX_ELEMENTS];
			for (i = 1; i <= count; i++) { tmpHeap[i] = heap[i]; }
			/* ascend */
			if (line[0] == 'A') {
				for (i = 1; i <= count; i++) {
					tmpItem = delete_max_heap(&n, tmpHeap);
					fprintf(fp_out, "%d ", tmpItem.key);
				}
			/* descend */
			} else {
				tmpItem = delete_min_heap(&n, tmpHeap, count);
				for (i = 1; i <= count; i++) { fprintf(fp_out, "%d ", tmpItem.key); }
			}
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

	if (HEAP_FULL(*n)) { fprintf(stderr, "The heap is full.\n"); exit(1); }

	int i = ++(*n);
	while ((i != 1) && (item.key > heap[i / 2].key)) {
		heap[i] = heap[i / 2];
		i /= 2;
	}
	heap[i] = item;
}


/* delete element with the highest key from the heap */
element delete_max_heap(int *n, element *tmpHeap) {

	if(HEAP_EMPTY(*n)) { fprintf(stderr, "The heap is empty"); exit(1); }
	/* save value of the element with the largest key */
	element item = tmpHeap[1];
	/* use the last element in the heap to adjust heap */
	element temp = tmpHeap[(*n)--];

	int parent = 1, child = 2;
	while(child <= *n) {
		/* find the larger child of the current parent */
		if((child < *n) && (tmpHeap[child].key < tmpHeap[child+1].key)) child++;
		if(temp.key >= tmpHeap[child].key) break;
		/* move to the next lower level */
		tmpHeap[parent] = tmpHeap[child];
		parent = child;
		child *= 2;
	}
	tmpHeap[parent] = temp;
	return item;
}

/* delete element with the lowest key from the heap */
element delete_min_heap(int *n, element *tmpHeap, int count) {

	if(HEAP_EMPTY(*n)) { fprintf(stderr, "The heap is empty"); exit(1); }
	/* save value of the element with the largest key */
	element item = tmpHeap[1];
	/* use the last element in the heap to adjust heap */
	element temp = tmpHeap[(*n)--];

	int parent = 1, child = 2;
	while(child <= *n) {
		/* find the larger child of the current parent */
		if((child < *n) && (tmpHeap[child].key < tmpHeap[child+1].key)) child++;
		if(temp.key >= tmpHeap[child].key) break;
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
		fprintf(stderr, "Memory allocation failed.\n");
		exit(1);
	}
	return ptr;
}
