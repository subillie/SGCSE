/* 
* server_tree.c - Utility functions for dealing with the data structure tree 
*/ 
#include "csapp.h"
#include "stockserver.h"

void add_item(item_t *ptr, int id, int quantity, int price) {

	if (ptr == NULL) {
		ptr = (item_t *)Malloc(sizeof(item_t));
		ptr->id = id;
		ptr->quantity = quantity;
		ptr->price = price;
		ptr->left = NULL;
		ptr->right = NULL;
	}
	else {
		if (id < ptr->id)
			add_item(ptr->left, id, quantity, price);
		else if (id > ptr->id)
			add_item(ptr->right, id, quantity, price);
		else
			app_error("add_item error: Item already exists");
	}
}

void init_items() {

	int id, quantity, price;

	FILE *fp = fopen("stock.txt", "r");
	nstock = 0;
	while (fscanf(fp, "%d %d %d", &id, &quantity, &price) != EOF)
		add_item(root, id, quantity, price);
	fclose(fp);
	print_inorder(root, stdout);
}

static void delete_in_tree(item_t *ptr, item_t *parent, item_t *child) {

	if (parent == NULL)
		root = child;
	else if (parent->left == ptr)
		parent->left = child;
	else
		parent->right = child;
}

void delete_item(int id, pool_t *pool) {

	item_t *ptr = root;
	item_t *parent = NULL;

	/* Find the node with the given id */
	while (ptr != NULL) {
		if (id < ptr->id) {
			parent = ptr;
			ptr = ptr->left;
		}
		else if (id > ptr->id) {
			parent = ptr;
			ptr = ptr->right;
		}
		else
			break;
	}
	if (ptr == NULL)
		app_error("delete_item error: Item not found");

	/* Delete the node */
	if (ptr->left == NULL && ptr->right == NULL)
		delete_in_tree(ptr, parent, NULL);
	else if (ptr->left == NULL)
		delete_in_tree(ptr, parent, ptr->right);
	else if (ptr->right == NULL)
		delete_in_tree(ptr, parent, ptr->left);
	else {
		item_t *tmp_ptr = ptr->right;
		item_t *tmp_parent = ptr;

		for (; tmp_ptr->left != NULL; tmp_ptr = tmp_ptr->left)
			tmp_parent = tmp_ptr;
		if (tmp_parent->left == tmp_ptr)
			tmp_parent->left = tmp_ptr->right;
		else
			tmp_parent->right = tmp_ptr->right;
		ptr->id = tmp_ptr->id;
		ptr->quantity = tmp_ptr->quantity;
		ptr->price = tmp_ptr->price;
		ptr = tmp_ptr;
	}
	free(ptr);
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

void print_inorder(item_t *ptr, FILE *fp) {

	if (ptr == NULL)
		return;
	print_inorder(ptr->left, fp);
	fprintf(fp, "%d %d %d\n", ptr->id, ptr->quantity, ptr->price);
	print_inorder(ptr->right, fp);
}

void upload_file() {

	FILE *fp = fopen("stock.txt", "w");
	print_inorder(root, fp);
	fclose(fp);
}
