/* 
* server_utils.c - Utility functions for the concurrent stock server 
*/ 
#include "csapp.h"
#include "stockserver.h"

void add_fd(int connfd, pool_t *pool) {

	if (++(pool->nclient) > FD_SETSIZE)
		app_error("add_client error: Too many clients");

	if (pool->listenfd < connfd)
		pool->listenfd = connfd;
	client_t *ptr = (client_t *)malloc(sizeof(client_t));
	ptr->fd = connfd;
	ptr->next = NULL;

	if (client_head == NULL) {
		client_head = ptr;
		client_tail = ptr;
	}
	else {
		client_tail->next = ptr;
		client_tail = ptr;
	}
}

void delete_fd(int connfd, pool_t *pool) {

	if (--(pool->nclient) < 0)
		app_error("delete_client error: Too few clients");

	client_t *ptr = client_head, *prev = NULL;
	for (; ptr != NULL; ptr = ptr->next) {
		if (ptr->fd == connfd) {
			if (ptr == client_head && ptr == client_tail) {	/* If the node is the only node */
				client_head = NULL;
				client_tail = NULL;
			}
			else if (ptr == client_head)	/* If the node is the head */
				client_head = ptr->next;
			else if (ptr == client_tail) {	/* If the node is the tail */
				client_tail = prev;
				client_tail->next = NULL;
			}
			else							/* If the node is in the middle */
				prev->next = ptr->next;
			break;
		}
		prev = ptr;
	}
	free(ptr);
	Close(connfd);
	FD_CLR(connfd, &pool->read_set);
}

void add_stock(int id, int quantity, int price, pool_t *pool) {

	stock_t *item = (stock_t *)malloc(sizeof(stock_t));
	item->id = id;
	item->quantity = quantity;
	item->price = price;
	(pool->nstock)++;
	item->left = NULL;
	item->right = NULL;

	if (root == NULL) {
		root = item;
		return;
	}

	stock_t *prev = NULL, *curr = root;
	while (curr != NULL) {
		prev = curr;
		if (id < curr->id)
			curr = curr->left;
		else if (id > curr->id)
			curr = curr->right;
		else
			return;
	}
	if (id < prev->id)
		prev->left = item;
	else
		prev->right = item;
}

static void sigint_handler(int sig) {

	upload_file();
	free_tree(root);
	exit(0);
}

void init_pool(char *listenfd, pool_t *pool) {

	FILE *fp = fopen("stock.txt", "r");
	int id, quantity, price;
	Signal(SIGINT, sigint_handler);
	pool->nstock = 0;
	while (fscanf(fp, "%d %d %d", &id, &quantity, &price) != EOF)
		add_stock(id, quantity, price, pool);
	fclose(fp);
	printf("Server initialized\n");
	print_inorder(root, fp);

	/* Initially, listenfd is only member of select read set */
	pool->listenfd = Open_listenfd(listenfd);
	FD_ZERO(&pool->read_set);
	FD_SET(pool->listenfd, &pool->read_set);
}

static void show(int connfd) {

	char list[MAXLINE];

	list[0] = '\0';
	traverse(root, list);
	strcat(list, "\n");
	Rio_writen(connfd, list, strlen(list));
}

static void buy(int connfd, int id, int count) {

	/* Find the given id in tree */
	stock_t *ptr = find(id);

	/* If the node is not found, return */
	if (ptr == NULL) {
		Rio_writen(connfd, "Invalid ID\n", 11);
		return;
	}

	/* If the node is found, update the stock */
	if (ptr->quantity >= count) {
		ptr->quantity -= count;
		Rio_writen(connfd, "[buy] success\n", 14);
	}
	else /* If there is not enough stock, return */
		Rio_writen(connfd, "Not enough left stock\n", 22);
}

static void sell(int connfd, int id, int count) {

	/* Find the node with the given id */
	stock_t *ptr = find(id);

	/* If not found, return */
	if (ptr == NULL) {
		Rio_writen(connfd, "Invalid ID\n", 11);
		return;
	}
	/* If found, update the stock */
	ptr->quantity += count;
	Rio_writen(connfd, "[sell] success\n", 15);
}

void execute_command(int connfd, int buflen, char *buf, pool_t *pool) {

	int id, count;
	char command[MAXLINE];

	if (!strcmp(buf, "exit\n")) {
		Close(connfd);
		FD_CLR(connfd, &pool->read_set);
		pool->client->fd = -1;
		Rio_writen(connfd, buf, buflen);
		upload_file();
	}
	else if (!strcmp(buf, "\n"))
		Rio_writen(connfd, buf, buflen);
	else {
		sscanf(buf, "%s %d %d", command, &id, &count);
		if (!strcmp(command, "show"))
			show(connfd);
		else if (!strcmp(command, "buy"))
			buy(connfd, id, count);
		else if (!strcmp(command, "sell"))
			sell(connfd, id, count);
		else
			Rio_writen(connfd, "Invalid command\n", 16);
	}
}
