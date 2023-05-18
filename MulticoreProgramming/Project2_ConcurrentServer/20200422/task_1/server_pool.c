/* 
* server_utils.c - Utility functions for the concurrent stock server 
*/ 
#include "csapp.h"
#include "stockserver.h"

void init_pool(int listenfd, pool_t *pool) {

	/* Initially, there are no connected descriptors */
	pool->maxi = -1;
	for (int i = 0; i < FD_SETSIZE; i++)
		pool->clientfd[i] = -1;

	/* Initially, listenfd is only member of select read set */
	pool->maxfd = listenfd;
	pool->nready = 0;
	FD_ZERO(&pool->read_set);
	FD_SET(listenfd, &pool->read_set);
}

void add_client(int connfd, pool_t *pool) {

	int i;

	pool->nready--;
	/* Find an available slot in the clientfd array */
	for (i = 0; i < FD_SETSIZE; i++) {
		if (pool->clientfd[i] < 0) {
			/* Add connected descriptor to the pool */
			pool->clientfd[i] = connfd;
			Rio_readinitb(&pool->clientrio[i], connfd);

			/* Add the descriptor to the descriptor set */
			FD_SET(connfd, &pool->read_set);

			/* Update max descriptor and pool high water mark */
			if (pool->maxfd < connfd)
				pool->maxfd = connfd;
			if (pool->maxi < i)
				pool->maxi = i;
			break;
		}
	}
	/* Couldn't find an empty slot */
	if (i == FD_SETSIZE)
		app_error("add_client error: Too many clients");
}

void check_clients(pool_t *pool) {

	int connfd, n = 0;
	char buf[MAXLINE];
	rio_t rio;

	/* Check for data from connected clients */
	for (int i = 0; (i <= pool->maxi) && (pool->nready > 0); i++) {
		connfd = pool->clientfd[i];
		rio = pool->clientrio[i];

		/* If the descriptor is ready, echo a text line from it */
		if ((connfd > 0) && (FD_ISSET(connfd, &pool->ready_set))) {
			pool->nready--;
			if (Rio_readnb(&rio, buf, 1) != 0) {
				while (buf[n++] != '\n')
					Rio_readnb(&rio, buf + n, 1);
				printf("Server received %d bytes\n", n);
				execute_command(i, connfd, n, buf, pool);
				memset(&rio, 0, MAXLINE);
				n = 0;
			}
			/* EOF detected, remove descriptor from pool */
			else {
				Close(connfd);
				FD_CLR(connfd, &pool->read_set);
				pool->clientfd[i] = -1;
			}
		}
	}
}

// static void print_rio(item_t *ptr, char *list) {

// 	if (ptr == NULL)
// 		return;

// 	print_rio(ptr->left, list);
// 	sprintf(list + strlen(list), "%d %d %d\n", ptr->id, ptr->quantity, ptr->price);
// 	print_rio(ptr->right, list);
// }

// static void show(int connfd) {

// 	char list[MAXLINE];

// 	memset(list, 0, MAXLINE);
// 	print_rio(root, list);
// 	printf("len: %lu, list: <%s>\n", strlen(list), list); //TODO: remove
// 	Rio_writen(connfd, list, strlen(list));
// }

static void print_rio(item_t *ptr, int connfd) {

	char list[MAXLINE];
	if (ptr == NULL)
		return;

	print_rio(ptr->left, connfd);
	sprintf(list, "%d %d %d\n", ptr->id, ptr->quantity, ptr->price);
	Rio_writen(connfd, list, strlen(list));
	memset(list, 0, MAXLINE);
	print_rio(ptr->right, connfd);
}

static void show(int connfd) {

	char list[MAXLINE];

	memset(list, 0, MAXLINE);
	print_rio(root, connfd);
}

// static void show(item_t *ptr, int connfd) {

// 	if (ptr == NULL)
// 		return;

// 	char list[MAXLINE];
// 	list[0] = '\0';
// 	show(ptr->left, connfd);
// 	sprintf(list, "%d %d %d\n", ptr->id, ptr->quantity, ptr->price);
// 	printf("list: %s", list); //TODO: remove
// 	Rio_writen(connfd, list, strlen(list));
// 	show(ptr->right, connfd);
// }

// static void show(item_t *ptr, int connfd) {

// 	if (ptr == NULL)
// 		return;

// 	char list[MAXLINE];
// 	memset(list, 0, MAXLINE);
// 	show(ptr->left, connfd);
// 	sprintf(list, "%d %d %d\n", ptr->id, ptr->quantity, ptr->price);
// 	Rio_writen(connfd, list, strlen(list));
// 	show(ptr->right, connfd);
// }

// static void show_helper(item_t *ptr, char *list, int connfd) {
// 	if (ptr == NULL)
// 		return;

// 	// Traverse the left subtree
// 	show_helper(ptr->left, list, connfd);

// 	// Append the stock information to the list string
// 	sprintf(list + strlen(list), "%d %d %d\n", ptr->id, ptr->quantity, ptr->price);

// 	// Check if the list buffer is full
// 	if (strlen(list) >= MAXLINE - 1) {
// 		// Send the accumulated stock information to the client
// 		Rio_writen(connfd, list, strlen(list));
// 		memset(list, 0, MAXLINE); // Clear the buffer after sending
// 	}

// 	// Traverse the right subtree
// 	show_helper(ptr->right, list, connfd);

// 	// Check if there is remaining stock information in the buffer
// 	if (strlen(list) > 0) {
// 		// Send the remaining accumulated stock information to the client
// 		Rio_writen(connfd, list, strlen(list));
// 		memset(list, 0, MAXLINE); // Clear the buffer after sending
// 	}
// }

// static void show(item_t *ptr, int connfd) {
// 	if (ptr == NULL)
// 		return;

// 	char list[MAXLINE];
// 	memset(list, 0, MAXLINE); // Clear the buffer before use
// 	show_helper(ptr, list, connfd);
// }

// static void show(int connfd) {

// 	FILE *fp = fopen("stock.txt", "r");
// 	if (fp == NULL) {
// 		Rio_writen(connfd, "Error: Stock file not opened\n", 29);
// 		return;
// 	}

// 	char buffer[MAXLINE];
// 	memset(buffer, 0, MAXLINE);

// 	size_t bytesRead;
// 	while ((bytesRead = fread(buffer, sizeof(char), MAXLINE - 1, fp)) > 0) {
// 		Rio_writen(connfd, buffer, bytesRead);
// 		memset(buffer, 0, MAXLINE);
// 	}

// 	fclose(fp);
// }

static void buy(int connfd, int id, int count) {

	/* Find the given id in tree */
	item_t *ptr = find(id);

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
	item_t *ptr = find(id);

	/* If not found, return */
	if (ptr == NULL) {
		Rio_writen(connfd, "Invalid ID\n", 11);
		return;
	}
	/* If found, update the stock */
	ptr->quantity += count;
	Rio_writen(connfd, "[sell] success\n", 15);
}

void execute_command(int i, int connfd, int buflen, char *buf, pool_t *pool) {

	int id, count;
	char command[MAXLINE];

	if (!strcmp(buf, "exit\n")) {
		update_textfile();
		Close(connfd); /* Close the connection */
		FD_CLR(connfd, &pool->read_set);
		pool->clientfd[i] = -1;
		return;        /* Return after closing the connection */
	}
	sscanf(buf, "%s %d %d", command, &id, &count);
	if (!strcmp(command, "show"))
		show(connfd);
	else if (!strcmp(command, "buy")) {
		buy(connfd, id, count);
		update_textfile();
	}
	else if (!strcmp(command, "sell")) {
		sell(connfd, id, count);
		update_textfile();
	}
	else 
		Rio_writen(connfd, "Invalid command\n", 16);
}
