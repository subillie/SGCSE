#ifndef HEAPSORT_H
#define HEAPSORT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <time.h>

#define MAX_LEN 17 /* key must be integer */
#define MAX_ELEMENTS 1000000
#define HEAP_FULL(n) (n == MAX_ELEMENTS - 1)
#define HEAP_EMPTY(n) (!n)

/* global variables */
typedef struct {
	int key;
} element;

element heap[MAX_ELEMENTS];
int n = 0;

#endif