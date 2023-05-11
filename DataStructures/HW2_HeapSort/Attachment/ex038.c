#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ELEMENTS 200
#define HEAP_FULL(n) (n == MAX_ELEMENTS-1)
#define HEAP_EMPTY(n) (!n)

typedef struct {
int key;
} element;
element heap[MAX_ELEMENTS];
int n = 0;

void insert_max_heap(element item, int *n);
element delete_max_heap(int *n);

int main() {

int i, rn;
element item;

srand(time(NULL));

printf("INSERTING: ");
for(i=0; i<10; i++) {
  rn = rand() % 100 + 1;
  printf("%3d ", rn);
  item.key = rn;
  insert_max_heap(item, &n);
}
printf("\n\n");

printf("DELETING:  ");
for(i=0; i<10; i++) {
  item = delete_max_heap(&n);
  printf("%3d ", item.key);
}
printf("\n");
return 0;
}

void insert_max_heap(element item, int *n) {
/* insert item into a max heap of current size *n */
int i;
if(HEAP_FULL(*n)) {
  fprintf(stderr, "The heap is full.\n");
  exit(1);
}
i = ++(*n);
while((i != 1) && (item.key > heap[i/2].key)) {
  heap[i] = heap[i/2];
  i /= 2;
}
heap[i] = item;
}

element delete_max_heap(int *n) {
/* delete element with the highest key from the heap */
int parent, child;
element item, temp;
if(HEAP_EMPTY(*n)) {
  fprintf(stderr, "The heap is empty");
  exit(1);
}
/* save value of the element with the largest key */
item = heap[1];
/* use the last element in the heap to adjust heap */
temp = heap[(*n)--];
parent = 1;
child = 2;
while(child <= *n) {
  /* find the larger child of the current parent */
  if((child < *n) && (heap[child].key < heap[child+1].key)) child++;
  if(temp.key >= heap[child].key) break;
  /* move to the next lower level */
  heap[parent] = heap[child];
  parent = child;
  child *= 2;
}
heap[parent] = temp;
return item;
}

