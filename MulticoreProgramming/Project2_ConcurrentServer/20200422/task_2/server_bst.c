/* 
* server_tree.c - Utility functions for dealing with the data structure tree 
*/ 
#include "csapp.h"
#include "stockserver.h"

void update_textfile() {

	P(&file_mutex);

	/* $begin critical section */
	FILE *fp = fopen("stock.txt", "w");
	if (!fp) {
		printf("File open error\n");
		V(&file_mutex);
		return;
	}
	print_fp(root, fp);
	fclose(fp);
	/* $end critical section */

	V(&file_mutex);
}

void add_item(item_t **ptr, int id, int quantity, int price) {

	if ((*ptr) == NULL) {
		(*ptr) = (item_t *)Malloc(sizeof(item_t));
		(*ptr)->id = id;
		(*ptr)->quantity = quantity;
		(*ptr)->price = price;
		(*ptr)->readcnt = 0;
		Sem_init(&(*ptr)->mutex, 0, 1);
		Sem_init(&(*ptr)->writer, 0, 1);
		(*ptr)->left = NULL;
		(*ptr)->right = NULL;
	}
	else {
		if (id < (*ptr)->id)
			add_item((&(*ptr)->left), id, quantity, price);
		else if (id > (*ptr)->id)
			add_item((&(*ptr)->right), id, quantity, price);
		else
			app_error("add_item error: Item already exists");
	}
}

void init_items() {

	int id, quantity, price;

	FILE *fp = fopen("stock.txt", "r");
	if (fp == NULL) {
		app_error("file open error");
		exit(1);
	}
	nstock = 0;
	while (fscanf(fp, "%d %d %d", &id, &quantity, &price) != EOF) {
		printf("[%d %d %d]\n", id, quantity, price);
		add_item(&root, id, quantity, price);
		nstock++;
	}
	fclose(fp);
}

void free_tree(item_t *ptr) {

	if (ptr == NULL) {
		return;
	}
	free_tree(ptr->left);
	free_tree(ptr->right);
	free(ptr);
}

item_t *find(int id) {

	item_t *ptr = root;

	/* Find the node with the given id */
	while (ptr != NULL) {
		if (id < ptr->id)
			ptr = ptr->left;
		else if (id > ptr->id)
			ptr = ptr->right;
		else
			break;
	}
	return ptr;
}

void print_fp(item_t *ptr, FILE *fp) {

	if (ptr == NULL)
		return;

	print_fp(ptr->left, fp);
	/* Reader's lock */
	P(&ptr->mutex);
	ptr->readcnt++;
	if (ptr->readcnt == 1)
		P(&ptr->writer);
	V(&ptr->mutex);

	/* $begin critical section */
	fprintf(fp, "%d %d %d\n", ptr->id, ptr->quantity, ptr->price);
	fflush(fp);
	/* $end critical section */

	/* Reader's unlock */
	P(&ptr->mutex);
	ptr->readcnt--;
	if (ptr->readcnt == 0)
		V(&ptr->writer);
	V(&ptr->mutex);
	print_fp(ptr->right, fp);
}
