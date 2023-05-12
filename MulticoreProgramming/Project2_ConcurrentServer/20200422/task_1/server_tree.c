/* 
* server_tree.c - Utility functions for dealing with the data structure tree 
*/ 
#include "csapp.h"
#include "stockserver.h"

void free_tree(stock_item_t *ptr) {
	if (ptr == NULL) {
		return;
	}
	free_tree(ptr->left);
	free_tree(ptr->right);
	free(ptr);
}

void traverse(stock_item_t *ptr, char *list) {

	if (ptr == NULL) {
		return;
	}
	traverse(ptr->left, list);
	sprintf(list + strlen(list), "%d %d %d\n", ptr->id, ptr->left_stock, ptr->price);
	traverse(ptr->right, list);
}

stock_item_t *find(int id) {

	stock_item_t *ptr = root;

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

void print_inorder(stock_item_t *ptr, FILE *fp) {

	if (ptr == NULL) {
		return;
	}
	print_inorder(ptr->left, fp);
	fprintf(fp, "%d %d %d\n", ptr->id, ptr->left_stock, ptr->price);
	print_inorder(ptr->right, fp);
}

void upload_file() {

	FILE *fp = fopen("stock.txt", "w");
	print_inorder(root, fp);
	fclose(fp);
}
